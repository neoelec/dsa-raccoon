#include "gtest/gtest.h"
#include "rcn_c/deque.h"

class DequeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        deque_ = new rcn_c::deque;
        deque_init(deque_, nr_entries_, nullptr);
        __deque_alloc_entry(deque_);
    }

    void TearDown() override
    {
        __deque_free_entry(deque_);
        delete deque_;
    }

    const size_t nr_entries_ = 5;
    rcn_c::deque *deque_;
};

TEST_F(DequeTest, Init)
{
    ASSERT_EQ(deque_size(deque_), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(deque_max_size(deque_), nr_entries);
    ASSERT_TRUE(deque_empty(deque_));
    ASSERT_FALSE(deque_full(deque_));
}

TEST_F(DequeTest, PushFront)
{
    int a = 1, b = 2, c = 3;

    deque_push_front(deque_, &a);
    ASSERT_EQ(deque_size(deque_), 1);
    ASSERT_EQ(deque_front(deque_), &a);

    deque_push_front(deque_, &b);
    ASSERT_EQ(deque_size(deque_), 2);
    ASSERT_EQ(deque_front(deque_), &b);

    deque_push_front(deque_, &c);
    ASSERT_EQ(deque_size(deque_), 3);
    ASSERT_EQ(deque_front(deque_), &c);
}

TEST_F(DequeTest, PushBack)
{
    int a = 1, b = 2, c = 3;

    deque_push_back(deque_, &a);
    ASSERT_EQ(deque_size(deque_), 1);
    ASSERT_EQ(deque_back(deque_), &a);

    deque_push_back(deque_, &b);
    ASSERT_EQ(deque_size(deque_), 2);
    ASSERT_EQ(deque_back(deque_), &b);

    deque_push_back(deque_, &c);
    ASSERT_EQ(deque_size(deque_), 3);
    ASSERT_EQ(deque_back(deque_), &c);
}

TEST_F(DequeTest, PopFront)
{
    int a = 1, b = 2, c = 3;

    deque_push_front(deque_, &a);
    deque_push_front(deque_, &b);
    deque_push_front(deque_, &c);

    ASSERT_EQ(deque_pop_front(deque_), &c);
    ASSERT_EQ(deque_size(deque_), 2);
    ASSERT_EQ(deque_pop_front(deque_), &b);
    ASSERT_EQ(deque_size(deque_), 1);
    ASSERT_EQ(deque_pop_front(deque_), &a);
    ASSERT_EQ(deque_size(deque_), 0);
    ASSERT_TRUE(deque_empty(deque_));
}

TEST_F(DequeTest, PopBack)
{
    int a = 1, b = 2, c = 3;

    deque_push_back(deque_, &a);
    deque_push_back(deque_, &b);
    deque_push_back(deque_, &c);

    ASSERT_EQ(deque_pop_back(deque_), &c);
    ASSERT_EQ(deque_size(deque_), 2);
    ASSERT_EQ(deque_pop_back(deque_), &b);
    ASSERT_EQ(deque_size(deque_), 1);
    ASSERT_EQ(deque_pop_back(deque_), &a);
    ASSERT_EQ(deque_size(deque_), 0);
    ASSERT_TRUE(deque_empty(deque_));
}

TEST_F(DequeTest, FrontAndBack)
{
    int a = 1, b = 2, c = 3;

    deque_push_front(deque_, &a);
    ASSERT_EQ(deque_front(deque_), &a);
    ASSERT_EQ(deque_back(deque_), &a);

    deque_push_back(deque_, &b);
    ASSERT_EQ(deque_front(deque_), &a);
    ASSERT_EQ(deque_back(deque_), &b);

    deque_push_front(deque_, &c);
    ASSERT_EQ(deque_front(deque_), &c);
    ASSERT_EQ(deque_back(deque_), &b);
}

TEST_F(DequeTest, FullAndClear)
{
    int a = 1, b = 2, c = 3, d = 4, e = 5;

    deque_push_front(deque_, &a);
    deque_push_front(deque_, &b);
    deque_push_front(deque_, &c);
    deque_push_front(deque_, &d);
    deque_push_front(deque_, &e);

    ASSERT_TRUE(deque_full(deque_));

    deque_clear(deque_);
    ASSERT_TRUE(deque_empty(deque_));
}

TEST_F(DequeTest, Swap)
{
    rcn_c::deque other_deque;

    deque_init(&other_deque, nr_entries_, nullptr);
    __deque_alloc_entry(&other_deque);

    int a = 1, b = 2;
    deque_push_front(deque_, &a);
    deque_push_back(&other_deque, &b);

    deque_swap(deque_, &other_deque);

    ASSERT_EQ(deque_front(deque_), &b);
    ASSERT_EQ(deque_back(deque_), &b);
    ASSERT_EQ(deque_size(deque_), 1);

    ASSERT_EQ(deque_front(&other_deque), &a);
    ASSERT_EQ(deque_back(&other_deque), &a);
    ASSERT_EQ(deque_size(&other_deque), 1);
    ASSERT_EQ(deque_back(&other_deque), &a);

    __deque_free_entry(&other_deque);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
