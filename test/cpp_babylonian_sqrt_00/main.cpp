#include <cmath>

#include "gtest/gtest.h"
#include "rcn_cpp/babylonian_sqrt.h"

TEST(BabylonianSqrtTest, PositiveNumber)
{
    float S = 25.0;
    float tolerance = 0.0001;
    float expected = 5.0;
    float result = rcn_cpp::babylonian_sqrt<float>(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, Zero)
{
    float S = 0.0;
    float tolerance = 0.0001;
    float expected = 0.0;
    float result = rcn_cpp::babylonian_sqrt<float>(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, NegativeNumber)
{
    float S = -25.0;
    float tolerance = 0.0001;
    float expected = 0.0;
    float result = rcn_cpp::babylonian_sqrt<float>(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, LargeNumber)
{
    float S = 1000000.0;
    float tolerance = 0.0001;
    float expected = 1000.0;
    float result = rcn_cpp::babylonian_sqrt<float>(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, SmallNumber)
{
    float S = 0.0001;
    float tolerance = 0.0001;
    float expected = 0.01;
    float result = rcn_cpp::babylonian_sqrt<float>(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, DifferentTolerance)
{
    float S = 25.0;
    float tolerance = 0.000001;
    float expected = 5.0;
    float result = rcn_cpp::babylonian_sqrt<float>(S, tolerance);
    ASSERT_NEAR(expected, result, tolerance);
}

TEST(BabylonianSqrtTest, RandomNumbers)
{
    srand(time(0));

    for (size_t i = 0; i < 100; ++i) {
        float S = static_cast<float>(rand()) / RAND_MAX * 1000.0;
        float tolerance = 0.0001;
        float expected = sqrtf(S);
        float result = rcn_cpp::babylonian_sqrt<float>(S, tolerance);
        ASSERT_NEAR(expected, result, tolerance);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
