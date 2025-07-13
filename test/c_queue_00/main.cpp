#include "gtest/gtest.h"
#include "rcn_c/queue.h"

class QueueTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        queue_ = new rcn_c::queue;
        queue_init(queue_, nr_entries_, nullptr);
        queue_alloc_entry(queue_);
    }

    void TearDown() override
    {
        queue_free_entry(queue_);
        delete queue_;
    }

    static const size_t nr_entries_ = 5;
    rcn_c::queue *queue_;
};

TEST_F(QueueTest, Init)
{
    ASSERT_EQ(queue_size(queue_), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(queue_max_size(queue_), nr_entries);
    ASSERT_TRUE(queue_empty(queue_));
    ASSERT_FALSE(queue_full(queue_));
}

TEST_F(QueueTest, Empty)
{
    ASSERT_TRUE(queue_empty(queue_));
    queue_push(queue_, (void *)1);
    ASSERT_FALSE(queue_empty(queue_));
}

TEST_F(QueueTest, Full)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        queue_push(queue_, &v[i]);
    }

    ASSERT_TRUE(queue_full(queue_));
    queue_pop(queue_);
    ASSERT_FALSE(queue_full(queue_));
}

TEST_F(QueueTest, PushPop)
{
    int a = 1, b = 2, c = 3;

    queue_push(queue_, &a);
    queue_push(queue_, &b);
    queue_push(queue_, &c);

    ASSERT_EQ(queue_size(queue_), 3);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(queue_max_size(queue_), nr_entries);

    ASSERT_EQ(queue_front(queue_), &a);
    ASSERT_EQ(queue_back(queue_), &c);

    ASSERT_EQ(queue_pop(queue_), &a);
    ASSERT_EQ(queue_pop(queue_), &b);
    ASSERT_EQ(queue_pop(queue_), &c);
    ASSERT_TRUE(queue_empty(queue_));
}

TEST_F(QueueTest, Clear)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        queue_push(queue_, &v[i]);
    }

    queue_clear(queue_);
    ASSERT_TRUE(queue_empty(queue_));
}

TEST_F(QueueTest, FrontBack)
{
    int a = 1, b = 2;

    queue_push(queue_, &a);
    queue_push(queue_, &b);

    ASSERT_EQ(queue_front(queue_), &a);
    ASSERT_EQ(queue_back(queue_), &b);

    queue_pop(queue_);
    ASSERT_EQ(queue_front(queue_), &b);
    ASSERT_EQ(queue_back(queue_), &b);
}

TEST_F(QueueTest, Swap)
{
    rcn_c::queue other_queue;

    queue_init(&other_queue, nr_entries_, nullptr);
    queue_alloc_entry(&other_queue);

    int a = 1, b = 2;
    queue_push(queue_, &a);
    queue_push(queue_, &b);

    int c = 3, d = 4;
    queue_push(&other_queue, &c);
    queue_push(&other_queue, &d);

    queue_swap(queue_, &other_queue);

    ASSERT_EQ(queue_front(queue_), &c);
    ASSERT_EQ(queue_back(queue_), &d);

    ASSERT_EQ(queue_front(&other_queue), &a);
    ASSERT_EQ(queue_back(&other_queue), &b);

    queue_free_entry(&other_queue);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
