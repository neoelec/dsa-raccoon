#include "gtest/gtest.h"
#include "rcn_c/stack.h"

class StackTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        stack_ = new rcn_c::stack;
        stack_init(stack_, nr_entries_, nullptr);
        stack_alloc_entry(stack_);
    }

    void TearDown() override
    {
        stack_free_entry(stack_);
        delete stack_;
    }

    static const size_t nr_entries_ = 5;
    rcn_c::stack *stack_;
};

TEST_F(StackTest, StackInitClear)
{
    ASSERT_EQ(stack_size(stack_), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(stack_max_size(stack_), nr_entries);
    ASSERT_TRUE(stack_empty(stack_));
    ASSERT_FALSE(stack_full(stack_));
}

TEST_F(StackTest, StackPushPop)
{
    int a = 1, b = 2, c = 3;

    stack_push(stack_, &a);
    ASSERT_EQ(stack_size(stack_), 1);
    ASSERT_FALSE(stack_empty(stack_));
    ASSERT_EQ(stack_top(stack_), &a);

    stack_push(stack_, &b);
    ASSERT_EQ(stack_size(stack_), 2);
    ASSERT_EQ(stack_top(stack_), &b);

    stack_push(stack_, &c);
    ASSERT_EQ(stack_size(stack_), 3);
    ASSERT_EQ(stack_top(stack_), &c);

    ASSERT_EQ(stack_pop(stack_), &c);
    ASSERT_EQ(stack_size(stack_), 2);
    ASSERT_EQ(stack_top(stack_), &b);

    ASSERT_EQ(stack_pop(stack_), &b);
    ASSERT_EQ(stack_size(stack_), 1);
    ASSERT_EQ(stack_top(stack_), &a);

    ASSERT_EQ(stack_pop(stack_), &a);
    ASSERT_EQ(stack_size(stack_), 0);
    ASSERT_TRUE(stack_empty(stack_));
    ASSERT_EQ(stack_top(stack_), nullptr);
}

TEST_F(StackTest, StackOverflow)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        stack_push(stack_, &v[i]);
    }

    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(stack_size(stack_), nr_entries);
    int a = 1;
    stack_push(stack_, &a);
    ASSERT_EQ(stack_size(stack_), nr_entries);
}

TEST_F(StackTest, StackUnderflow)
{
    ASSERT_TRUE(stack_empty(stack_));
    ASSERT_EQ(stack_pop(stack_), nullptr);
}

TEST_F(StackTest, StackSwap)
{
    rcn_c::stack other_stack;

    stack_init(&other_stack, nr_entries_, nullptr);
    stack_alloc_entry(&other_stack);

    int a = 1, b = 2;
    stack_push(stack_, &a);
    stack_push(&other_stack, &b);

    stack_swap(stack_, &other_stack);

    ASSERT_EQ(stack_size(stack_), 1);
    ASSERT_EQ(stack_top(stack_), &b);

    ASSERT_EQ(stack_size(&other_stack), 1);
    ASSERT_EQ(stack_top(&other_stack), &a);

    stack_free_entry(&other_stack);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
