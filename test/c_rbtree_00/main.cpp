#include "gtest/gtest.h"
#include "rcn_c/rbtree.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_c::rbnode node_;
    int value_;
};

class RedBlackTreeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        tree_ = new rcn_c::rbtree;
        rbtree_init(tree_, _ValueCompare);
    }

    void TearDown() override
    {
        while (!rbtree_empty(tree_)) {
            delete (TestData *)rbtree_pop_front(tree_);
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

    rcn_c::rbtree *tree_;
};

TEST_F(RedBlackTreeTest, InitAndEmpty)
{
    ASSERT_TRUE(rbtree_empty(tree_));
}

TEST_F(RedBlackTreeTest, InsertAndFind)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    rbtree_insert(tree_, &data1->node_, data1);
    rbtree_insert(tree_, &data2->node_, data2);
    rbtree_insert(tree_, &data3->node_, data3);

    ASSERT_FALSE(rbtree_empty(tree_));

    auto key = new TestData(100);
    auto found = rbtree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)found->entry_)->value_, 100);

    key->value_ = 50;
    found = rbtree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)found->entry_)->value_, 50);

    key->value_ = 150;
    found = rbtree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)found->entry_)->value_, 150);

    key->value_ = 20;
    found = rbtree_find(tree_, key);
    ASSERT_EQ(found, rbtree_nil(tree_));

    delete key;
}

TEST_F(RedBlackTreeTest, BeginNextEnd)
{
    ASSERT_EQ(rbtree_begin(tree_), rbtree_nil(tree_));

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    rbtree_insert(tree_, &data1->node_, data1);
    rbtree_insert(tree_, &data2->node_, data2);
    rbtree_insert(tree_, &data3->node_, data3);

    rcn_c::rbnode *node = rbtree_begin(tree_);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)node->entry_)->value_, 50);

    node = rbtree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)node->entry_)->value_, 100);

    node = rbtree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)node->entry_)->value_, 150);

    node = rbtree_next(tree_, node);
    ASSERT_EQ(node, rbtree_nil(tree_));
}

TEST_F(RedBlackTreeTest, Erase)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    rbtree_insert(tree_, &data1->node_, data1);
    rbtree_insert(tree_, &data2->node_, data2);
    rbtree_insert(tree_, &data3->node_, data3);

    auto key = new TestData(50);
    auto found = rbtree_find(tree_, key);
    ASSERT_NE(found, rbtree_nil(tree_));

    rcn_c::rbnode *node_to_erase = (rcn_c::rbnode *)rbtree_nil(tree_);

    for (rcn_c::rbnode *node = rbtree_begin(tree_); node != rbtree_end(tree_);
         node = rbtree_next(tree_, node)) {
        if (((TestData *)node->entry_)->value_ == 50) {
            node_to_erase = node;
            break;
        }
    }

    ASSERT_NE(node_to_erase, nullptr);
    ASSERT_NE(node_to_erase, rbtree_nil(tree_));

    auto erased = (TestData *)rbtree_erase(tree_, node_to_erase);
    ASSERT_EQ(erased, data2);

    found = rbtree_find(tree_, key);
    ASSERT_EQ(found, rbtree_nil(tree_));

    delete key;
}

TEST_F(RedBlackTreeTest, Count)
{
    ASSERT_EQ(rbtree_count(tree_), 0);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    rbtree_insert(tree_, &data1->node_, data1);
    ASSERT_EQ(rbtree_count(tree_), 1);
    rbtree_insert(tree_, &data2->node_, data2);
    ASSERT_EQ(rbtree_count(tree_), 2);
    rbtree_insert(tree_, &data3->node_, data3);
    ASSERT_EQ(rbtree_count(tree_), 3);
}

TEST_F(RedBlackTreeTest, FrontAndBack)
{
    ASSERT_EQ(rbtree_front(tree_), nullptr);
    ASSERT_EQ(rbtree_back(tree_), nullptr);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    rbtree_insert(tree_, &data1->node_, data1);
    rbtree_insert(tree_, &data2->node_, data2);
    rbtree_insert(tree_, &data3->node_, data3);

    ASSERT_EQ(((TestData *)rbtree_front(tree_))->value_, 50);
    ASSERT_EQ(((TestData *)rbtree_back(tree_))->value_, 150);
}

TEST_F(RedBlackTreeTest, PopFrontAndPopBack)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    rbtree_insert(tree_, &data1->node_, data1);
    rbtree_insert(tree_, &data2->node_, data2);
    rbtree_insert(tree_, &data3->node_, data3);

    auto front = (TestData *)rbtree_pop_front(tree_);
    ASSERT_EQ(front->value_, 50);
    ASSERT_EQ(rbtree_count(tree_), 2);

    auto back = (TestData *)rbtree_pop_back(tree_);
    ASSERT_EQ(back->value_, 150);
    ASSERT_EQ(rbtree_count(tree_), 1);

    auto remaining = (TestData *)rbtree_pop_front(tree_);
    ASSERT_EQ(remaining->value_, 100);
    ASSERT_EQ(rbtree_count(tree_), 0);
    ASSERT_TRUE(rbtree_empty(tree_));
}

TEST_F(RedBlackTreeTest, LowerBound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    rbtree_insert(tree_, &data1->node_, data1);
    rbtree_insert(tree_, &data2->node_, data2);
    rbtree_insert(tree_, &data3->node_, data3);
    rbtree_insert(tree_, &data4->node_, data4);
    rbtree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto lb = rbtree_lower_bound(tree_, key);
    ASSERT_NE(lb, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 75);

    key->value_ = 100;
    lb = rbtree_lower_bound(tree_, key);
    ASSERT_NE(lb, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 100);

    key->value_ = 200;
    lb = rbtree_lower_bound(tree_, key);
    ASSERT_EQ(lb, rbtree_nil(tree_));

    delete key;
}

TEST_F(RedBlackTreeTest, UpperBound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    rbtree_insert(tree_, &data1->node_, data1);
    rbtree_insert(tree_, &data2->node_, data2);
    rbtree_insert(tree_, &data3->node_, data3);
    rbtree_insert(tree_, &data4->node_, data4);
    rbtree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto lb = rbtree_upper_bound(tree_, key);
    ASSERT_NE(lb, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 75);

    key->value_ = 100;
    lb = rbtree_upper_bound(tree_, key);
    ASSERT_NE(lb, rbtree_nil(tree_));
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 125);

    key->value_ = 200;
    lb = rbtree_upper_bound(tree_, key);
    ASSERT_EQ(lb, rbtree_nil(tree_));

    delete key;
}

TEST_F(RedBlackTreeTest, RedBlackTreeValidation)
{
    ASSERT_TRUE(rbtree_validate(tree_));

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);
    auto data6 = new TestData(25);
    auto data7 = new TestData(175);

    rbtree_insert(tree_, &data1->node_, data1);
    ASSERT_TRUE(rbtree_validate(tree_));

    rbtree_insert(tree_, &data2->node_, data2);
    ASSERT_TRUE(rbtree_validate(tree_));

    rbtree_insert(tree_, &data3->node_, data3);
    ASSERT_TRUE(rbtree_validate(tree_));

    rbtree_insert(tree_, &data4->node_, data4);
    ASSERT_TRUE(rbtree_validate(tree_));

    rbtree_insert(tree_, &data5->node_, data5);
    ASSERT_TRUE(rbtree_validate(tree_));

    rbtree_insert(tree_, &data6->node_, data6);
    ASSERT_TRUE(rbtree_validate(tree_));

    rbtree_insert(tree_, &data7->node_, data7);
    ASSERT_TRUE(rbtree_validate(tree_));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
