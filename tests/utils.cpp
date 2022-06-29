#include <gtest/gtest.h>

class UtilsTest : public ::testing::Test {
 protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(UtilsTest, atod) { ASSERT_DOUBLE_EQ(1.5, 1.5); }
