#include "gtest/gtest.h"
#include "rcn_cpp/slist.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_cpp::slnode<TestData> node_;
    int value_;
};

class SListTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        list_ = new rcn_cpp::slist<TestData>;
    }

    void TearDown() override
    {
        while (!list_->empty()) {
            delete list_->pop_front();
        }

        delete list_;
    }

    rcn_cpp::slist<TestData> *list_;
};

TEST_F(SListTest, Init)
{
    ASSERT_TRUE(list_->empty());
    ASSERT_EQ(list_->size(), 0);
}

TEST_F(SListTest, PushFront)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    list_->push_front(&data1->node_, data1);
    auto front = list_->front();
    ASSERT_EQ(front->value_, 10);
    ASSERT_EQ(list_->size(), 1);

    list_->push_front(&data2->node_, data2);
    front = list_->front();
    ASSERT_EQ(front->value_, 20);
    ASSERT_EQ(list_->size(), 2);
}

TEST_F(SListTest, PopFront)
{
    auto *data1 = new TestData(10);
    auto *data2 = new TestData(20);

    ASSERT_NE(data1, nullptr);
    ASSERT_NE(data2, nullptr);

    list_->push_front(&data1->node_, data1);
    list_->push_front(&data2->node_, data2);

    auto popped_front = list_->pop_front();
    ASSERT_EQ(popped_front->value_, 20);
    delete popped_front;
    ASSERT_EQ(list_->size(), 1);

    popped_front = list_->pop_front();
    ASSERT_EQ(popped_front->value_, 10);
    delete popped_front;
    ASSERT_TRUE(list_->empty());
}

TEST_F(SListTest, At)
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

TEST_F(SListTest, EraseAfter)
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

    auto begin = list_->begin();
    auto first_data = begin->erase_after();
    ASSERT_EQ(first_data->value_, 20);
    delete first_data;
    ASSERT_EQ(list_->at(0)->value_, 30);
    ASSERT_EQ(list_->at(1)->value_, 10);
}

TEST_F(SListTest, SizeAndEmpty)
{
    ASSERT_TRUE(list_->empty());
    ASSERT_EQ(list_->size(), 0);

    auto *data = new TestData(10);

    ASSERT_NE(data, nullptr);

    list_->push_front(&data->node_, data);

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

TEST_F(SListTest, RemoveIf)
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

TEST_F(SListTest, Reverse)
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

    list_->reverse();

    ASSERT_EQ(list_->at(0)->value_, 10);
    ASSERT_EQ(list_->at(1)->value_, 20);
    ASSERT_EQ(list_->at(2)->value_, 30);
}

TEST_F(SListTest, Swap)
{
    rcn_cpp::slist<TestData> other_list;

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

TEST_F(SListTest, Clear)
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
