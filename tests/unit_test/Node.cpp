#include "Node.hpp"

#include <gtest/gtest.h>

#include <list>
#include <string>

class NodeTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST(NodeTest, basic) {
    Node node;
    std::list<std::string> vals;
    val.push_back("/var/www/html/");
    node.set_directive_vals(vals);

    node.ValidateAliasValue();
}
