#include "gtest/gtest.h"
#include "rcn_c/spltree.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_c::splnode node_;
    int value_;
};

class SplayTreeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        tree_ = new rcn_c::spltree;
        spltree_init(tree_, _ValueCompare);
    }

    void TearDown() override
    {
        while (!spltree_empty(tree_)) {
            delete (TestData *)spltree_pop_front(tree_);
        }

        delete tree_;
    }

    static int _ValueCompare(const void *_ke, const void *_in_tree)
    {
        auto ke = (const TestData *)_ke;
        auto in_tree = (const TestData *)_in_tree;

        if (ke->value_ < in_tree->value_) {
            return -1;
        }

        if (ke->value_ > in_tree->value_) {
            return 1;
        }

        return 0;
    }

    rcn_c::spltree *tree_;
};

TEST_F(SplayTreeTest, InitAndEmpty)
{
    ASSERT_TRUE(spltree_empty(tree_));
}

TEST_F(SplayTreeTest, InsertAndFind)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);

    ASSERT_FALSE(spltree_empty(tree_));

    auto key = new TestData(100);
    auto found = spltree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 100);

    key->value_ = 50;
    found = spltree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 50);

    key->value_ = 150;
    found = spltree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 150);

    key->value_ = 20;
    found = spltree_find(tree_, key);
    ASSERT_EQ(found, nullptr);

    delete key;
}

TEST_F(SplayTreeTest, BeginNextEnd)
{
    ASSERT_EQ(spltree_begin(tree_), nullptr);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);

    rcn_c::splnode *node = spltree_begin(tree_);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 50);

    node = spltree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 100);

    node = spltree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 150);

    node = spltree_next(tree_, node);
    ASSERT_EQ(node, nullptr);
}

TEST_F(SplayTreeTest, Erase)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);

    auto key = new TestData(50);
    auto found = spltree_find(tree_, key);
    ASSERT_NE(found, nullptr);

    rcn_c::splnode *node_to_erase = nullptr;

    for (rcn_c::splnode *node = spltree_begin(tree_);
         node != spltree_end(tree_); node = spltree_next(tree_, node)) {
        if (((TestData *)node->entry_)->value_ == 50) {
            node_to_erase = node;
            break;
        }
    }

    ASSERT_NE(node_to_erase, nullptr);

    auto erased = (TestData *)spltree_erase(tree_, node_to_erase);
    ASSERT_EQ(erased, data2);

    found = spltree_find(tree_, key);
    ASSERT_EQ(found, nullptr);

    delete key;
}

TEST_F(SplayTreeTest, Count)
{
    ASSERT_EQ(spltree_size(tree_), 0);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    spltree_insert(tree_, &data1->node_, data1);
    ASSERT_EQ(spltree_size(tree_), 1);
    spltree_insert(tree_, &data2->node_, data2);
    ASSERT_EQ(spltree_size(tree_), 2);
    spltree_insert(tree_, &data3->node_, data3);
    ASSERT_EQ(spltree_size(tree_), 3);
}

TEST_F(SplayTreeTest, FrontAndBack)
{
    ASSERT_EQ(spltree_front(tree_), nullptr);
    ASSERT_EQ(spltree_back(tree_), nullptr);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);

    ASSERT_EQ(((TestData *)spltree_front(tree_))->value_, 50);
    ASSERT_EQ(((TestData *)spltree_back(tree_))->value_, 150);
}

TEST_F(SplayTreeTest, PopFrontAndPopBack)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);

    auto front = (TestData *)spltree_pop_front(tree_);
    ASSERT_EQ(front->value_, 50);
    ASSERT_EQ(spltree_size(tree_), 2);

    auto back = (TestData *)spltree_pop_back(tree_);
    ASSERT_EQ(back->value_, 150);
    ASSERT_EQ(spltree_size(tree_), 1);

    auto remaining = (TestData *)spltree_pop_front(tree_);
    ASSERT_EQ(remaining->value_, 100);
    ASSERT_EQ(spltree_size(tree_), 0);
    ASSERT_TRUE(spltree_empty(tree_));
}

TEST_F(SplayTreeTest, At)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);

    auto found = (TestData *)spltree_at(tree_, 0);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 50);

    found = (TestData *)spltree_at(tree_, 1);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 100);

    found = (TestData *)spltree_at(tree_, 2);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 150);

    found = (TestData *)spltree_at(tree_, 3);
    ASSERT_EQ(found, nullptr);
}

TEST_F(SplayTreeTest, LowerBound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);
    spltree_insert(tree_, &data4->node_, data4);
    spltree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto lb = spltree_lower_bound(tree_, key);
    ASSERT_NE(lb, nullptr);
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 75);

    key->value_ = 100;
    lb = spltree_lower_bound(tree_, key);
    ASSERT_NE(lb, nullptr);
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 100);

    key->value_ = 200;
    lb = spltree_lower_bound(tree_, key);
    ASSERT_EQ(lb, nullptr);

    delete key;
}

TEST_F(SplayTreeTest, UpperBound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    spltree_insert(tree_, &data1->node_, data1);
    spltree_insert(tree_, &data2->node_, data2);
    spltree_insert(tree_, &data3->node_, data3);
    spltree_insert(tree_, &data4->node_, data4);
    spltree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto ub = spltree_upper_bound(tree_, key);
    ASSERT_NE(ub, nullptr);
    ASSERT_EQ(((TestData *)ub->entry_)->value_, 75);

    key->value_ = 100;
    ub = spltree_upper_bound(tree_, key);
    ASSERT_NE(ub, nullptr);
    ASSERT_EQ(((TestData *)ub->entry_)->value_, 125);

    key->value_ = 200;
    ub = spltree_upper_bound(tree_, key);
    ASSERT_EQ(ub, nullptr);

    delete key;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
