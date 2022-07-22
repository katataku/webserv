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
    req.Parse(
        "GET / HTTP/1.1\r\n"
        "Host: test\r\n"
        "\r\n");
    ASSERT_EQ(req.method(), "GET");
    ASSERT_EQ(req.request_target(), "/");
    ASSERT_EQ(req.host(), "test");
    ASSERT_EQ(req.content_length(), -1);
    ASSERT_EQ(req.transfer_encoding(), "");
    ASSERT_EQ(req.request_body(), "");
}

TEST_F(HTTPTest, parse_mutiple) {
    HTTPRequest req = HTTPRequest();
    req.Parse("GET / HTTP/1.1\r\n");
    req.Parse("Host: test\r\n");
    req.Parse("\r\n");
    ASSERT_EQ(req.method(), "GET");
    ASSERT_EQ(req.request_target(), "/");
    ASSERT_EQ(req.host(), "test");
    ASSERT_EQ(req.content_length(), -1);
    ASSERT_EQ(req.transfer_encoding(), "");
    ASSERT_EQ(req.request_body(), "");
}

TEST_F(HTTPTest, parse_body_by_content_length) {
    HTTPRequest req = HTTPRequest();
    req.Parse("GET / HTTP/1.1\r\n");
    req.Parse("Host: test\r\n");
    req.Parse("Content-Length: 8\r\n");
    req.Parse("\r\n");
    req.Parse("12345678");
    ASSERT_EQ(req.method(), "GET");
    ASSERT_EQ(req.absolute_path(), "/");
    ASSERT_EQ(req.host(), "test");
    ASSERT_EQ(req.content_length(), 8);
    ASSERT_EQ(req.transfer_encoding(), "");
    ASSERT_EQ(req.request_body(), "12345678");
}

TEST_F(HTTPTest, parse_body_by_chuncked) {
    HTTPRequest req = HTTPRequest();
    req.Parse("POST /cgi-bin/file_manager.py HTTP/1.1\r\n");
    req.Parse("Host: test\r\n");
    req.Parse("Transfer-Encoding: chunked\r\n");
    req.Parse("\r\n");
    req.Parse("6\r\nhello,");
    req.Parse("6\r\nworld!");
    req.Parse("0\r\n\r\n");
    ASSERT_EQ(req.method(), "POST");
    ASSERT_EQ(req.absolute_path(), "/cgi-bin/file_manager.py");
    ASSERT_EQ(req.host(), "test");
    ASSERT_EQ(req.content_length(), -1);
    ASSERT_EQ(req.transfer_encoding(), "chunked");
    ASSERT_EQ(req.request_body(), "hello,world!");
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
