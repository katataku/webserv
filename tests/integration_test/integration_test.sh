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
CONFIG_PATH=./test_data/config/nginx/
REQUEST_PATH=./test_data/request/

OK_SUM=0
NG_SUM=0

CONFIG_ARRAY=(`ls ${CONFIG_PATH} | tr -d 'localhost'`)
REQUEST_ARRAY=(`ls ./test_data/request | tr -d '.sh'`)

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
	echo "[${CONFIG_NO}-${REQUEST_NO}]start test"

    IS_OK=1
    ACTUAL_FILE_NAME=${ACTUAL_PATH}${CONFIG_NO}-${REQUEST_NO}
    EXPECTED_FILE_NAME=${EXPECTED_PATH}${CONFIG_NO}-${REQUEST_NO}
    REQUEST_FILE_NAME=${REQUEST_PATH}${REQUEST_NO}.sh
    
    #---テストのためのリクエスト実行---
    bash ${REQUEST_FILE_NAME} > ${ACTUAL_FILE_NAME} 2>&1

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
    make dc-nginx-re > /dev/null 2>&1
    sleep 1 #コンテナ起動待ち
}


echo "test case means : [config-request]"
echo ""

#全てのCONFIG/REQUESTのパターンを網羅的に実行する。
    for CONFIG_NO in "${CONFIG_ARRAY[@]}"
    do
        # configファイルを変更するごとにコンテナを再作成する。
        start_server_container

        for REQUEST_NO in "${REQUEST_ARRAY[@]}"
        do 
            do_test
        done
    done

#ひとつひとつのテストを個別に実行することもできる。
#CONFIGを変更した後はstart_server_containerを実行すること。
    CONFIG_NO=0001
    start_server_container

    #0001-0001
    REQUEST_NO=0001
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