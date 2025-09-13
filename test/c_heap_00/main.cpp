#include "gtest/gtest.h"
#include "rcn_c/heap.h"

class HeapTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        heap_ = new rcn_c::heap;
        heap_init(heap_, nr_entries_, _ValueCompare, nullptr);
        __heap_alloc_entry(heap_);
    }

    void TearDown() override
    {
        __heap_free_entry(heap_);
        delete heap_;
    }

    static int _ValueCompare(const void *_ke, const void *_in_heap)
    {
        int ke = *(int *)_ke;
        int in_heap = *(int *)_in_heap;

        if (ke < in_heap) {
            return -1;
        }

        if (ke > in_heap) {
            return 1;
        }

        return 0;
    }

    static const size_t nr_entries_ = 5;
    rcn_c::heap *heap_;
};

TEST_F(HeapTest, Init)
{
    ASSERT_EQ(heap_size(heap_), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(heap_max_size(heap_), nr_entries);
    ASSERT_TRUE(heap_empty(heap_));
    ASSERT_FALSE(heap_full(heap_));
}

TEST_F(HeapTest, Empty)
{
    ASSERT_TRUE(heap_empty(heap_));
    heap_push(heap_, (void *)1);
    ASSERT_FALSE(heap_empty(heap_));
}

TEST_F(HeapTest, Full)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        heap_push(heap_, &v[i]);
    }

    ASSERT_TRUE(heap_full(heap_));
    heap_pop(heap_);
    ASSERT_FALSE(heap_full(heap_));
}

TEST_F(HeapTest, PushPop)
{
    int a = 1, b = 2, c = 3;

    heap_push(heap_, &a);
    heap_push(heap_, &b);
    heap_push(heap_, &c);

    ASSERT_EQ(heap_size(heap_), 3);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(heap_max_size(heap_), nr_entries);

    ASSERT_EQ(heap_top(heap_), &a);

    ASSERT_EQ(heap_pop(heap_), &a);
    ASSERT_EQ(heap_pop(heap_), &b);
    ASSERT_EQ(heap_pop(heap_), &c);
    ASSERT_TRUE(heap_empty(heap_));
}

TEST_F(HeapTest, Clear)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        heap_push(heap_, &v[i]);
    }

    heap_clear(heap_);
    ASSERT_TRUE(heap_empty(heap_));
}

TEST_F(HeapTest, FrontBack)
{
    int a = 1, b = 2;

    heap_push(heap_, &a);
    heap_push(heap_, &b);

    ASSERT_EQ(heap_top(heap_), &a);

    heap_pop(heap_);
    ASSERT_EQ(heap_top(heap_), &b);
}

TEST_F(HeapTest, Swap)
{
    rcn_c::heap other_heap;

    heap_init(&other_heap, nr_entries_, _ValueCompare, nullptr);
    __heap_alloc_entry(&other_heap);

    int a = 1, b = 2;
    heap_push(heap_, &a);
    heap_push(heap_, &b);

    int c = 3, d = 4;
    heap_push(&other_heap, &c);
    heap_push(&other_heap, &d);

    heap_swap(heap_, &other_heap);

    ASSERT_EQ(heap_top(heap_), &c);
    ASSERT_EQ(heap_top(&other_heap), &a);

    __heap_free_entry(&other_heap);
}

TEST_F(HeapTest, HeapValidateion)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        heap_push(heap_, &v[i]);
        heap_validate(heap_);
    }

    ASSERT_TRUE(heap_full(heap_));
    heap_validate(heap_);
    heap_pop(heap_);
    ASSERT_FALSE(heap_full(heap_));
    heap_validate(heap_);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
