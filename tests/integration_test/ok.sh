#!/bin/bash

source tests/integration_test/integration_test.sh

IS_ERROR_TEST=0

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

    CONFIG_NO=error_page.conf
    start_server_container

        REQUEST_NO=GET_incorrect_path
        do_test

    CONFIG_NO=error_page_multi.conf
    start_server_container

        REQUEST_NO=GET_incorrect_path
        do_test

    CONFIG_NO=error_page_override.conf
    start_server_container

        REQUEST_NO=GET_incorrect_path
        do_test

    CONFIG_NO=error_page_not_exist.conf
    start_server_container

        REQUEST_NO=GET_incorrect_path
        do_test

    CONFIG_NO=error_page_same_code.conf
    start_server_container

        REQUEST_NO=GET_incorrect_path
        do_test

    CONFIG_NO=default.conf
    start_server_container

        REQUEST_NO=POST_hello_world
        do_test
        REQUEST_NO=GET_hello_world
        do_test

        rm -f /app/sample_data/html/hoge

echo    "----------------------------"
echo -n "ALL test finish. Final Conclusion:"
if [ ${NG_SUM} -eq 0 ] ; then
    printf " ${GREEN}[✓](OK:${OK_SUM} / NG:${NG_SUM})${NC}\n"
    exit 0
else
    printf " ${RED}[-](OK:${OK_SUM} / NG:${NG_SUM})${NC}\n"
    exit 1
fi
