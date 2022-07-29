#include "utils.hpp"

#include <gtest/gtest.h>

TEST(Split, basic) {
    std::string str = "first\r\nsecond\r\nthird\r\n";
    std::string delim = "\r\n";

    std::vector<std::string> strs = Split(str, delim);
    ASSERT_EQ(4, strs.size());
    ASSERT_EQ(std::string("first"), strs[0]);
    ASSERT_EQ(std::string("second"), strs[1]);
    ASSERT_EQ(std::string("third"), strs[2]);
    ASSERT_EQ(std::string(""), strs[3]);
}

TEST(Split, no_delim_in_the_end) {
    std::string str = "first\r\nsecond\r\nthird";
    std::string delim = "\r\n";

    std::vector<std::string> strs = Split(str, delim);
    ASSERT_EQ(3, strs.size());
    ASSERT_EQ(std::string("first"), strs[0]);
    ASSERT_EQ(std::string("second"), strs[1]);
    ASSERT_EQ(std::string("third"), strs[2]);
}

TEST(Split, only_delimiter) {
    std::string str = "/";
    std::string delim = "/";

    std::vector<std::string> strs = Split(str, delim);
    ASSERT_EQ(2, strs.size());
    ASSERT_EQ(std::string(""), strs[0]);
    ASSERT_EQ(std::string(""), strs[1]);
}

TEST(Split, no_delimiter) {
    std::string str = "no_delimiter";
    std::string delim = "/";

    std::vector<std::string> strs = Split(str, delim);
    ASSERT_EQ(1, strs.size());
    ASSERT_EQ(std::string("no_delimiter"), strs[0]);
}

TEST(Join, basic) {
    std::vector<std::string> strs;
    strs.push_back("var");
    strs.push_back("www");
    strs.push_back("html");

    ASSERT_EQ("var/www/html", Join(strs, "/"));
}
