#include <algorithm>
#include <cstring>

#include "gtest/gtest.h"
#include "rcn_c/heap_sort.h"

#define NR_ELEM(a) (sizeof(a) / sizeof(a[0]))

int IntCompar(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

TEST(HeapSortTest, EmptyArray)
{
    int arr[0];

    rcn_c::heap_sort(arr, NR_ELEM(arr), sizeof(arr[0]), IntCompar);
    ASSERT_EQ(NR_ELEM(arr), 0);
}

TEST(HeapSortTest, SingleElementArray)
{
    int arr[] = { 5 };

    rcn_c::heap_sort(arr, NR_ELEM(arr), sizeof(arr[0]), IntCompar);
    ASSERT_EQ(NR_ELEM(arr), 1);
    ASSERT_EQ(arr[0], 5);
}

TEST(HeapSortTest, AlreadySortedArray)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int expected[NR_ELEM(arr)];

    std::memcpy(expected, arr, sizeof(arr));
    rcn_c::heap_sort(arr, NR_ELEM(arr), sizeof(arr[0]), IntCompar);
    EXPECT_TRUE(0 == std::memcmp(arr, expected, sizeof(expected)));
}

TEST(HeapSortTest, ReverseSortedArray)
{
    int arr[] = { 5, 4, 3, 2, 1 };
    int expected[] = { 1, 2, 3, 4, 5 };

    rcn_c::heap_sort(arr, NR_ELEM(arr), sizeof(arr[0]), IntCompar);
    EXPECT_TRUE(0 == std::memcmp(arr, expected, sizeof(expected)));
}

TEST(HeapSortTest, RandomArray)
{
    int arr[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
    int expected[NR_ELEM(arr)];

    std::memcpy(expected, arr, sizeof(arr));
    std::sort(expected, expected + NR_ELEM(expected));
    rcn_c::heap_sort(arr, NR_ELEM(arr), sizeof(arr[0]), IntCompar);
    EXPECT_TRUE(0 == std::memcmp(arr, expected, sizeof(expected)));
}

TEST(HeapSortTest, DuplicateElements)
{
    int arr[] = { 5, 2, 1, 2, 5, 3, 1 };
    int expected[NR_ELEM(arr)];

    std::memcpy(expected, arr, sizeof(arr));
    std::sort(expected, expected + NR_ELEM(expected));
    rcn_c::heap_sort(arr, NR_ELEM(arr), sizeof(arr[0]), IntCompar);
    EXPECT_TRUE(0 == std::memcmp(arr, expected, sizeof(expected)));
}

TEST(HeapSortTest, LargeArray)
{
    int arr[1000];

    for (size_t i = 0; i < NR_ELEM(arr); ++i) {
        arr[i] = rand() % 100;
    }

    int expected[NR_ELEM(arr)];
    std::memcpy(expected, arr, sizeof(arr));
    std::sort(expected, expected + NR_ELEM(expected));
    rcn_c::heap_sort(arr, NR_ELEM(arr), sizeof(arr[0]), IntCompar);
    EXPECT_TRUE(0 == std::memcmp(arr, expected, sizeof(expected)));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
