#include "gtest/gtest.h"
#include "rcn_cpp/dlist.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_cpp::dlnode<TestData> node_;
    int value_;
};

class DListTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        list_ = new rcn_cpp::dlist<TestData>;
    }

    void TearDown() override
    {
        while (!list_->empty()) {
            delete list_->pop_front();
        }

        delete list_;
    }

    rcn_cpp::dlist<TestData> *list_;
};

TEST_F(DListTest, Init)
{
    ASSERT_TRUE(list_->empty());
    ASSERT_EQ(list_->size(), 0);
}

TEST_F(DListTest, PushFrontAndBack)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    list_->push_front(&data1->node_, data1);
    list_->push_back(&data2->node_, data2);
    list_->push_front(&data3->node_, data3);

    ASSERT_EQ(list_->size(), 3);
    auto front = list_->front();
    ASSERT_EQ(front->value_, 30);
    auto back = list_->back();
    ASSERT_EQ(back->value_, 20);
}

TEST_F(DListTest, PopFrontAndBack)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    list_->push_back(&data1->node_, data1);
    list_->push_back(&data2->node_, data2);
    ASSERT_EQ(list_->size(), 2);

    auto *popped_front = list_->pop_front();
    ASSERT_EQ(popped_front->value_, 10);
    delete popped_front;
    ASSERT_EQ(list_->size(), 1);

    auto *popped_back = list_->pop_back();
    ASSERT_EQ(popped_back->value_, 20);
    delete popped_back;
    ASSERT_TRUE(list_->empty());
}

TEST_F(DListTest, At)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    list_->push_front(&data1->node_, data1);
    list_->push_front(&data2->node_, data2);
    list_->push_front(&data3->node_, data3);

    ASSERT_EQ(list_->at(0)->value_, 30);
    ASSERT_EQ(list_->at(1)->value_, 20);
    ASSERT_EQ(list_->at(2)->value_, 10);
    ASSERT_EQ(list_->at(3), nullptr);
}

TEST_F(DListTest, Erase)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);

    list_->push_back(&data1->node_, data1);
    list_->push_back(&data2->node_, data2);
    list_->push_back(&data3->node_, data3);

    ASSERT_EQ(list_->size(), 3);

    auto second_node = list_->begin()->next_;
    auto erased_data = second_node->erase();

    ASSERT_EQ(erased_data->value_, 20);
    delete erased_data;

    ASSERT_EQ(list_->size(), 2);
    auto idx_0_data = list_->at(0);
    ASSERT_EQ(idx_0_data->value_, 10);
    auto idx_1_data = list_->at(1);
    ASSERT_EQ(idx_1_data->value_, 30);
}

TEST_F(DListTest, SizeAndEmpty)
{
    ASSERT_TRUE(list_->empty());
    ASSERT_EQ(list_->size(), 0);

    auto *data = new TestData(10);

    ASSERT_NE(data, nullptr);

    list_->push_back(&data->node_, data);

    ASSERT_FALSE(list_->empty());
    ASSERT_EQ(list_->size(), 1);
}

int TestDataCompare(const TestData *ke, const TestData *in_list)
{
    if (ke->value_ < in_list->value_) {
        return -1;
    }

    if (ke->value_ > in_list->value_) {
        return 1;
    }

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

    list_->push_front(&data1->node_, data1);
    list_->push_front(&data2->node_, data2);
    list_->push_front(&data3->node_, data3);

    auto *key = new TestData(20);
    auto removed_node = list_->remove_if(key, TestDataCompare);
    ASSERT_EQ(removed_node->entry_->value_, 20);
    ASSERT_NE(removed_node, nullptr);
    delete removed_node->entry_;

    ASSERT_EQ(list_->size(), 2);
    ASSERT_EQ(list_->at(0)->value_, 30);
    ASSERT_EQ(list_->at(1)->value_, 10);

    key->value_ = 30;
    removed_node = list_->remove_if(key, TestDataCompare);
    ASSERT_EQ(removed_node->entry_->value_, 30);
    delete removed_node->entry_;
    ASSERT_EQ(list_->size(), 1);
    ASSERT_EQ(list_->at(0)->value_, 10);

    key->value_ = 10;
    removed_node = list_->remove_if(key, TestDataCompare);
    ASSERT_EQ(removed_node->entry_->value_, 10);
    delete removed_node->entry_;
    ASSERT_TRUE(list_->empty());

    key->value_ = 50;
    removed_node = list_->remove_if(key, TestDataCompare);
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

    list_->push_back(&data1->node_, data1);
    list_->push_back(&data2->node_, data2);
    list_->push_back(&data3->node_, data3);

    list_->reverse();

    ASSERT_EQ(list_->at(0)->value_, 30);
    ASSERT_EQ(list_->at(1)->value_, 20);
    ASSERT_EQ(list_->at(2)->value_, 10);
}

TEST_F(DListTest, Swap)
{
    rcn_cpp::dlist<TestData> other_list;

    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);
    auto *data3 = new TestData(30);
    auto *data4 = new TestData(40);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);
    ASSERT_NE(data3, nullptr);
    ASSERT_NE(data4, nullptr);

    list_->push_front(&data1->node_, data1);
    list_->push_front(&data2->node_, data2);
    other_list.push_front(&data3->node_, data3);
    other_list.push_front(&data4->node_, data4);

    list_->swap(&other_list);

    ASSERT_EQ(list_->size(), 2);
    ASSERT_EQ(other_list.size(), 2);

    ASSERT_EQ(list_->at(0)->value_, 40);
    ASSERT_EQ(list_->at(1)->value_, 30);

    ASSERT_EQ(other_list.at(0)->value_, 20);
    ASSERT_EQ(other_list.at(1)->value_, 10);

    while (!other_list.empty()) {
        delete other_list.pop_front();
    }
}

TEST_F(DListTest, Clear)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    list_->push_front(&data1->node_, data1);
    list_->push_front(&data2->node_, data2);

    list_->clear();
    ASSERT_TRUE(list_->empty());
    ASSERT_EQ(list_->size(), 0);

    delete data1;
    delete data2;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
