#include "ServerLocationFacade.hpp"

#include <gtest/gtest.h>

#include "WebservConfig.hpp"

class ServerLocationFacadeTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ServerLocationFacadeTest, Parse) {
    WebservConfig config =
        WebservConfig::Parse("../../../test_data/config/webserv/default.conf");
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    ServerLocation* sl = facade.Choose("80", "", "/");
    ASSERT_EQ("80", sl->port());
    ASSERT_EQ("", sl->host());
    ASSERT_EQ("/", sl->path());
    ASSERT_EQ("/app/sample_data/html", sl->alias());
    ASSERT_EQ("off", sl->auto_index());
}
