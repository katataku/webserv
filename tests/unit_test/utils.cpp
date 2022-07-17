#include "utils.hpp"

#include <gtest/gtest.h>

TEST(Split, basic) {
    std::string str = "first\r\nsecond\r\nthird\r\n";
    std::string delim = "\r\n";

    std::vector<std::string> strs = Split(str, delim);
    ASSERT_EQ(3, strs.size());
    std::string h = strs[0];
    ASSERT_EQ(std::string("first"), strs[0]);
    ASSERT_EQ(std::string("second"), strs[1]);
    ASSERT_EQ(std::string("third"), strs[2]);
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
