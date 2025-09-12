#include "gtest/gtest.h"
#include "rcn_c/slhash.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_c::slnode node_;
    int value_;
};

class SLHashTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        table_ = new rcn_c::slhash;
        slhash_init(table_, sz_bucket_, _KeyHash, _ValueCompare, nullptr);
        __slhash_alloc_bucket(table_);
    }

    void TearDown() override
    {
        __slhash_free_bucket(table_);
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
    rcn_c::slhash *table_;
};

TEST_F(SLHashTest, Init)
{
    const size_t sz_bucket = sz_bucket_;

    ASSERT_EQ(slhash_buckets(table_), sz_bucket);
    ASSERT_EQ(slhash_size(table_), 0);
}

TEST_F(SLHashTest, Find)
{
    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slhash_insert(table_, &data1->node_, data1);
    slhash_insert(table_, &data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(slhash_find(table_, key), &data1->node_);
    key->value_ = 20;
    ASSERT_EQ(slhash_find(table_, key), &data2->node_);

    delete data1;
    delete data2;
}

TEST_F(SLHashTest, InsertAndAt)
{
    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slhash_insert(table_, &data1->node_, data1);
    slhash_insert(table_, &data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(slhash_at(table_, key), data1);
    key->value_ = 20;
    ASSERT_EQ(slhash_at(table_, key), data2);

    delete key;
    delete data1;
    delete data2;
}

TEST_F(SLHashTest, EmptyAndSize)
{
    ASSERT_TRUE(slhash_empty(table_));
    ASSERT_EQ(slhash_size(table_), 0);

    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slhash_insert(table_, &data1->node_, data1);
    slhash_insert(table_, &data2->node_, data2);

    ASSERT_FALSE(slhash_empty(table_));
    ASSERT_EQ(slhash_size(table_), 2);

    delete data1;
    delete data2;
}

TEST_F(SLHashTest, Clear)
{
    ASSERT_TRUE(slhash_empty(table_));
    ASSERT_EQ(slhash_size(table_), 0);

    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slhash_insert(table_, &data1->node_, data1);
    slhash_insert(table_, &data2->node_, data2);

    slhash_clear(table_);

    ASSERT_TRUE(slhash_empty(table_));
    ASSERT_EQ(slhash_size(table_), 0);

    delete data1;
    delete data2;
}

TEST_F(SLHashTest, Count)
{
    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slhash_insert(table_, &data1->node_, data1);
    slhash_insert(table_, &data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(slhash_size(table_, key), 1);
    key->value_ = 20;
    ASSERT_EQ(slhash_size(table_, key), 1);
}

TEST_F(SLHashTest, Swap)
{
    rcn_c::slhash other_table_;

    slhash_init(&other_table_, 23, _KeyHash, _ValueCompare, nullptr);
    __slhash_alloc_bucket(&other_table_);

    auto data1 = new TestData(10);
    auto data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slhash_insert(table_, &data1->node_, data1);
    slhash_insert(&other_table_, &data2->node_, data2);

    ASSERT_EQ(slhash_size(table_), 1);
    ASSERT_EQ(slhash_size(&other_table_), 1);

    slhash_swap(table_, &other_table_);

    ASSERT_EQ(slhash_size(table_), 1);
    ASSERT_EQ(slhash_size(&other_table_), 1);
    auto key = new TestData(20);
    ASSERT_EQ(slhash_at(table_, key), data2);
    key->value_ = 10;
    ASSERT_EQ(slhash_at(&other_table_, key), data1);

    delete data1;
    delete data2;
    __slhash_alloc_bucket(&other_table_);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
