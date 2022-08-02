#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "CGIExecutor.hpp"
#include "HTTPException.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "ResponseBuilder.hpp"

class CGITest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(CGITest, hello_cgi) {
    /*
        GET /sample_data/cgi-bin/cgi_test.py HTTP/1.1
    */
    HTTPRequest http_req;

    http_req.set_method("GET");
    http_req.set_request_target("/sample_data/cgi-bin/cgi_test.py");

    ServerLocation sl;

    sl.set_alias("./../../..");

    CGIExecutor cgi;

    HTTPResponse *http_resp = cgi.Exec(http_req, sl);

    delete http_resp;

    // ASSERT_EQ(http_resp->status_code(), 200);
    ASSERT_EQ(http_resp->response_body(), "hello from cgi\n");
}

TEST_F(CGITest, post_cgi) {
    /*
        POST /sample_data/cgi-bin/cgi_test.py HTTP/1.1

        hello world
    */
    system("mkdir -p /var/www/html");

    HTTPRequest http_req;

    std::string body = "hello world\n";
    http_req.set_method("POST");
    http_req.set_request_target("/sample_data/cgi-bin/cgi_test.py");
    http_req.set_request_body(body);
    http_req.set_content_length(body.size());

    ServerLocation sl;

    sl.set_alias("./../../..");

    CGIExecutor cgi;

    HTTPResponse *http_resp = cgi.Exec(http_req, sl);

    delete http_resp;

    std::ifstream ifs("/var/www/html/hoge.file");
    std::stringstream ss;

    ss << ifs.rdbuf();

    ASSERT_EQ(ss.str(), body);

    unlink("/var/www/html/hoge.file");

    ASSERT_EQ(http_resp->status_code(), 201);
}

TEST_F(CGITest, delete_cgi) {
    /*
        DELETE /sample_data/cgi-bin/cgi_test.py HTTP/1.1
    */
    system("mkdir -p /var/www/html");
    system("touch /var/www/html/hoge.file");

    HTTPRequest http_req;

    std::string body = "hello world\n";
    http_req.set_method("DELETE");
    http_req.set_request_target("/sample_data/cgi-bin/cgi_test.py");

    ServerLocation sl;

    sl.set_alias("./../../..");

    CGIExecutor cgi;

    HTTPResponse *http_resp = cgi.Exec(http_req, sl);

    delete http_resp;

    ASSERT_EQ(access("/var/www/html/hoge.file", F_OK) == 0, false);
}

TEST_F(CGITest, error_parse_empty_cgi_response) {
    std::string resp = "";

    ASSERT_THROW(CGIResponse::Parse(resp), HTTPException);
}

TEST_F(CGITest, error_parse_no_header_cgi_response) {
    std::string resp = "\nhello world";

    ASSERT_THROW(CGIResponse::Parse(resp), HTTPException);
}

TEST_F(CGITest, error_parse_no_empty_line_cgi_response) {
    std::string resp = "Status:200 OK\nhello world";

    ASSERT_THROW(CGIResponse::Parse(resp), HTTPException);
}

TEST_F(CGITest, error_parse_invalid_status_code_minus_cgi_response) {
    std::string resp = "Status: -1 OK\nhello world";

    ASSERT_THROW(CGIResponse::Parse(resp), HTTPException);
}

TEST_F(CGITest, error_parse_invalid_status_code_mix_cgi_response) {
    std::string resp = "Status: 100a OK\nhello world";

    ASSERT_THROW(CGIResponse::Parse(resp), HTTPException);
}

TEST_F(CGITest, error_parse_invalid_status_code_big_cgi_response) {
    std::string resp = "Status: 1000 OK\nhello world";

    ASSERT_THROW(CGIResponse::Parse(resp), HTTPException);
}
