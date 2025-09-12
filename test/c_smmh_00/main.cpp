#include "gtest/gtest.h"
#include "rcn_c/smmh.h"

class SMMHTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        heap_ = new rcn_c::smmh;
        smmh_init(heap_, nr_entries_ + 2, _ValueCompare, nullptr);
        __smmh_alloc_entry(heap_);
    }

    void TearDown() override
    {
        __smmh_free_entry(heap_);
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
    rcn_c::smmh *heap_;
};

TEST_F(SMMHTest, Init)
{
    ASSERT_EQ(smmh_size(heap_), 0);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(smmh_max_size(heap_), nr_entries);
    ASSERT_TRUE(smmh_empty(heap_));
    ASSERT_FALSE(smmh_full(heap_));
}

TEST_F(SMMHTest, Empty)
{
    ASSERT_TRUE(smmh_empty(heap_));
    smmh_push(heap_, (void *)1);
    ASSERT_FALSE(smmh_empty(heap_));
}

TEST_F(SMMHTest, Full)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        smmh_push(heap_, &v[i]);
    }

    ASSERT_TRUE(smmh_full(heap_));
    smmh_pop_front(heap_);
    ASSERT_FALSE(smmh_full(heap_));
}

TEST_F(SMMHTest, PushPop)
{
    int a = 1, b = 2, c = 3, d = 4, e = 5;

    smmh_push(heap_, &a);
    smmh_push(heap_, &b);
    smmh_push(heap_, &c);
    smmh_push(heap_, &d);
    smmh_push(heap_, &e);

    ASSERT_EQ(smmh_size(heap_), 5);
    static const size_t nr_entries = nr_entries_;
    ASSERT_EQ(smmh_max_size(heap_), nr_entries);

    ASSERT_EQ(smmh_front(heap_), &a);
    ASSERT_EQ(smmh_back(heap_), &e);

    ASSERT_EQ(smmh_pop_front(heap_), &a);
    ASSERT_EQ(smmh_pop_back(heap_), &e);

    ASSERT_EQ(smmh_pop_back(heap_), &d);
    ASSERT_EQ(smmh_pop_front(heap_), &b);

    ASSERT_EQ(smmh_front(heap_), &c);
    ASSERT_EQ(smmh_back(heap_), &c);
    ASSERT_EQ(smmh_pop_back(heap_), &c);

    ASSERT_TRUE(smmh_empty(heap_));
}

TEST_F(SMMHTest, Clear)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        smmh_push(heap_, &v[i]);
    }

    smmh_clear(heap_);
    ASSERT_TRUE(smmh_empty(heap_));
}

TEST_F(SMMHTest, FrontBack)
{
    int a = 1, b = 2;

    smmh_push(heap_, &a);
    smmh_push(heap_, &b);

    ASSERT_EQ(smmh_front(heap_), &a);
    ASSERT_EQ(smmh_back(heap_), &b);

    smmh_pop_front(heap_);
    ASSERT_EQ(smmh_front(heap_), &b);
    ASSERT_EQ(smmh_back(heap_), &b);
}

TEST_F(SMMHTest, Swap)
{
    rcn_c::smmh other_heap;

    smmh_init(&other_heap, nr_entries_, _ValueCompare, nullptr);
    __smmh_alloc_entry(&other_heap);

    int a = 1, b = 2;
    smmh_push(heap_, &a);
    smmh_push(heap_, &b);

    int c = 3, d = 4;
    smmh_push(&other_heap, &c);
    smmh_push(&other_heap, &d);

    smmh_swap(heap_, &other_heap);

    ASSERT_EQ(smmh_front(heap_), &c);
    ASSERT_EQ(smmh_front(&other_heap), &a);

    __smmh_free_entry(&other_heap);
}

TEST_F(SMMHTest, SMMHValidateion)
{
    int v[nr_entries_];

    for (size_t i = 0; i < nr_entries_; ++i) {
        v[i] = (int)(i + 1);
        smmh_push(heap_, &v[i]);
        smmh_validate(heap_);
    }

    ASSERT_TRUE(smmh_full(heap_));
    smmh_validate(heap_);
    smmh_pop_front(heap_);
    ASSERT_FALSE(smmh_full(heap_));
    smmh_validate(heap_);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
