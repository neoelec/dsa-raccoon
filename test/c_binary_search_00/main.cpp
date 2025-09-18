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

TEST(BinarySearchTest, BinaryInsertAndDelete)
{
    int key = 4;
    int arr[6] = { 1, 3, 5, 7, 9 };
    size_t nmemb = 5;
    ssize_t pos;

    pos = rcn_c::binary_insert(&key, arr, &nmemb, sizeof(arr[0]), IntCompar);
    ASSERT_EQ(2, pos);
    ASSERT_EQ(key, arr[pos]);
    ASSERT_EQ(5, arr[pos + 1]);
    ASSERT_EQ(7, arr[pos + 2]);
    ASSERT_EQ(9, arr[pos + 3]);
    ASSERT_EQ(6, nmemb);

    pos = rcn_c::binary_delete(&key, arr, &nmemb, sizeof(arr[0]), IntCompar);
    ASSERT_EQ(2, pos);
    ASSERT_EQ(5, arr[pos]);
    ASSERT_EQ(7, arr[pos + 1]);
    ASSERT_EQ(9, arr[pos + 2]);
    ASSERT_EQ(5, nmemb);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
