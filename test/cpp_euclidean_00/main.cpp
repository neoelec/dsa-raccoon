#include "gtest/gtest.h"
#include "rcn_cpp/euclidean.h"

typedef unsigned long long ullong;

TEST(EuclideanGCDTest, PositiveNumbers)
{
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(10, 5), 5);
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(48, 18), 6);
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(1071, 462), 21);
}

TEST(EuclideanGCDTest, OneNumberIsZero)
{
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(10, 0), 10);
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(0, 10), 10);
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(0, 0), 0);
}

TEST(EuclideanGCDTest, LargeNumbers)
{
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(123456789, 987654321), 9);
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(18446744073709551615ULL, 3), 3);
}

TEST(EuclideanGCDTest, SameNumbers)
{
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(7, 7), 7);
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(100, 100), 100);
}

TEST(EuclideanGCDTest, OneIsMultipleOfTheOther)
{
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(25, 5), 5);
    EXPECT_EQ(rcn_cpp::euclidean_gcd<ullong>(144, 12), 12);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
