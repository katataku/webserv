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
    ASSERT_EQ(conf.error_pages().empty(), true);
    ASSERT_EQ(conf.client_max_body_size(), 1024);
    ASSERT_EQ(conf.auto_index(), false);
    ASSERT_EQ(conf.index_page(), "");

    std::vector<ServerContext> serv_contexts = conf.contexts();

    ASSERT_EQ(serv_contexts.size(), 1);

    ServerContext serv_context = serv_contexts.at(0);

    ASSERT_EQ(serv_context.contexts().size(), 1);
    ASSERT_EQ(serv_context.error_pages().empty(), true);
    ASSERT_EQ(serv_context.client_max_body_size(), 1024);
    ASSERT_EQ(serv_context.auto_index(), false);
    ASSERT_EQ(serv_context.index_page(), "");
    ASSERT_EQ(serv_context.redirect_url(), "");
    ASSERT_EQ(serv_context.server_name(), "");
    ASSERT_EQ(serv_context.port(), 80);

    std::vector<LocationContext> locate_contexts = serv_context.contexts();
    ASSERT_EQ(locate_contexts.size(), 1);

    LocationContext locate_context = locate_contexts.at(0);

    ASSERT_EQ(locate_context.error_pages().empty(), true);
    ASSERT_EQ(locate_context.client_max_body_size(), 1024);
    ASSERT_EQ(locate_context.auto_index(), false);
    ASSERT_EQ(locate_context.index_page(), "");
    ASSERT_EQ(locate_context.allow_methods().empty(), true);
    ASSERT_EQ(locate_context.alias(), "/var/www/html");
    ASSERT_EQ(locate_context.path(), "/");
    ASSERT_EQ(locate_context.redirect_uri(), "");
}
