#include <gtest/gtest.h>
#include <unistd.h>

#include "ConfigProcesser.hpp"
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

    ASSERT_EQ(serv_context.contexts().size(), 1);
    ASSERT_EQ(serv_context.port(), 80);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    ASSERT_EQ(locate_contexts.size(), 1);

    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.alias(), "/var/www/html");
}

TEST_F(ConfigParserTest, autoindex_on_location) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/autoindex_on_location.conf");
    WebservConfig conf = confproc.Exec();

    ASSERT_EQ(conf.auto_index(), false);

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.auto_index(), false);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.auto_index(), true);
}

TEST_F(ConfigParserTest, autoindex_on_server) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/autoindex_on_server.conf");
    WebservConfig conf = confproc.Exec();

    ASSERT_EQ(conf.auto_index(), false);

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.auto_index(), true);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.auto_index(), false);
}

TEST_F(ConfigParserTest, autoindex_on_http) {
    ConfigProcesser confproc(
        "../../../test_data/config/webserv/ok/autoindex_on_http.conf");
    WebservConfig conf = confproc.Exec();

    ASSERT_EQ(conf.auto_index(), true);

    std::vector<ServerContext> serv_contexts = conf.contexts();
    ServerContext serv_context = serv_contexts.at(0);
    ASSERT_EQ(serv_context.auto_index(), false);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    LocationContext locate_context = locate_contexts.at(0);
    ASSERT_EQ(locate_context.auto_index(), false);
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
