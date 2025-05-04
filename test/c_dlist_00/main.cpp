#include "gtest/gtest.h"
#include "rcn_c/dlist.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }
    rcn_c::dlnode node_;
    int value_;
};

class DListTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        list_ = new rcn_c::dlist;
        dlist_init(list_);
    }

    void TearDown() override
    {
        while (!dlist_empty(list_)) {
            delete (TestData *)dlist_pop_front(list_);
        }
        delete list_;
    }

    rcn_c::dlist *list_;
};

TEST_F(DListTest, Init)
{
    ASSERT_TRUE(dlist_empty(list_));
    ASSERT_EQ(dlist_size(list_), 0);
}

TEST_F(DListTest, PushFrontAndBack)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    dlist_push_front(list_, &data1->node_, data1);
    dlist_push_back(list_, &data2->node_, data2);
    dlist_push_front(list_, &data3->node_, data3);

    ASSERT_EQ(dlist_size(list_), 3);
    auto front = (TestData *)dlist_front(list_);
    ASSERT_EQ(front->value_, 30);
    auto back = (TestData *)dlist_back(list_);
    ASSERT_EQ(back->value_, 20);
}

TEST_F(DListTest, PopFrontAndBack)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlist_push_back(list_, &data1->node_, data1);
    dlist_push_back(list_, &data2->node_, data2);
    ASSERT_EQ(dlist_size(list_), 2);

    auto *popped_front = (TestData *)dlist_pop_front(list_);
    ASSERT_EQ(popped_front->value_, 10);
    delete popped_front;
    ASSERT_EQ(dlist_size(list_), 1);

    auto *popped_back = (TestData *)dlist_pop_back(list_);
    ASSERT_EQ(popped_back->value_, 20);
    delete popped_back;
    ASSERT_TRUE(dlist_empty(list_));
}

TEST_F(DListTest, At)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    dlist_push_front(list_, &data1->node_, data1);
    dlist_push_front(list_, &data2->node_, data2);
    dlist_push_front(list_, &data3->node_, data3);

    ASSERT_EQ(((TestData *)dlist_at(list_, 0))->value_, 30);
    ASSERT_EQ(((TestData *)dlist_at(list_, 1))->value_, 20);
    ASSERT_EQ(((TestData *)dlist_at(list_, 2))->value_, 10);
    ASSERT_EQ(dlist_at(list_, 3), nullptr);
}

TEST_F(DListTest, Erase)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    dlist_push_back(list_, &data1->node_, data1);
    dlist_push_back(list_, &data2->node_, data2);
    dlist_push_back(list_, &data3->node_, data3);

    ASSERT_EQ(dlist_size(list_), 3);

    auto second_node = dlist_begin(list_)->next_;
    auto erased_data = (TestData *)dlist_erase(second_node);

    ASSERT_EQ(erased_data->value_, 20);
    delete erased_data;

    ASSERT_EQ(dlist_size(list_), 2);
    auto idx_0_data = (TestData *)dlist_at(list_, 0);
    ASSERT_EQ(idx_0_data->value_, 10);
    auto idx_1_data = (TestData *)dlist_at(list_, 1);
    ASSERT_EQ(idx_1_data->value_, 30);
}

TEST_F(DListTest, SizeAndEmpty)
{
    ASSERT_TRUE(dlist_empty(list_));
    ASSERT_EQ(dlist_size(list_), 0);

    auto *data = new TestData(10);

    ASSERT_NE(data, nullptr);

    dlist_push_back(list_, &data->node_, data);

    ASSERT_FALSE(dlist_empty(list_));
    ASSERT_EQ(dlist_size(list_), 1);
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

TEST_F(DListTest, RemoveIf)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    dlist_push_front(list_, &data1->node_, data1);
    dlist_push_front(list_, &data2->node_, data2);
    dlist_push_front(list_, &data3->node_, data3);

    auto *key = new TestData(20);
    auto removed_node = dlist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(((TestData *)removed_node->entry_)->value_, 20);
    ASSERT_NE(removed_node, nullptr);
    delete (TestData *)removed_node->entry_;

    ASSERT_EQ(dlist_size(list_), 2);
    ASSERT_EQ(((TestData *)dlist_at(list_, 0))->value_, 30);
    ASSERT_EQ(((TestData *)dlist_at(list_, 1))->value_, 10);

    key->value_ = 30;
    removed_node = dlist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(((TestData *)removed_node->entry_)->value_, 30);
    delete (TestData *)removed_node->entry_;
    ASSERT_EQ(dlist_size(list_), 1);
    ASSERT_EQ(((TestData *)dlist_at(list_, 0))->value_, 10);

    key->value_ = 10;
    removed_node = dlist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(((TestData *)removed_node->entry_)->value_, 10);
    delete (TestData *)removed_node->entry_;
    ASSERT_TRUE(dlist_empty(list_));

    key->value_ = 50;
    removed_node = dlist_remove_if(list_, key, TestDataCompare);
    ASSERT_EQ(removed_node, nullptr);
}

TEST_F(DListTest, Reverse)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    dlist_push_back(list_, &data1->node_, data1);
    dlist_push_back(list_, &data2->node_, data2);
    dlist_push_back(list_, &data3->node_, data3);

    dlist_reverse(list_);

    ASSERT_EQ(((TestData *)dlist_at(list_, 0))->value_, 30);
    ASSERT_EQ(((TestData *)dlist_at(list_, 1))->value_, 20);
    ASSERT_EQ(((TestData *)dlist_at(list_, 2))->value_, 10);
}

TEST_F(DListTest, Swap)
{
    rcn_c::dlist other_list;
    dlist_init(&other_list);

    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);
    auto *data4 = new TestData(40);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);
    ASSERT_NE(data4, nullptr);

    dlist_push_front(list_, &data1->node_, data1);
    dlist_push_front(list_, &data2->node_, data2);
    dlist_push_front(&other_list, &data3->node_, data3);
    dlist_push_front(&other_list, &data4->node_, data4);

    dlist_swap(list_, &other_list);

    ASSERT_EQ(dlist_size(list_), 2);
    ASSERT_EQ(dlist_size(&other_list), 2);

    ASSERT_EQ(((TestData *)dlist_at(list_, 0))->value_, 40);
    ASSERT_EQ(((TestData *)dlist_at(list_, 1))->value_, 30);

    ASSERT_EQ(((TestData *)dlist_at(&other_list, 0))->value_, 20);
    ASSERT_EQ(((TestData *)dlist_at(&other_list, 1))->value_, 10);

    while (!dlist_empty(&other_list)) {
        delete (TestData *)dlist_pop_front(&other_list);
    }
}

TEST_F(DListTest, Clear)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    dlist_push_front(list_, &data1->node_, data1);
    dlist_push_front(list_, &data2->node_, data2);

    dlist_clear(list_);
    ASSERT_TRUE(dlist_empty(list_));
    ASSERT_EQ(dlist_size(list_), 0);

    delete data1;
    delete data2;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
