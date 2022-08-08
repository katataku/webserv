#!/bin/bash

REQUEST_PATH=./test_data/request/ok/
REQUEST_ARRAY=(`ls ${REQUEST_PATH}`)

CONFIG_PATH=./test_data/config/webserv/ok/
CONFIG_ARRAY=(`ls ${CONFIG_PATH}`)

function do_valgrind_test() {
    # valgrind起動
    docker compose -f ./docker/webserv/docker-compose.yml exec -T -d webserv valgrind --log-file="log/valgrind.log" --leak-check=full ./webserv ${CONFIG_FILE_NAME}
    sleep 1

    # リクエスト実行
    nc localhost 8080 < ${REQUEST_FILE_NAME} > /dev/null

    # killでwebservを終了させ、valgrindの結果ファイルを作成する。
    docker compose -f ./docker/webserv/docker-compose.yml exec -T webserv pkill -SIGINT -f webserv
    sleep 1

    echo "["${CONFIG_FILE_NAME}"]["${REQUEST_FILE_NAME}"]"
    echo "["${CONFIG_FILE_NAME}"]["${REQUEST_FILE_NAME}"]" >> ./log/summary-valgrind.log
    grep definitely < ./log/valgrind.log >> ./log/summary-valgrind.log
    echo ""  >> ./log/summary-valgrind.log
}

mkdir -p ./log
make dc-re > /dev/null 2>&1
docker compose -f ./docker/webserv/docker-compose.yml exec -T webserv make > /dev/null 2>&1

echo "start valgrind test"

# configファイルごとにforループ
for CONFIG_NO in "${CONFIG_ARRAY[@]}"
do 
    CONFIG_FILE_NAME=${CONFIG_PATH}${CONFIG_NO}
    
    # 引数がない場合は、全てのrequestファイルを確認する。
    if [ $# -eq 0 ]; then
        # requestファイルごとにforループ
        for REQUEST_NO in "${REQUEST_ARRAY[@]}"
        do
            REQUEST_FILE_NAME=${REQUEST_PATH}${REQUEST_NO}
            do_valgrind_test
        done
    fi

    # 引数がある場合は、指定されたrequestファイルのみ確認する。
    if [ $# -eq 1 ]; then
        REQUEST_NO=$1
        REQUEST_FILE_NAME=${REQUEST_PATH}${REQUEST_NO}
        do_valgrind_test

        ./tests/valgrind/log_check.sh
        if [ $? -eq 1 ]; then
            cat ./log/log.txt
        fi
    fi

done



