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
TEST(IsInteger, valid) {
    ASSERT_EQ(true, IsInteger("0"));
    ASSERT_EQ(true, IsInteger("42"));
    ASSERT_EQ(true, IsInteger("-42"));
    ASSERT_EQ(true, IsInteger("+42"));
    ASSERT_EQ(true, IsInteger("2147483647"));
    ASSERT_EQ(true, IsInteger("-2147483648"));
}

TEST(IsInteger, invalid) {
    ASSERT_EQ(false, IsInteger(""));
    ASSERT_EQ(false, IsInteger("42a"));
    ASSERT_EQ(false, IsInteger("a42"));
    ASSERT_EQ(false, IsInteger("4a2"));
    ASSERT_EQ(false, IsInteger("++42"));
    ASSERT_EQ(false, IsInteger("2147483648"));
    ASSERT_EQ(false, IsInteger("-2147483649"));
    ASSERT_EQ(false, IsInteger("9223372036854775807"));
}

TEST(ToInteger, normal) {
    ASSERT_EQ(0, ToInteger("0"));
    ASSERT_EQ(42, ToInteger("42"));
    ASSERT_EQ(-42, ToInteger("-42"));
    ASSERT_EQ(42, ToInteger("+42"));
    ASSERT_EQ(2147483647, ToInteger("2147483647"));
    ASSERT_EQ(-2147483648, ToInteger("-2147483648"));
}

TEST(Trim, normal) {
    std::string str = " \t42 \t";
    std::string chars = " \t";

    ASSERT_EQ("42 \t", LeftTrim(str, chars));
    ASSERT_EQ(" \t42", RightTrim(str, chars));
    ASSERT_EQ("42", Trim(str, chars));
}

TEST(ToUpper, normal) {
    std::string str = "Content-Length";
    ASSERT_EQ("CONTENT-LENGTH", ToUpper(str));
}

TEST(ToLower, normal) {
    std::string str = "Content-Length";
    ASSERT_EQ("content-length", ToLower(str));
}
