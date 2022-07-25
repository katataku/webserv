#include <gtest/gtest.h>

#include "CGIExecutor.hpp"
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
        GET /sample_data/cgi-bin/hello_cgi.py HTTP/1.1
    */
    HTTPRequest http_req;

    http_req.set_method("GET");
    http_req.set_request_target("/sample_data/cgi-bin/hello_cgi.py");

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
        POST /sample_data/cgi-bin/file_manager.py/hoge.file HTTP/1.1

        hello world
    */
    HTTPRequest http_req;

    http_req.set_method("POST");
    http_req.set_request_target(
        "/sample_data/cgi-bin/file_manager.py/hoge.file");
    http_req.set_request_body("hello world\n");

    ServerLocation sl;

    sl.set_alias("./../../..");

    CGIExecutor cgi;

    HTTPResponse *http_resp = cgi.Exec(http_req, sl);

    delete http_resp;

    // ASSERT_EQ(http_resp->status_code(), 200);
    // ASSERT_EQ(http_resp->response_body(), "hello from cgi\n");
}
