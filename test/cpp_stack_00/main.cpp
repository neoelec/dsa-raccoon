#include "gtest/gtest.h"
#include "rcn_cpp/stack.h"

class StackTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        stack_ = new rcn_cpp::stack<int>(nr_entries_);
        stack_->alloc_entry();
    }

    void TearDown() override
    {
        stack_->free_entry();
        delete stack_;
    }

    static const size_t nr_entries_ = 5;
    rcn_cpp::stack<int> *stack_;
};

TEST_F(StackTest, StackInitClear)
{
    ASSERT_EQ(stack_->size(), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(stack_->max_size(), nr_entries);
    ASSERT_TRUE(stack_->empty());
    ASSERT_FALSE(stack_->full());
}

TEST_F(StackTest, StackPushPop)
{
    int a = 1, b = 2, c = 3;

    stack_->push(&a);
    ASSERT_EQ(stack_->size(), 1);
    ASSERT_FALSE(stack_->empty());
    ASSERT_EQ(stack_->top(), &a);

    stack_->push(&b);
    ASSERT_EQ(stack_->size(), 2);
    ASSERT_EQ(stack_->top(), &b);

    stack_->push(&c);
    ASSERT_EQ(stack_->size(), 3);
    ASSERT_EQ(stack_->top(), &c);

    ASSERT_EQ(stack_->pop(), &c);
    ASSERT_EQ(stack_->size(), 2);
    ASSERT_EQ(stack_->top(), &b);

    ASSERT_EQ(stack_->pop(), &b);
    ASSERT_EQ(stack_->size(), 1);
    ASSERT_EQ(stack_->top(), &a);

    ASSERT_EQ(stack_->pop(), &a);
    ASSERT_EQ(stack_->size(), 0);
    ASSERT_TRUE(stack_->empty());
    ASSERT_EQ(stack_->top(), nullptr);
}

TEST_F(StackTest, StackOverflow)
{
    int v[nr_entries_];
    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        stack_->push(&v[i]);
    }
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(stack_->size(), nr_entries);
    int a = 1;
    stack_->push(&a);
    ASSERT_EQ(stack_->size(), nr_entries);
}

TEST_F(StackTest, StackUnderflow)
{
    ASSERT_TRUE(stack_->empty());
    ASSERT_EQ(stack_->pop(), nullptr);
}

TEST_F(StackTest, StackSwap)
{
    rcn_cpp::stack<int> other_stack(nr_entries_);
    other_stack.alloc_entry();

    int a = 1, b = 2;
    stack_->push(&a);
    other_stack.push(&b);

    stack_->swap(&other_stack);

    ASSERT_EQ(stack_->size(), 1);
    ASSERT_EQ(stack_->top(), &b);

    ASSERT_EQ(other_stack.size(), 1);
    ASSERT_EQ(other_stack.top(), &a);

    other_stack.free_entry();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
