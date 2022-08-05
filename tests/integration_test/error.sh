#!/bin/bash

source tests/integration_test/integration_test.sh

IS_ERROR_TEST=1

#コンテナ直近化のために、冒頭に一度だけコンテナビルドを行う
echo "Building docker container for test"
${COMMAND_MAKE_DC_BUILD} > /dev/null 2>&1

echo "test case means : [config][request]"
echo ""

#ひとつひとつのテストを個別に実行することもできる。
#CONFIGを変更した後はstart_server_containerを実行すること。

    CONFIG_NO=limit_except.conf
    start_server_container

        #405エラー
        REQUEST_NO=GET_simple
        do_test

    CONFIG_NO=default.conf
    start_server_container

        #404エラー
        REQUEST_NO=GET_incorrect_path
        do_test

        REQUEST_NO=host_not_exist
        do_test

        REQUEST_NO=method_not_supported
        do_test

        REQUEST_NO=protocol_version_not_supported
        do_test

        REQUEST_NO=uri_too_long
        do_test

    CONFIG_NO=client_max_body_size_on_location.conf
    start_server_container

        #413エラー(content-length)
        REQUEST_NO=POST_hello_world
        do_test

        #413エラー(chunked)
        REQUEST_NO=POST_hello_world_chunked
        do_test

    CONFIG_NO=cgi_extension.conf
    start_server_container

        #500エラー(execve失敗)
        REQUEST_NO=GET_incorrect_py_path
        do_test

        #500エラー(execve成功・子プロセス異常終了)
        REQUEST_NO=GET_cgi_error_exit
        do_test

    CONFIG_NO=default.conf
    start_server_container

        REQUEST_NO=DELETE_not_exist
        do_test

        REQUEST_NO=POST_dir_not_exist
        do_test

        REQUEST_NO=POST_dir
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
