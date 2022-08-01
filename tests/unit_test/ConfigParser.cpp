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
