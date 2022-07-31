#include <gtest/gtest.h>
#include <unistd.h>

#include "ConfigProcesser.hpp"
#include "InitialValues.hpp"
#include "LocationContext.hpp"
#include "ServerContext.hpp"
#include "WebservConfig.hpp"

class ConfigParserTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ConfigParserTest, LocationContextInServerContext) {
    ConfigProcesser confproc("../../../test_data/config/webserv/default.conf");
    WebservConfig conf = confproc.Exec();

    ASSERT_EQ(conf.contexts().size(), 1);

    std::vector<ServerContext> serv_contexts = conf.contexts();

    ASSERT_EQ(serv_contexts.size(), 1);

    ServerContext serv_context = serv_contexts.at(0);

    ASSERT_EQ(serv_context.contexts().size(), 2);
    ASSERT_EQ(serv_context.port(), 80);
    ASSERT_EQ(serv_context.server_name(), "default");

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    ASSERT_EQ(locate_contexts.size(), 2);

    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.alias(), "/app/sample_data/html");

    locate_context = locate_contexts.at(1);
    ASSERT_EQ(locate_context.alias(), "/app/sample_data/files");
}

TEST_F(ConfigParserTest, autoindex_on_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/autoindex_on_location.conf");
    WebservConfig conf = confproc.Exec();

    ASSERT_EQ(conf.auto_index(), "");

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.auto_index(), "");

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.auto_index(), "on");
}

TEST_F(ConfigParserTest, autoindex_on_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/autoindex_on_server.conf");
    WebservConfig conf = confproc.Exec();

    ASSERT_EQ(conf.auto_index(), "");

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.auto_index(), "on");

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.auto_index(), "");
}

TEST_F(ConfigParserTest, autoindex_on_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/autoindex_on_http.conf");
    WebservConfig conf = confproc.Exec();

    ASSERT_EQ(conf.auto_index(), "on");

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.auto_index(), "");

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.auto_index(), "");
}

TEST_F(ConfigParserTest, multi_directive) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/multi_directive.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();

    ASSERT_EQ(serv_contexts.size(), 2);

    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    ASSERT_EQ(locate_contexts.size(), 3);
}

TEST_F(ConfigParserTest, cgi_extension) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/cgi_extension.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();

    ASSERT_EQ(serv_contexts.size(), 1);

    ServerContext serv_context = serv_contexts.at(0);

    ASSERT_EQ(serv_context.port(), 80);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    ASSERT_EQ(locate_contexts.size(), 1);

    LocationContext locate_context = locate_contexts.at(0);

    ASSERT_EQ(locate_context.path(), "/");
    ASSERT_EQ(locate_context.alias(), "/var/www/html");
    ASSERT_EQ(locate_context.cgi_extension(), "py");
}

TEST_F(ConfigParserTest, alias) {
    ConfigProcesser confproc("../../../test_data/config/webserv/ok/alias.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.alias(), "/aaa/bbb/ccc");
}

TEST_F(ConfigParserTest, alias_no_newline) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/alias_no_newline.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.alias(), "/aaa/bbb/ccc");
}

TEST_F(ConfigParserTest, redirect_on_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/return_on.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.redirect_url(), "https://google.com");
}

TEST_F(ConfigParserTest, redirect_on_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/return_on_server.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.redirect_url(), "https://google.com");

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.redirect_url(), "");
}

TEST_F(ConfigParserTest, error_page) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/error_page.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.error_pages().at(404), "/error_page/404.html");
}

TEST_F(ConfigParserTest, error_page_multi) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/error_page_multi.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.error_pages().at(404), "/error_page/404.html");
    ASSERT_EQ(locate_context.error_pages().at(500), "/error_page/50x.html");
    ASSERT_EQ(locate_context.error_pages().at(502), "/error_page/50x.html");
    ASSERT_EQ(locate_context.error_pages().at(503), "/error_page/50x.html");
    ASSERT_EQ(locate_context.error_pages().at(504), "/error_page/50x.html");
}

TEST_F(ConfigParserTest, error_page_override) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/error_page_override.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.error_pages().at(400), "/error_page/40x.html");
    ASSERT_EQ(serv_context.error_pages().at(404), "/error_page/40x.html");

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.error_pages().at(404), "/error_page/404.html");
    ASSERT_EQ(locate_context.error_pages().at(500), "/error_page/50x.html");
    ASSERT_EQ(locate_context.error_pages().at(502), "/error_page/50x.html");
    ASSERT_EQ(locate_context.error_pages().at(503), "/error_page/50x.html");
    ASSERT_EQ(locate_context.error_pages().at(504), "/error_page/50x.html");
}

TEST_F(ConfigParserTest, error_page_not_exist) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/error_page_not_exist.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.error_pages().at(404), "/error_page/999.html");
}

TEST_F(ConfigParserTest, error_page_same_code) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/error_page_same_code.conf");
    WebservConfig conf = confproc.Exec();
    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.error_pages().at(404), "/error_page/404.html");
}

TEST_F(ConfigParserTest, client_max_body_size_on_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/"
        "client_max_body_size_on_location.conf");
    WebservConfig conf = confproc.Exec();
    ASSERT_EQ(conf.client_max_body_size(), InitialValues::kClientMaxBodySize);

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.client_max_body_size(),
              InitialValues::kClientMaxBodySize);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.client_max_body_size(), 1);
}

