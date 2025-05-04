#include "gtest/gtest.h"
#include "rcn_cpp/queue.h"

class QueueTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        queue_ = new rcn_cpp::queue<int>(nr_entries_);
        queue_->alloc_entry();
    }

    void TearDown() override
    {
        queue_->free_entry();
        delete queue_;
    }

    static const size_t nr_entries_ = 5;
    rcn_cpp::queue<int> *queue_;
};

TEST_F(QueueTest, Init)
{
    ASSERT_EQ(queue_->size(), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(queue_->max_size(), nr_entries);
    ASSERT_TRUE(queue_->empty());
    ASSERT_FALSE(queue_->full());
}

TEST_F(QueueTest, Empty)
{
    ASSERT_TRUE(queue_->empty());
    int a = 1;
    queue_->push(&a);
    ASSERT_FALSE(queue_->empty());
}

TEST_F(QueueTest, Full)
{
    int v[nr_entries_];
    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        queue_->push(&v[i]);
    }
    ASSERT_TRUE(queue_->full());
    queue_->pop();
    ASSERT_FALSE(queue_->full());
}

TEST_F(QueueTest, PushPop)
{
    int a = 1, b = 2, c = 3;
    queue_->push(&a);
    queue_->push(&b);
    queue_->push(&c);

    ASSERT_EQ(queue_->size(), 3);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(queue_->max_size(), nr_entries);

    ASSERT_EQ(queue_->front(), &a);
    ASSERT_EQ(queue_->back(), &c);

    ASSERT_EQ(queue_->pop(), &a);
    ASSERT_EQ(queue_->pop(), &b);
    ASSERT_EQ(queue_->pop(), &c);
    ASSERT_TRUE(queue_->empty());
}

TEST_F(QueueTest, Clear)
{
    int v[nr_entries_];
    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        queue_->push(&v[i]);
    }
    queue_->clear();
    ASSERT_TRUE(queue_->empty());
}

TEST_F(QueueTest, FrontBack)
{
    int a = 1, b = 2;
    queue_->push(&a);
    queue_->push(&b);

    ASSERT_EQ(queue_->front(), &a);
    ASSERT_EQ(queue_->back(), &b);

    queue_->pop();
    ASSERT_EQ(queue_->front(), &b);
    ASSERT_EQ(queue_->back(), &b);
}

TEST_F(QueueTest, Swap)
{
    rcn_cpp::queue<int> other_queue(nr_entries_);
    other_queue.alloc_entry();

    int a = 1, b = 2;
    queue_->push(&a);
    queue_->push(&b);

    int c = 3, d = 4;
    other_queue.push(&c);
    other_queue.push(&d);

    queue_->swap(&other_queue);

    ASSERT_EQ(queue_->front(), &c);
    ASSERT_EQ(queue_->back(), &d);

    ASSERT_EQ(other_queue.front(), &a);
    ASSERT_EQ(other_queue.back(), &b);

    other_queue.free_entry();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
