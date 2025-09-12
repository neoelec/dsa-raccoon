#include "gtest/gtest.h"
#include "rcn_c/dlhash.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_c::dlnode node_;
    int value_;
};

class DLHashTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        table_ = new rcn_c::dlhash;
        dlhash_init(table_, sz_bucket_, _KeyHash, _ValueCompare, nullptr);
        __dlhash_alloc_bucket(table_);
    }

    void TearDown() override
    {
        for (size_t i = 0; i < dlhash_buckets(table_); i++) {
            while (dlhash_begin(table_, i) != dlhash_end(table_, i)) {
                rcn_c::dlnode *x = dlhash_begin(table_, i);
                dlhash_erase(table_, x);
                delete (TestData *)x;
            }
        }

        __dlhash_free_bucket(table_);
        delete table_;
    }

    static size_t _KeyHash(const void *_ke)
    {
        auto ke = (const TestData *)_ke;

        return ke->value_;
    }

    static int _ValueCompare(const void *_ke, const void *_in_table)
    {
        auto ke = (const TestData *)_ke;
        auto in_table = (const TestData *)_in_table;

        if (ke->value_ < in_table->value_) {
            return -1;
        }

        if (ke->value_ > in_table->value_) {
            return 1;
        }

        return 0;
    }

    const size_t sz_bucket_ = 53;
    rcn_c::dlhash *table_;
};

TEST_F(DLHashTest, Init)
{
    const size_t sz_bucket = sz_bucket_;

    ASSERT_EQ(dlhash_buckets(table_), sz_bucket);
    ASSERT_EQ(dlhash_size(table_), 0);
}

TEST_F(DLHashTest, Find)
{
    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlhash_insert(table_, &data1->node_, data1);
    dlhash_insert(table_, &data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(dlhash_find(table_, key), &data1->node_);
    key->value_ = 20;
    ASSERT_EQ(dlhash_find(table_, key), &data2->node_);

    delete key;
}

TEST_F(DLHashTest, InsertAndAt)
{
    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlhash_insert(table_, &data1->node_, data1);
    dlhash_insert(table_, &data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(dlhash_at(table_, key), data1);
    key->value_ = 20;
    ASSERT_EQ(dlhash_at(table_, key), data2);

    delete key;
}

TEST_F(DLHashTest, Erase)
{
    auto data = new TestData(10);

    ASSERT_NE(data, nullptr);

    dlhash_insert(table_, &data->node_, data);
    auto key = new TestData(10);
    ASSERT_NE(dlhash_at(table_, key), nullptr);

    dlhash_erase(table_, &data->node_);
    ASSERT_EQ(dlhash_at(table_, key), nullptr);

    delete key;
}

TEST_F(DLHashTest, Remove)
{
    auto data = new TestData(10);

    ASSERT_NE(data, nullptr);

    dlhash_insert(table_, &data->node_, data);
    auto key = new TestData(10);
    ASSERT_NE(dlhash_at(table_, key), nullptr);

    dlhash_remove(table_, key);
    ASSERT_EQ(dlhash_at(table_, key), nullptr);

    delete key;
}

TEST_F(DLHashTest, EmptyAndSize)
{
    ASSERT_TRUE(dlhash_empty(table_));
    ASSERT_EQ(dlhash_size(table_), 0);

    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlhash_insert(table_, &data1->node_, data1);
    dlhash_insert(table_, &data2->node_, data2);

    ASSERT_FALSE(dlhash_empty(table_));
    ASSERT_EQ(dlhash_size(table_), 2);
}

TEST_F(DLHashTest, Clear)
{
    ASSERT_TRUE(dlhash_empty(table_));
    ASSERT_EQ(dlhash_size(table_), 0);

    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlhash_insert(table_, &data1->node_, data1);
    dlhash_insert(table_, &data2->node_, data2);

    dlhash_clear(table_);

    ASSERT_TRUE(dlhash_empty(table_));
    ASSERT_EQ(dlhash_size(table_), 0);

    delete data1;
    delete data2;
}

TEST_F(DLHashTest, Count)
{
    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlhash_insert(table_, &data1->node_, data1);
    dlhash_insert(table_, &data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(dlhash_size(table_, key), 1);
    key->value_ = 20;
    ASSERT_EQ(dlhash_size(table_, key), 1);

    delete key;
}

TEST_F(DLHashTest, Swap)
{
    rcn_c::dlhash other_table_;

    dlhash_init(&other_table_, 23, _KeyHash, _ValueCompare, nullptr);
    __dlhash_alloc_bucket(&other_table_);

    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlhash_insert(table_, &data1->node_, data1);
    dlhash_insert(&other_table_, &data2->node_, data2);

    ASSERT_EQ(dlhash_size(table_), 1);
    ASSERT_EQ(dlhash_size(&other_table_), 1);

    dlhash_swap(table_, &other_table_);

    ASSERT_EQ(dlhash_size(table_), 1);
    ASSERT_EQ(dlhash_size(&other_table_), 1);
    auto key = new TestData(20);
    ASSERT_EQ(dlhash_at(table_, key), data2);
    key->value_ = 10;
    ASSERT_EQ(dlhash_at(&other_table_, key), data1);

    delete data1;
    __dlhash_alloc_bucket(&other_table_);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
