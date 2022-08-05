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
    WebservConfig config = WebservConfig::Parse(
        "../../../test_data/config/webserv/ok/default.conf");
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    ServerLocation sl = facade.Choose("80", "", "/");
    ASSERT_EQ(80, sl.port());
    ASSERT_EQ("default", sl.host());
    ASSERT_EQ("/", sl.path());
    ASSERT_EQ("/app/sample_data/html/", sl.alias());
    ASSERT_EQ("off", sl.auto_index());
}

TEST_F(ServerLocationFacadeTest, default_server) {
    WebservConfig config = WebservConfig::Parse(
        "../../../test_data/config/webserv/ok/default.conf");
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    ServerLocation sl = facade.Choose("80", "not_matching", "/");
    ASSERT_EQ(80, sl.port());
    ASSERT_EQ("default", sl.host());
    ASSERT_EQ("/", sl.path());
    ASSERT_EQ("/app/sample_data/html/", sl.alias());
    ASSERT_EQ("off", sl.auto_index());
}

TEST_F(ServerLocationFacadeTest, longest_path_match) {
    WebservConfig config = WebservConfig::Parse(
        "../../../test_data/config/webserv/ok/default.conf");
    std::vector<ServerLocation> locations = config.CreateServerLocations();
    ServerLocationFacade facade(locations);
    ServerLocation sl = facade.Choose("80", "", "/files/hoge.html");
    ASSERT_EQ(80, sl.port());
    ASSERT_EQ("default", sl.host());
    ASSERT_EQ("/files/", sl.path());
    ASSERT_EQ("/app/sample_data/files/", sl.alias());
    ASSERT_EQ("off", sl.auto_index());
}

TEST_F(ServerLocationFacadeTest, get_ports_single) {
    std::vector<ServerLocation> sls;

    ServerLocation sl1;
    sl1.set_port(80);
    sls.push_back(sl1);

    ServerLocationFacade sl_facade(sls);

    std::vector<std::string> ports = sl_facade.GetPorts();

    ASSERT_EQ(ports.size(), 1);
    ASSERT_EQ(ports.at(0), "80");
}

TEST_F(ServerLocationFacadeTest, get_ports_multi) {
    std::vector<ServerLocation> sls;

    ServerLocation sl1;
    sl1.set_port(80);
    ServerLocation sl2;
    sl2.set_port(8080);
    sls.push_back(sl1);
    sls.push_back(sl2);

    ServerLocationFacade sl_facade(sls);

    std::vector<std::string> ports = sl_facade.GetPorts();

    ASSERT_EQ(ports.size(), 2);
    ASSERT_EQ(ports.at(0), "80");
    ASSERT_EQ(ports.at(1), "8080");
}

TEST_F(ServerLocationFacadeTest, get_ports_duplicate) {
    std::vector<ServerLocation> sls;

    ServerLocation sl1;
    sl1.set_port(80);
    ServerLocation sl2;
    sl2.set_port(80);
    sls.push_back(sl1);
    sls.push_back(sl2);

    ServerLocationFacade sl_facade(sls);

    std::vector<std::string> ports = sl_facade.GetPorts();

    ASSERT_EQ(ports.size(), 1);
    ASSERT_EQ(ports.at(0), "80");
}