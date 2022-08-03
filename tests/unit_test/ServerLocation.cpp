#include <gtest/gtest.h>

#include "ServerLocation.hpp"

class ServerLocationTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ServerLocationTest, IsCGI) {
    ServerLocation sl;

    sl.set_cgi_extension("py");

    ASSERT_EQ(sl.IsCGI("/"), false);
    ASSERT_EQ(sl.IsCGI("/html/index.html"), false);
    ASSERT_EQ(sl.IsCGI("/sample_data/cgi-bin/ubuntu_cgi_tester"), false);

    ASSERT_EQ(sl.IsCGI("/sample_data/cgi-bin/cgi_test.py"), true);
}
