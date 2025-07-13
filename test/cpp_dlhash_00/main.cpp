#include "gtest/gtest.h"
#include "rcn_cpp/dlhash.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_cpp::dlnode<TestData> node_;
    int value_;
};

class DLHashTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        table_ =
            new rcn_cpp::dlhash<TestData>(sz_bucket_, _KeyHash, _ValueCompare);
        table_->alloc_bucket();
    }

    void TearDown() override
    {
        for (size_t i = 0; i < table_->buckets(); i++) {
            while (table_->begin(i) != table_->end(i)) {
                auto x = table_->begin(i);
                table_->erase(x);
                delete x->entry_;
            }
        }

        table_->free_bucket();
        delete table_;
    }

    static size_t _KeyHash(const TestData *ke)
    {
        return ke->value_;
    }

    static int _ValueCompare(const TestData *ke, const TestData *in_table)
    {
        if (ke->value_ < in_table->value_) {
            return -1;
        }

        if (ke->value_ > in_table->value_) {
            return 1;
        }

        return 0;
    }

    const size_t sz_bucket_ = 53;
    rcn_cpp::dlhash<TestData> *table_;
};

TEST_F(DLHashTest, Init)
{
    const size_t sz_bucket = sz_bucket_;

    ASSERT_EQ(table_->buckets(), sz_bucket);
    ASSERT_EQ(table_->size(), 0);
}

TEST_F(DLHashTest, Find)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    table_->insert(&data1->node_, data1);
    table_->insert(&data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(table_->find(key), &data1->node_);
    key->value_ = 20;
    ASSERT_EQ(table_->find(key), &data2->node_);
}

TEST_F(DLHashTest, InsertAndAt)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    table_->insert(&data1->node_, data1);
    table_->insert(&data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(table_->at(key), data1);
    key->value_ = 20;
    ASSERT_EQ(table_->at(key), data2);

    delete key;
}

TEST_F(DLHashTest, Erase)
{
    auto *data = new TestData(10);

    ASSERT_NE(data, nullptr);

    table_->insert(&data->node_, data);
    auto key = new TestData(10);
    ASSERT_NE(table_->at(key), nullptr);

    table_->erase(&data->node_);
    ASSERT_EQ(table_->at(key), nullptr);

    delete key;
}

TEST_F(DLHashTest, Remove)
{
    auto *data = new TestData(10);

    ASSERT_NE(data, nullptr);

    table_->insert(&data->node_, data);
    auto key = new TestData(10);
    ASSERT_NE(table_->at(key), nullptr);

    table_->remove(key);
    ASSERT_EQ(table_->at(key), nullptr);
    delete key;
}

TEST_F(DLHashTest, EmptyAndSize)
{
    ASSERT_TRUE(table_->empty());
    ASSERT_EQ(table_->size(), 0);

    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    table_->insert(&data1->node_, data1);
    table_->insert(&data2->node_, data2);

    ASSERT_FALSE(table_->empty());
    ASSERT_EQ(table_->size(), 2);
}

TEST_F(DLHashTest, Clear)
{
    ASSERT_TRUE(table_->empty());
    ASSERT_EQ(table_->size(), 0);

    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    table_->insert(&data1->node_, data1);
    table_->insert(&data2->node_, data2);

    table_->clear();

    ASSERT_TRUE(table_->empty());
    ASSERT_EQ(table_->size(), 0);

    delete data1;
    delete data2;
}

TEST_F(DLHashTest, Count)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    table_->insert(&data1->node_, data1);
    table_->insert(&data2->node_, data2);

    auto key = new TestData(10);
    ASSERT_EQ(table_->count(key), 1);
    key->value_ = 20;
    ASSERT_EQ(table_->count(key), 1);
}

TEST_F(DLHashTest, Swap)
{
    rcn_cpp::dlhash other_table(23, _KeyHash, _ValueCompare);

    other_table.alloc_bucket();

    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    table_->insert(&data1->node_, data1);
    other_table.insert(&data2->node_, data2);

    ASSERT_EQ(table_->size(), 1);
    ASSERT_EQ(other_table.size(), 1);

    table_->swap(&other_table);

    ASSERT_EQ(table_->size(), 1);
    ASSERT_EQ(other_table.size(), 1);
    auto key = new TestData(20);
    ASSERT_EQ(table_->at(key), data2);
    key->value_ = 10;
    ASSERT_EQ(other_table.at(key), data1);

    delete data1;
    other_table.alloc_bucket();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
