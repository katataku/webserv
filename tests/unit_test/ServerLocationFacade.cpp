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
    WebservConfig config = WebservConfig::Parse(".");
    std::map<ServerLocationKey, ServerLocation> locations =
        config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    facade.Choose("a", "a", "b");
    ASSERT_EQ("", "");
}
