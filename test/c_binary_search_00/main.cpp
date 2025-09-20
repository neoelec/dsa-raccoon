#include "gtest/gtest.h"
#include "rcn_c/binary_search.h"
#include "rcn_c/common.h"
#include "rcn_c/linear_search.h"

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
    int *found;
    size_t nmemb = 5;
    int err;

    found = (int *)rcn_c::linear_insert(&key, arr, &nmemb, sizeof(arr[0]),
                                        IntCompar);
    ASSERT_EQ(key, found[0]);
    ASSERT_EQ(5, found[1]);
    ASSERT_EQ(7, found[2]);
    ASSERT_EQ(9, found[3]);
    ASSERT_EQ(6, nmemb);

    err = rcn_c::binary_delete(&key, arr, &nmemb, sizeof(arr[0]), IntCompar);
    ASSERT_EQ(0, err);
    ASSERT_EQ(5, arr[2]);
    ASSERT_EQ(7, arr[3]);
    ASSERT_EQ(9, arr[4]);
    ASSERT_EQ(5, nmemb);

    key = 2;
    err = rcn_c::binary_delete(&key, arr, &nmemb, sizeof(arr[0]), IntCompar);
    ASSERT_EQ(-EEXIST, err);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
