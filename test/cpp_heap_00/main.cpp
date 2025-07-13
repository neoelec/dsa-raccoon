#include "gtest/gtest.h"
#include "rcn_cpp/heap.h"

class HeapTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        heap_ = new rcn_cpp::heap<int>(nr_entries_, _ValueCompare);
        heap_->alloc_entry();
    }

    void TearDown() override
    {
        heap_->free_entry();
        delete heap_;
    }

    static int _ValueCompare(const int *_ke, const int *_in_heap)
    {
        if (*_ke < *_in_heap) {
            return -1;
        }

        if (*_ke > *_in_heap) {
            return 1;
        }

        return 0;
    }

    static const size_t nr_entries_ = 5;
    rcn_cpp::heap<int> *heap_;
};

TEST_F(HeapTest, Init)
{
    ASSERT_EQ(heap_->size(), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(heap_->max_size(), nr_entries);
    ASSERT_TRUE(heap_->empty());
    ASSERT_FALSE(heap_->full());
}

TEST_F(HeapTest, Empty)
{
    ASSERT_TRUE(heap_->empty());
    int a = 1;
    heap_->push(&a);
    ASSERT_FALSE(heap_->empty());
}

TEST_F(HeapTest, Full)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        heap_->push(&v[i]);
    }

    ASSERT_TRUE(heap_->full());
    heap_->pop();
    ASSERT_FALSE(heap_->full());
}

TEST_F(HeapTest, PushPop)
{
    int a = 1, b = 2, c = 3;

    heap_->push(&a);
    heap_->push(&b);
    heap_->push(&c);

    ASSERT_EQ(heap_->size(), 3);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(heap_->max_size(), nr_entries);

    ASSERT_EQ(*heap_->top(), a);

    ASSERT_EQ(*heap_->pop(), a);
    ASSERT_EQ(heap_->pop(), &b);
    ASSERT_EQ(heap_->pop(), &c);
    ASSERT_TRUE(heap_->empty());
}

TEST_F(HeapTest, Clear)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        heap_->push(&v[i]);
    }

    heap_->clear();
    ASSERT_TRUE(heap_->empty());
}

TEST_F(HeapTest, FrontBack)
{
    int a = 1, b = 2;

    heap_->push(&a);
    heap_->push(&b);

    ASSERT_EQ(heap_->top(), &a);

    heap_->pop();
    ASSERT_EQ(heap_->top(), &b);
}

TEST_F(HeapTest, Swap)
{
    rcn_cpp::heap other_heap(nr_entries_, _ValueCompare);

    other_heap.alloc_entry();

    int a = 1, b = 2;
    heap_->push(&a);
    heap_->push(&b);

    int c = 3, d = 4;
    other_heap.push(&c);
    other_heap.push(&d);

    heap_->swap(&other_heap);

    ASSERT_EQ(heap_->top(), &c);
    ASSERT_EQ(other_heap.top(), &a);

    other_heap.free_entry();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
