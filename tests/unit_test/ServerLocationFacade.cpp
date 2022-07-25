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

TEST_F(ServerLocationFacadeTest, basic) {
    WebservConfig config =
        WebservConfig::Parse("../../../test_data/config/webserv/default.conf");
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    ServerLocation sl = facade.Choose("80", "", "/");
    ASSERT_EQ(80, sl.port());
    ASSERT_EQ("", sl.host());
    ASSERT_EQ("/", sl.path());
    ASSERT_EQ("/app/sample_data/html", sl.alias());
    ASSERT_EQ("off", sl.auto_index());
}

TEST_F(ServerLocationFacadeTest, default_server) {
    WebservConfig config =
        WebservConfig::Parse("../../../test_data/config/webserv/default.conf");
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    ServerLocation sl = facade.Choose("80", "not_matching", "/");
    ASSERT_EQ(80, sl.port());
    ASSERT_EQ("", sl.host());
    ASSERT_EQ("/", sl.path());
    ASSERT_EQ("/app/sample_data/html", sl.alias());
    ASSERT_EQ("off", sl.auto_index());
}

TEST_F(ServerLocationFacadeTest, longest_path_match) {
    WebservConfig config =
        WebservConfig::Parse("../../../test_data/config/webserv/default.conf");
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    ServerLocation sl = facade.Choose("80", "", "/files/hoge.html");
    ASSERT_EQ(80, sl.port());
    ASSERT_EQ("", sl.host());
    ASSERT_EQ("/files/", sl.path());
    ASSERT_EQ("/app/sample_data/files", sl.alias());
    ASSERT_EQ("off", sl.auto_index());
}
