#include "gtest/gtest.h"
#include "rcn_c/slist.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }
    rcn_c::slnode node_;
    int value_;
};

class SListTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        list_ = new rcn_c::slist;
        slist_init(list_);
    }

    void TearDown() override
    {
        while (!slist_empty(list_)) {
            delete (TestData *)slist_pop_front(list_);
        }
        delete list_;
    }

    rcn_c::slist *list_;
};

TEST_F(SListTest, Init)
{
    ASSERT_TRUE(slist_empty(list_));
    ASSERT_EQ(slist_size(list_), 0);
}

TEST_F(SListTest, PushFront)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    auto front = (TestData *)slist_front(list_);
    ASSERT_EQ(front->value_, 10);
    ASSERT_EQ(slist_size(list_), 1);

    slist_push_front(list_, &data2->node_, data2);
    front = (TestData *)slist_front(list_);
    ASSERT_EQ(front->value_, 20);
    ASSERT_EQ(slist_size(list_), 2);
}

TEST_F(SListTest, PopFront)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    slist_push_front(list_, &data2->node_, data2);

    auto popped_front = (TestData *)slist_pop_front(list_);
    ASSERT_EQ(popped_front->value_, 20);
    delete (TestData *)popped_front;
    ASSERT_EQ(slist_size(list_), 1);

    popped_front = (TestData *)slist_pop_front(list_);
    ASSERT_EQ(popped_front->value_, 10);
    delete popped_front;
    ASSERT_TRUE(slist_empty(list_));
}

TEST_F(SListTest, At)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    slist_push_front(list_, &data2->node_, data2);
    slist_push_front(list_, &data3->node_, data3);

    ASSERT_EQ(((TestData *)slist_at(list_, 0))->value_, 30);
    ASSERT_EQ(((TestData *)slist_at(list_, 1))->value_, 20);
    ASSERT_EQ(((TestData *)slist_at(list_, 2))->value_, 10);
    ASSERT_EQ(slist_at(list_, 3), nullptr);
}

TEST_F(SListTest, EraseAfter)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    slist_push_front(list_, &data2->node_, data2);
    slist_push_front(list_, &data3->node_, data3);

    auto begin = slist_begin(list_);
    auto first_data = (TestData *)slist_erase_after(begin);
    ASSERT_EQ(first_data->value_, 20);
    delete first_data;
    ASSERT_EQ(((TestData *)slist_at(list_, 0))->value_, 30);
    ASSERT_EQ(((TestData *)slist_at(list_, 1))->value_, 10);
}

TEST_F(SListTest, SizeAndEmpty)
{
    ASSERT_TRUE(slist_empty(list_));
    ASSERT_EQ(slist_size(list_), 0);

    auto *data = new TestData(10);

    ASSERT_NE(data, nullptr);

    slist_push_front(list_, &data->node_, data);

    ASSERT_FALSE(slist_empty(list_));
    ASSERT_EQ(slist_size(list_), 1);
}

int TestDataCompare(const void *_ke, const void *_in_list)
{
    auto ke = (const TestData *)_ke;
    auto *in_list = (const TestData *)_in_list;
    if (ke->value_ < in_list->value_)
        return -1;
    if (ke->value_ > in_list->value_)
        return 1;
    return 0;
}

TEST_F(SListTest, RemoveIf)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    slist_push_front(list_, &data2->node_, data2);
    slist_push_front(list_, &data3->node_, data3);

    auto *key = new TestData(20);
    auto removed_node = slist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(((TestData *)removed_node->entry_)->value_, 20);
    ASSERT_NE(removed_node, nullptr);
    delete (TestData *)removed_node->entry_;

    ASSERT_EQ(slist_size(list_), 2);
    ASSERT_EQ(((TestData *)slist_at(list_, 0))->value_, 30);
    ASSERT_EQ(((TestData *)slist_at(list_, 1))->value_, 10);

    key->value_ = 30;
    removed_node = slist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(((TestData *)removed_node->entry_)->value_, 30);
    delete (TestData *)removed_node->entry_;
    ASSERT_EQ(slist_size(list_), 1);
    ASSERT_EQ(((TestData *)slist_at(list_, 0))->value_, 10);

    key->value_ = 10;
    removed_node = slist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(((TestData *)removed_node->entry_)->value_, 10);
    delete (TestData *)removed_node->entry_;
    ASSERT_TRUE(slist_empty(list_));

    key->value_ = 50;
    removed_node = slist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(removed_node, nullptr);
}

TEST_F(SListTest, Reverse)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    slist_push_front(list_, &data2->node_, data2);
    slist_push_front(list_, &data3->node_, data3);

    slist_reverse(list_);

    ASSERT_EQ(((TestData *)slist_at(list_, 0))->value_, 10);
    ASSERT_EQ(((TestData *)slist_at(list_, 1))->value_, 20);
    ASSERT_EQ(((TestData *)slist_at(list_, 2))->value_, 30);
}

TEST_F(SListTest, Swap)
{
    rcn_c::slist other_list;
    slist_init(&other_list);

    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);
    auto *data4 = new TestData(40);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);
    ASSERT_NE(data4, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    slist_push_front(list_, &data2->node_, data2);
    slist_push_front(&other_list, &data3->node_, data3);
    slist_push_front(&other_list, &data4->node_, data4);

    slist_swap(list_, &other_list);

    ASSERT_EQ(slist_size(list_), 2);
    ASSERT_EQ(slist_size(&other_list), 2);

    ASSERT_EQ(((TestData *)slist_at(list_, 0))->value_, 40);
    ASSERT_EQ(((TestData *)slist_at(list_, 1))->value_, 30);

    ASSERT_EQ(((TestData *)slist_at(&other_list, 0))->value_, 20);
    ASSERT_EQ(((TestData *)slist_at(&other_list, 1))->value_, 10);

    while (!slist_empty(&other_list)) {
        delete (TestData *)slist_pop_front(&other_list);
    }
}

TEST_F(SListTest, Clear)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    slist_push_front(list_, &data1->node_, data1);
    slist_push_front(list_, &data2->node_, data2);

    slist_clear(list_);
    ASSERT_TRUE(slist_empty(list_));
    ASSERT_EQ(slist_size(list_), 0);

    delete data1;
    delete data2;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
