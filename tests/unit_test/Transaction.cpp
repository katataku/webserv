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
    req.method("GET");
    req.uri("/html/hello_world.html");

    std::map<int, std::string> error_pages;
    std::set<std::string> allow_methods;
    ServerLocation sl =
        ServerLocation(8081, "webserv1", "/html", error_pages, 4086, false,
                       "index.html", "", allow_methods, "/var/www", "");

    Transaction tr;
    HTTPResponse *res = tr.Exec(&req, &sl);
    ASSERT_EQ(res->response_body(), "hello world\n");
    unlink("/var/www/html/hello_world.html");
}