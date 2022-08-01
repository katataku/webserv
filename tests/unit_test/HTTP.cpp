#include <gtest/gtest.h>

#include "HTTPException.hpp"
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
    ASSERT_EQ(req.canonical_path(), "/");
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
    ASSERT_EQ(req.canonical_path(), "/");
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
    ASSERT_EQ(req.request_target(), "/");
    ASSERT_EQ(req.canonical_path(), "/");
    ASSERT_EQ(req.host(), "test");
    ASSERT_EQ(req.content_length(), 8);
    ASSERT_EQ(req.transfer_encoding(), "");
    ASSERT_EQ(req.request_body(), "12345678");
}

TEST_F(HTTPTest, parse_body_by_chuncked_onetime) {
    HTTPRequest req = HTTPRequest();
    req.Parse(
        "POST /cgi-bin/file_manager.py HTTP/1.1\r\n"
        "Host: test\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "6\r\nhello,\r\n"
        "6\r\nworld!\r\n"
        "0\r\n\r\n");
    ASSERT_EQ(req.method(), "POST");
    ASSERT_EQ(req.request_target(), "/cgi-bin/file_manager.py");
    ASSERT_EQ(req.canonical_path(), "/cgi-bin/file_manager.py");
    ASSERT_EQ(req.host(), "test");
    ASSERT_EQ(req.content_length(), -1);
    ASSERT_EQ(req.transfer_encoding(), "chunked");
    ASSERT_EQ(req.request_body(), "hello,world!");
}

TEST_F(HTTPTest, parse_body_by_chuncked) {
    HTTPRequest req = HTTPRequest();
    req.Parse("POST /cgi-bin/file_manager.py HTTP/1.1\r\n");
    req.Parse("Host: test\r\n");
    req.Parse("Transfer-Encoding: chunked\r\n");
    req.Parse("\r\n");
    req.Parse("6\r\nhello,\r\n");
    req.Parse("6\r\nworld!\r\n");
    req.Parse("0\r\n\r\n");
    ASSERT_EQ(req.method(), "POST");
    ASSERT_EQ(req.request_target(), "/cgi-bin/file_manager.py");
    ASSERT_EQ(req.canonical_path(), "/cgi-bin/file_manager.py");
    ASSERT_EQ(req.host(), "test");
    ASSERT_EQ(req.content_length(), -1);
    ASSERT_EQ(req.transfer_encoding(), "chunked");
    ASSERT_EQ(req.request_body(), "hello,world!");
}

TEST_F(HTTPTest, remove_dot_segment) {
    HTTPRequest req = HTTPRequest();
    req.Parse(
        "GET /a/b/c/./../../g HTTP/1.1\r\n"
        "Host: test\r\n"
        "\r\n");
    ASSERT_EQ(req.request_target(), "/a/b/c/./../../g");
    ASSERT_EQ(req.canonical_path(), "/a/g");
}

TEST_F(HTTPTest, cannot_remove_directory) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET /a/b/./../../../g HTTP/1.1\r\n"
        "Host: test\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, header_names_are_case_insensitive) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "HOST: test1\r\n"
        "content-TYPE: text/html\r\n"
        "content-length: 8\r\n"
        "\r\n"
        "12345678";
    req.Parse(str);
    ASSERT_EQ(req.method(), "GET");
    ASSERT_EQ(req.request_target(), "/");
    ASSERT_EQ(req.host(), "test1");
    ASSERT_EQ(req.content_type(), "text/html");
    ASSERT_EQ(req.content_length(), 8);
    ASSERT_EQ(req.request_body(), "12345678");
}

TEST_F(HTTPTest, no_host_header) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, method_is_missing_in_request_line) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "/ HTTP/1.1\r\n"
        "Host: test1\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, request_line_has_extra_segment) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / / HTTP/1.1\r\n"
        "Host: test1\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, protocol_vesrion_not_supported) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.2\r\n"
        "Host: test1\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

// nginxは400を返しているがRFC7231に準拠して501を返す
TEST_F(HTTPTest, method_not_supported) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "TOKYO / HTTP/1.1\r\n"
        "Host: test1\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, uri_too_long) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET "
        "/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa HTTP/1.1\r\n"
        "Host: test1\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

// nginxではエラーにならないがRFCに準拠してエラーにする
TEST_F(HTTPTest, multiple_hot_header) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "Host: test1\r\n"
        "Host: test2\r\n"
        "\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

// IsIntegerの修正が取り込まれるまではdisableにする
TEST_F(HTTPTest, DISABLED_content_length_with_plus_sign) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "Host: test\r\n"
        "Content-Length: +8\r\n"
        "\r\n"
        "12345678";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, content_length_with_non_numberic_char) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "Host: test\r\n"
        "Content-Length: 8a\r\n"
        "\r\n"
        "12345678";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, content_length_is_only_one) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "Host: test\r\n"
        "Content-Length: 8\r\n"
        "Content-Length: 8\r\n"
        "\r\n"
        "12345678";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, content_length_leading_zero_is_ignored) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "Host: test\r\n"
        "Content-Length: 08\r\n"
        "\r\n"
        "12345678";
    req.Parse(str);
    ASSERT_EQ(req.request_body(), "12345678");
    ASSERT_EQ(req.content_length(), 8);
}

TEST_F(HTTPTest, content_length_0_is_acceptable) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "Host: test\r\n"
        "Content-Length: 0\r\n"
        "\r\n"
        "12345678";
    req.Parse(str);
    ASSERT_EQ(req.request_body(), "");
    ASSERT_EQ(req.content_length(), 0);
}

TEST_F(HTTPTest, transfer_encoding_is_not_chunked) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "POST /cgi-bin/file_manager.py HTTP/1.1\r\n"
        "Host: test\r\n"
        "Transfer-Encoding: hunked\r\n"
        "\r\n"
        "6\r\nhello,\r\n"
        "6\r\nworld!\r\n"
        "0\r\n\r\n";
    ASSERT_THROW(req.Parse(str), HTTPException);
}

TEST_F(HTTPTest, content_length_and_transfer_encoding_in_header) {
    HTTPRequest req = HTTPRequest();
    std::string str =
        "GET / HTTP/1.1\r\n"
        "Host: test\r\n"
        "Content-Length: 8\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "12345678";
    ASSERT_THROW(req.Parse(str), HTTPException);
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
