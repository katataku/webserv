#include <gtest/gtest.h>

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "ResponseBuilder.hpp"

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

TEST_F(HTTPTest, ResponseBuilder_200) {
    HTTPResponse *res = ResponseBuilder::Build("hoge");

    ASSERT_EQ(res->status_code(), 200);
    ASSERT_EQ(res->response_body(), "hoge");
    ASSERT_EQ(res->content_length(), 4);
    ASSERT_EQ(res->connection(), "close");
}

TEST_F(HTTPTest, ResponseBuilder_200_empty) {
    HTTPResponse *res = ResponseBuilder::Build("");

    ASSERT_EQ(res->status_code(), 200);
    ASSERT_EQ(res->response_body(), "");
    ASSERT_EQ(res->content_length(), 0);
    ASSERT_EQ(res->connection(), "close");
}

TEST_F(HTTPTest, Response_string) {
    std::string expected =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Length: 4\r\n"
        "\r\n"
        "hoge";

    HTTPResponse *res = ResponseBuilder::Build("hoge");
    std::string actual = res->GetResponseString();
    ASSERT_EQ(actual, expected);
}

TEST_F(HTTPTest, Response_string_empty) {
    std::string expected =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Length: 0\r\n";

    HTTPResponse *res = ResponseBuilder::Build("");
    std::string actual = res->GetResponseString();
    ASSERT_EQ(actual, expected);
}