TEST_F(ConfigParserTest, client_max_body_size_on_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/"
        "client_max_body_size_on_server.conf");
    WebservConfig conf = confproc.Exec();
    ASSERT_EQ(conf.client_max_body_size(), InitialValues::kClientMaxBodySize);

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.client_max_body_size(), 1);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.client_max_body_size(),
              InitialValues::kClientMaxBodySize);
}

TEST_F(ConfigParserTest, client_max_body_size_on_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/"
        "client_max_body_size_on_http.conf");
    WebservConfig conf = confproc.Exec();
    ASSERT_EQ(conf.client_max_body_size(), 1);

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.client_max_body_size(),
              InitialValues::kClientMaxBodySize);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.client_max_body_size(),
              InitialValues::kClientMaxBodySize);
}

class ConfigParserDeathTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ConfigParserDeathTest, return_in_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "return_in_http.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, server_name_in_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "server_name_in_http.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, listen_in_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "listen_in_http.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, alias_in_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "alias_in_http.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, limit_except_in_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "limit_except_in_http.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, cgi_extension_in_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "cgi_extension_in_http.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, location_in_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "location_in_http.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, alias_in_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "alias_in_server.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, cgi_extension_in_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "cgi_extension_in_server.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, limit_except_in_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "limit_except_in_server.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, server_in_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "server_in_server.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, server_name_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "server_name_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, listen_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "listen_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, server_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "server_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, location_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/different_context/"
        "location_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is not allowed here");
}

TEST_F(ConfigParserDeathTest, error_page_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "error_page.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, error_page_none_value_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "error_page_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, error_page_none_value_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "error_page_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, client_max_body_size_multi_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "client_max_body_size.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, client_max_body_size_multi_value_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "client_max_body_size_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, client_max_body_size_multi_value_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "client_max_body_size_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, server_name_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "server_name.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, listen_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "listen.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, alias_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "alias.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, limit_except_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "limit_except.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, autoindex_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "autoindex.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, autoindex_none_value_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "autoindex_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, autoindex_none_value_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "autoindex_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, index_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "index.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, index_none_value_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "index_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, index_none_value_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "index_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, return_none_value_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "return_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, return_none_value_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "return_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, cgi_extension_none_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value_size/"
        "cgi_extension.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: invalid number of arguments in \".*\" directive");
}

TEST_F(ConfigParserDeathTest, invalid_server_name_value) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "server_name.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_listen_value_zero) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "listen_zero.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_listen_value_minus) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "listen_minus.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_listen_value_big) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "listen_big.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_listen_value_mix) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "listen_mix.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_error_page_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "error_page_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_error_page) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "error_page.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_error_page_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "error_page_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_error_page_mix) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "error_page_mix.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_error_page_mix_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "error_page_mix_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_error_page_mix_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "error_page_mix_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_client_max_body_size) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "client_max_body_size.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_client_max_body_size_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "client_max_body_size_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_client_max_body_size_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "client_max_body_size_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_client_max_body_size_mix) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "client_max_body_size_mix.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_client_max_body_size_mix_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "client_max_body_size_mix_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_client_max_body_size_mix_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "client_max_body_size_mix_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_limit_except) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "limit_except.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_autoindex) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "autoindex.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_autoindex_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "autoindex_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_autoindex_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "autoindex_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_return_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "return_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, invalid_return_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/invalid_value/"
        "return_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive invalid value .*");
}

TEST_F(ConfigParserDeathTest, duplicate_server_name) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "server_name.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_listen) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "listen.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_client_max_body_size) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "client_max_body_size.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_client_max_body_size_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "client_max_body_size_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_client_max_body_size_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "client_max_body_size_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_alias) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "alias.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_limit_except) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "limit_except.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_autoindex) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "autoindex.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_autoindex_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "autoindex_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_autoindex_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "autoindex_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_index) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "index.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_index_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "index_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_index_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "index_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_return_in_serv) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "return_in_serv.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_return_in_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "return_in_location.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, duplicate_cgi_extension) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/duplicate/"
        "cgi_extension.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: \".*\" directive is duplicate");
}

TEST_F(ConfigParserDeathTest, unexpected_server1) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "server1.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unexpected_server2) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "server2.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unexpected_server5) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "server5.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

// TEST_F(ConfigParserDeathTest, unexpected_server4) {
//     ConfigProcesser confproc(
//         "../../../test_data/config/webserv/error/unexpected/"
//         "server4.conf");
//     WebservConfig conf;

//     EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
//                 "Error: unexpected .*, expecting \".*\"");

//     exit(1);
// }

TEST_F(ConfigParserDeathTest, unexpected_server3) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "server3.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unexpected_semicoron1) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "semicoron1.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unexpected_semicoron2) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "semicoron2.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unexpected_location1) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "location1.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unexpected_location2) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "location2.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unexpected_empty) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unexpected/"
        "empty.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}

TEST_F(ConfigParserDeathTest, unknown_directive) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/error/unknown/"
        "directive.conf");
    WebservConfig conf;

    EXPECT_EXIT(conf = confproc.Exec(), testing::ExitedWithCode(1),
                "Error: unexpected .*, expecting .*");
}
