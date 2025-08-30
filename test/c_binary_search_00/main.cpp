#include "gtest/gtest.h"
#include "rcn_c/binary_search.h"
#include "rcn_c/common.h"

int IntCompar(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

TEST(BinarySearchTest, EmptyArray)
{
    int key = 5;
    int arr[] = {};
    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);

    ASSERT_EQ(nullptr, result);
}

TEST(BinarySearchTest, SingleElementFound)
{
    int key = 5;
    int arr[] = { 5 };
    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);

    ASSERT_NE(nullptr, result);
    ASSERT_EQ(key, *(int *)result);
}

TEST(BinarySearchTest, SingleElementNotFound)
{
    int key = 5;
    int arr[] = { 10 };
    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);

    ASSERT_EQ(nullptr, result);
}

TEST(BinarySearchTest, MultipleElementsFound)
{
    int key = 5;
    int arr[] = { 1, 3, 5, 7, 9 };
    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);

    ASSERT_NE(nullptr, result);
    ASSERT_EQ(key, *(int *)result);
}

TEST(BinarySearchTest, MultipleElementsNotFound)
{
    int key = 6;
    int arr[] = { 1, 3, 5, 7, 9 };
    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);

    ASSERT_EQ(nullptr, result);
}

TEST(BinarySearchTest, MultipleElementsFoundFirst)
{
    int key = 1;
    int arr[] = { 1, 3, 5, 7, 9 };
    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);

    ASSERT_NE(nullptr, result);
    ASSERT_EQ(key, *(int *)result);
}

TEST(BinarySearchTest, MultipleElementsFoundLast)
{
    int key = 9;
    int arr[] = { 1, 3, 5, 7, 9 };
    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);

    ASSERT_NE(nullptr, result);
    ASSERT_EQ(key, *(int *)result);
}

TEST(BinarySearchTest, LargeArrayFound)
{
    int key = 500;
    int arr[1000];

    for (size_t i = 0; i < NR_ELEM(arr); ++i) {
        arr[i] = i;
    }

    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);
    ASSERT_NE(nullptr, result);
    ASSERT_EQ(key, *(int *)result);
}

TEST(BinarySearchTest, LargeArrayNotFound)
{
    int key = 1000;
    int arr[1000];

    for (size_t i = 0; i < NR_ELEM(arr); ++i) {
        arr[i] = i;
    }

    void *result = rcn_c::binary_search(&key, arr, NR_ELEM(arr), sizeof(arr[0]),
                                        IntCompar);
    ASSERT_EQ(nullptr, result);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
