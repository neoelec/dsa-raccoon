#include <cstring>
#include <vector>

#include "gtest/gtest.h"
#include "rcn_cpp/eratosthenes.h"

typedef unsigned int uint;

TEST(eratosthenesTest, BasicTest)
{
    bool expected[] = { false, false, true,  true,  false, true,  false, true,
                        false, false, false, true,  false, true,  false, false,
                        false, true,  false, true,  false, false, false, true,
                        false, false, false, false, false, true };
    bool *sieve = rcn_cpp::eratosthenes<uint>(29);

    EXPECT_TRUE(0 == std::memcmp(sieve, expected, sizeof(expected)));
    delete sieve;
}

TEST(eratosthenesTest, EmptyTest)
{
    bool expected[] = { false, false };
    bool *sieve = rcn_cpp::eratosthenes<uint>(1);

    EXPECT_TRUE(0 == std::memcmp(sieve, expected, sizeof(expected)));
    delete sieve;
}

TEST(eratosthenesTest, LargerNumberTest)
{
    bool *sieve = rcn_cpp::eratosthenes<uint>(100);

    ASSERT_EQ(sieve[2], true);
    ASSERT_EQ(sieve[3], true);
    ASSERT_EQ(sieve[5], true);
    ASSERT_EQ(sieve[7], true);
    ASSERT_EQ(sieve[97], true);
    ASSERT_EQ(sieve[4], false);
    ASSERT_EQ(sieve[6], false);
    ASSERT_EQ(sieve[10], false);
    ASSERT_EQ(sieve[100], false);
    delete sieve;
}

TEST(eratosthenesTest, CheckPrimesUpto100)
{
    std::vector<uint> expected_primes = { 2,  3,  5,  7,  11, 13, 17, 19, 23,
                                          29, 31, 37, 41, 43, 47, 53, 59, 61,
                                          67, 71, 73, 79, 83, 89, 97 };
    bool *sieve = rcn_cpp::eratosthenes<uint>(100);
    std::vector<uint> actual_primes;

    for (size_t i = 2; i <= 100; ++i) {
        if (sieve[i]) {
            actual_primes.push_back(i);
        }
    }

    ASSERT_EQ(expected_primes, actual_primes);
    delete sieve;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
