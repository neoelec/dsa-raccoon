#include "gtest/gtest.h"
#include "rcn_cpp/deque.h"

class DequeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        deque_ = new rcn_cpp::deque<int>(nr_entries_);
        deque_->alloc_entry();
    }

    void TearDown() override
    {
        deque_->free_entry();
        delete deque_;
    }

    const size_t nr_entries_ = 5;
    rcn_cpp::deque<int> *deque_;
};

TEST_F(DequeTest, Init)
{
    ASSERT_EQ(deque_->size(), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(deque_->max_size(), nr_entries);
    ASSERT_TRUE(deque_->empty());
    ASSERT_FALSE(deque_->full());
}

TEST_F(DequeTest, PushFront)
{
    int a = 1, b = 2, c = 3;
    deque_->push_front(&a);
    ASSERT_EQ(deque_->size(), 1);
    ASSERT_EQ(deque_->front(), &a);

    deque_->push_front(&b);
    ASSERT_EQ(deque_->size(), 2);
    ASSERT_EQ(deque_->front(), &b);

    deque_->push_front(&c);
    ASSERT_EQ(deque_->size(), 3);
    ASSERT_EQ(deque_->front(), &c);
}

TEST_F(DequeTest, PushBack)
{
    int a = 1, b = 2, c = 3;
    deque_->push_back(&a);
    ASSERT_EQ(deque_->size(), 1);
    ASSERT_EQ(deque_->back(), &a);

    deque_->push_back(&b);
    ASSERT_EQ(deque_->size(), 2);
    ASSERT_EQ(deque_->back(), &b);

    deque_->push_back(&c);
    ASSERT_EQ(deque_->size(), 3);
    ASSERT_EQ(deque_->back(), &c);
}

TEST_F(DequeTest, PopFront)
{
    int a = 1, b = 2, c = 3;
    deque_->push_front(&a);
    deque_->push_front(&b);
    deque_->push_front(&c);

    ASSERT_EQ(deque_->pop_front(), &c);
    ASSERT_EQ(deque_->size(), 2);
    ASSERT_EQ(deque_->pop_front(), &b);
    ASSERT_EQ(deque_->size(), 1);
    ASSERT_EQ(deque_->pop_front(), &a);
    ASSERT_EQ(deque_->size(), 0);
    ASSERT_TRUE(deque_->empty());
}

TEST_F(DequeTest, PopBack)
{
    int a = 1, b = 2, c = 3;
    deque_->push_back(&a);
    deque_->push_back(&b);
    deque_->push_back(&c);

    ASSERT_EQ(deque_->pop_back(), &c);
    ASSERT_EQ(deque_->size(), 2);
    ASSERT_EQ(deque_->pop_back(), &b);
    ASSERT_EQ(deque_->size(), 1);
    ASSERT_EQ(deque_->pop_back(), &a);
    ASSERT_EQ(deque_->size(), 0);
    ASSERT_TRUE(deque_->empty());
}

TEST_F(DequeTest, FrontAndBack)
{
    int a = 1, b = 2, c = 3;
    deque_->push_front(&a);
    ASSERT_EQ(deque_->front(), &a);
    ASSERT_EQ(deque_->back(), &a);

    deque_->push_back(&b);
    ASSERT_EQ(deque_->front(), &a);
    ASSERT_EQ(deque_->back(), &b);

    deque_->push_front(&c);
    ASSERT_EQ(deque_->front(), &c);
    ASSERT_EQ(deque_->back(), &b);
}

TEST_F(DequeTest, FullAndClear)
{
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    deque_->push_front(&a);
    deque_->push_front(&b);
    deque_->push_front(&c);
    deque_->push_front(&d);
    deque_->push_front(&e);

    ASSERT_TRUE(deque_->full());

    deque_->clear();
    ASSERT_TRUE(deque_->empty());
}

TEST_F(DequeTest, Swap)
{
    rcn_cpp::deque<int> other_deque(nr_entries_);
    other_deque.alloc_entry();

    int a = 1, b = 2;
    deque_->push_front(&a);
    other_deque.push_back(&b);

    deque_->swap(&other_deque);

    ASSERT_EQ(deque_->front(), &b);
    ASSERT_EQ(deque_->back(), &b);
    ASSERT_EQ(deque_->size(), 1);

    ASSERT_EQ(other_deque.front(), &a);
    ASSERT_EQ(other_deque.back(), &a);
    ASSERT_EQ(other_deque.size(), 1);
    ASSERT_EQ(other_deque.back(), &a);

    other_deque.free_entry();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
