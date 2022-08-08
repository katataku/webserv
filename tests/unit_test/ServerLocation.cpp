#include "ServerLocation.hpp"

#include <gtest/gtest.h>

class ServerLocationTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ServerLocationTest, IsCGI) {
    ServerLocation sl;

    std::set<std::string> cgis;
    cgis.insert("py");
    cgis.insert("sh");

    sl.set_cgi_extensions(cgis);

    ASSERT_EQ(sl.IsCGI("/"), false);
    ASSERT_EQ(sl.IsCGI("/html/index.html"), false);
    ASSERT_EQ(sl.IsCGI("/sample_data/cgi-bin/ubuntu_cgi_tester"), false);

    ASSERT_EQ(sl.IsCGI("/sample_data/cgi-bin/cgi_test.py"), true);
    ASSERT_EQ(sl.IsCGI("/sample_data/cgi-bin/cgi_test.sh"), true);
}

TEST_F(ServerLocationTest, resolveAlias_relative_path) {
    ServerLocation sl;

    sl.set_path("/files/");
    sl.set_alias("./sample_data/files/");

    ASSERT_EQ(sl.ResolveAlias("/files/hoge.txt"),
              "./sample_data/files/hoge.txt");
}
