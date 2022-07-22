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
