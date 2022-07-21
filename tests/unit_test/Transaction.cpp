#include "Transaction.hpp"

#include <gtest/gtest.h>

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "ResponseBuilder.hpp"
#include "ServerLocation.hpp"

class TransactionTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TransactionTest, FileReadExecutor) {
    system("mkdir -p /var/www/html");
    system("echo 'hello world' > /var/www/html/hello_world.html");

    HTTPRequest req = HTTPRequest();
    req.set_method("GET");
    req.set_request_target("/html/hello_world.html");

    std::map<int, std::string> error_pages;
    std::set<std::string> allow_methods;
    allow_methods.insert("GET");

    ServerLocation sl =
        ServerLocation(8081, "webserv1", "/html", error_pages, 4086, false,
                       "index.html", "", allow_methods, "/var/www", "");

    Transaction tr;
    HTTPResponse *res = tr.Exec(&req, &sl);
    ASSERT_EQ(res->response_body(), "hello world\n");
    unlink("/var/www/html/hello_world.html");
}

TEST_F(TransactionTest, ListDirectoryExecutor) {
    system("mkdir -p /var/www/html");
    system("echo 'hello world' > /var/www/html/hello_world.html");

    HTTPRequest req = HTTPRequest();
    req.set_method("GET");
    req.set_request_target("/html?query=value");

    std::map<int, std::string> error_pages;
    std::set<std::string> allow_methods;
    allow_methods.insert("GET");
    ServerLocation sl =
        ServerLocation(8081, "webserv1", "/html", error_pages, 4086, false,
                       "index.html", "", allow_methods, "/var/www", "");

    Transaction tr;
    HTTPResponse *res = tr.Exec(&req, &sl);
    ASSERT_NE(res->response_body().find("<h1>Index of /html</h1>"),
              std::string::npos);
    ASSERT_NE(res->response_body().find(" hello_world.html</a>"),
              std::string::npos);
    ASSERT_NE(res->response_body().find(" ./</a>"), std::string::npos);
    ASSERT_NE(res->response_body().find(" ../</a>"), std::string::npos);
    unlink("/var/www/html/hello_world.html");
}

TEST_F(TransactionTest, Allowed_methods) {
    HTTPRequest req = HTTPRequest();
    req.set_method("HOGE");

    std::map<int, std::string> error_pages;
    std::set<std::string> allow_methods;
    ServerLocation sl =
        ServerLocation(8081, "webserv1", "/html", error_pages, 4086, false,
                       "index.html", "", allow_methods, "/var/www", "");

    Transaction tr;
    HTTPResponse *res = tr.Exec(&req, &sl);
    ASSERT_EQ(res->status_code(), 403);
}

TEST_F(TransactionTest, client_max_body_size) {
    HTTPRequest req = HTTPRequest();
    std::string expected =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Length: 4\r\n"
        "\r\n"
        "hoge";

    std::map<int, std::string> error_pages;
    std::set<std::string> allow_methods;
    allow_methods.insert("GET");
    ServerLocation sl =
        ServerLocation(8081, "webserv1", "/html", error_pages, 1, false,
                       "index.html", "", allow_methods, "/var/www", "");

    Transaction tr;
    HTTPResponse *res = tr.Exec(&req, &sl);
    ASSERT_EQ(res->status_code(), 403);
}
