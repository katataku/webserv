#include <gtest/gtest.h>

#include "HTTPRequest.hpp"

class HTTPTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(HTTPTest, Parse) {
    HTTPRequest req = HTTPRequest();
    req.Parse("");
    ASSERT_EQ(req.unparsed_string(), "");
    ASSERT_EQ(req.method(), "GET");
    ASSERT_EQ(req.uri(), "http://localhost:8181/index.html");
    ASSERT_EQ(req.host(), "localhost");
    ASSERT_EQ(req.content_length(), "0");
    ASSERT_EQ(req.transfer_encoding(), "");
    ASSERT_EQ(req.request_body(), "");
    ASSERT_EQ(req.is_ready(), true);
}
