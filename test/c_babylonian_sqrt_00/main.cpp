#include <cmath>

#include "gtest/gtest.h"
#include "rcn_c/babylonian_sqrt.h"

TEST(BabylonianSqrtTest, PositiveNumber)
{
    double S = 25.0;
    double tolerance = 0.0001;
    double expected = 5.0;
    double result = rcn_c::babylonian_sqrt(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, Zero)
{
    double S = 0.0;
    double tolerance = 0.0001;
    double expected = 0.0;
    double result = rcn_c::babylonian_sqrt(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, NegativeNumber)
{
    double S = -25.0;
    double tolerance = 0.0001;
    double expected = 0.0;
    double result = rcn_c::babylonian_sqrt(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, LargeNumber)
{
    double S = 1000000.0;
    double tolerance = 0.0001;
    double expected = 1000.0;
    double result = rcn_c::babylonian_sqrt(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, SmallNumber)
{
    double S = 0.0001;
    double tolerance = 0.0001;
    double expected = 0.01;
    double result = rcn_c::babylonian_sqrt(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, DifferentTolerance)
{
    double S = 25.0;
    double tolerance = 0.000001;
    double expected = 5.0;
    double result = rcn_c::babylonian_sqrt(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, RandomNumbers)
{
    srand(time(0));

    for (size_t i = 0; i < 100; ++i) {
        double S = static_cast<double>(rand()) / RAND_MAX * 1000.0;
        double tolerance = 0.0001;
        double expected = sqrt(S);
        double result = rcn_c::babylonian_sqrt(S, tolerance);
        ASSERT_NEAR(expected, result, tolerance);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
