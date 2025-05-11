#include "gtest/gtest.h"
#include "rcn_c/euclidean.h"

TEST(EuclideanGCDTest, PositiveNumbers)
{
    EXPECT_EQ(rcn_c::euclidean_gcd(10, 5), 5);
    EXPECT_EQ(rcn_c::euclidean_gcd(48, 18), 6);
    EXPECT_EQ(rcn_c::euclidean_gcd(1071, 462), 21);
}

TEST(EuclideanGCDTest, OneNumberIsZero)
{
    EXPECT_EQ(rcn_c::euclidean_gcd(10, 0), 10);
    EXPECT_EQ(rcn_c::euclidean_gcd(0, 10), 10);
    EXPECT_EQ(rcn_c::euclidean_gcd(0, 0), 0);
}

TEST(EuclideanGCDTest, LargeNumbers)
{
    EXPECT_EQ(rcn_c::euclidean_gcd(123456789, 987654321), 9);
    EXPECT_EQ(rcn_c::euclidean_gcd(18446744073709551615ULL, 3), 3);
}

TEST(EuclideanGCDTest, SameNumbers)
{
    EXPECT_EQ(rcn_c::euclidean_gcd(7, 7), 7);
    EXPECT_EQ(rcn_c::euclidean_gcd(100, 100), 100);
}

TEST(EuclideanGCDTest, OneIsMultipleOfTheOther)
{
    EXPECT_EQ(rcn_c::euclidean_gcd(25, 5), 5);
    EXPECT_EQ(rcn_c::euclidean_gcd(144, 12), 12);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
