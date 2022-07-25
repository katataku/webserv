#!/bin/bash

NC="\033[0m"
BOLD="\033[1m"
ULINE="\033[4m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"

ACTUAL_PATH=./test_data/actual/
EXPECTED_PATH=./test_data/expected/
CONFIG_PATH=./test_data/config/webserv/ok/
REQUEST_PATH=./test_data/request/ok/

OK_SUM=0
NG_SUM=0

CONFIG_ARRAY=(`ls ${CONFIG_PATH}`)
REQUEST_ARRAY=(`ls ${REQUEST_PATH} | tr -d '.sh'`)

COMMAND_MAKE_DC_RE="make dc-re"
COMMAND_MAKE_DC_DOWN="make dc-down"
COMMAND_MAKE_DC_UP="make dc-up"
COMMAND_MAKE_DC_BUILD="make dc-build"

function do_single_command_check(){
    #引数で与えられたコマンドを実行し、OK/NGを判定。
    $@ > /dev/null 2>&1
    if [ $? -eq 0 ] ;then
		printf "${GREEN}OK${NC}"
	else
		printf "${RED}NG${NC}"
        IS_OK=0
	fi
    echo ""
}

function do_test() {
	echo "[${CONFIG_NO}][${REQUEST_NO}]start test"

    IS_OK=1
    ACTUAL_FILE_NAME=${ACTUAL_PATH}${CONFIG_NO}-${REQUEST_NO}
    EXPECTED_FILE_NAME=${EXPECTED_PATH}${CONFIG_NO}-${REQUEST_NO}
    REQUEST_FILE_NAME=${REQUEST_PATH}${REQUEST_NO}.txt
    
    #---テストのためのリクエスト実行---
    nc localhost 8080 < ${REQUEST_FILE_NAME} > ${ACTUAL_FILE_NAME} 2>&1

    #---個別チェックの実行---
    # 1行目のstatus-lineの完全一致判定
	echo -n "  header line: "
    PICKUP_CMD="sed -n 1p"
    do_single_command_check diff <(${PICKUP_CMD} ${ACTUAL_FILE_NAME}) <(${PICKUP_CMD} ${EXPECTED_FILE_NAME})

#    # 3行目のtimestampなので失敗想定のテスト(結合テスト自体の動作確認用)
#	echo -n "  time line: "
#    PICKUP_CMD="sed -n 3p"
#    do_single_command_check diff <(${PICKUP_CMD} ${ACTUAL_FILE_NAME}) <(${PICKUP_CMD} ${EXPECTED_FILE_NAME})

	echo -n "  Content-Type: "
    PICKUP_CMD="grep Content-Type:"
    do_single_command_check diff <(${PICKUP_CMD} ${ACTUAL_FILE_NAME}) <(${PICKUP_CMD} ${EXPECTED_FILE_NAME})

	echo -n "  Content-Length: "
    PICKUP_CMD="grep Content-Length:"
    do_single_command_check diff <(${PICKUP_CMD} ${ACTUAL_FILE_NAME}) <(${PICKUP_CMD} ${EXPECTED_FILE_NAME})

	echo -n "  Transfer-Encoding: "
    PICKUP_CMD="grep Transfer-Encoding:"
    do_single_command_check diff <(${PICKUP_CMD} ${ACTUAL_FILE_NAME}) <(${PICKUP_CMD} ${EXPECTED_FILE_NAME})

	echo -n "  \"success</p>\": "
    PICKUP_CMD="grep success</p>"
    do_single_command_check diff <(${PICKUP_CMD} ${ACTUAL_FILE_NAME}) <(${PICKUP_CMD} ${EXPECTED_FILE_NAME})



    # サマリーの出力
    echo -n "[${CONFIG_NO}-${REQUEST_NO}]test finish. Conclusion:"
	if [ $IS_OK -eq 1 ] ; then
        OK_SUM=$(( ${OK_SUM}+1))
		printf " ${GREEN}[✓]${NC}\n"
	else
        NG_SUM=$(( ${NG_SUM}+1))
		printf " ${RED}[-]${NC}\n"
		#diff ${ACTUAL_FILE_NAME} ${EXPECTED_FILE_NAME}
	fi
}

function start_server_container() {
    echo "--- starting server container. config:[${CONFIG_NO}] ---"
    cp ${CONFIG_PATH}${CONFIG_NO} ${CONFIG_PATH}localhost
    ${COMMAND_MAKE_DC_DOWN} > /dev/null 2>&1
    ${COMMAND_MAKE_DC_UP} > /dev/null 2>&1
    docker compose -f ./docker/webserv/docker-compose.yml exec -T webserv make
    docker compose -f ./docker/webserv/docker-compose.yml exec -T webserv bash /usr/local/bin/start.sh &
    sleep 1 #コンテナでのwebservプロセス起動待ち。makeは待たないので、1秒程度でOK。
}

#コンテナ直近化のために、冒頭に一度だけコンテナビルドを行う
echo "Building docker container for test"
${COMMAND_MAKE_DC_BUILD} > /dev/null 2>&1

echo "test case means : [config][request]"
echo ""

#ひとつひとつのテストを個別に実行することもできる。
#CONFIGを変更した後はstart_server_containerを実行すること。
    CONFIG_NO=default.conf
    start_server_container

        REQUEST_NO=GET_simple
        do_test

        REQUEST_NO=GET_directory
        do_test

    CONFIG_NO=autoindex_on.conf
    start_server_container

        REQUEST_NO=GET_directory
        do_test

    CONFIG_NO=return_on.conf
    start_server_container

        REQUEST_NO=GET_directory
        do_test

echo    "----------------------------"
echo -n "ALL test finish. Final Conclusion:"
if [ ${NG_SUM} -eq 0 ] ; then
    printf " ${GREEN}[✓](OK:${OK_SUM} / NG:${NG_SUM})${NC}\n"
    exit 0
else
    printf " ${RED}[-](OK:${OK_SUM} / NG:${NG_SUM})${NC}\n"
    exit 1
fi