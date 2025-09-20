#include "gtest/gtest.h"
#include "rcn_c/bstree.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_c::bsnode node_;
    int value_;
};

class BinarySearchTreeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        tree_ = new rcn_c::bstree;
        bstree_init(tree_, _ValueCompare);
    }

    void TearDown() override
    {
        while (!bstree_empty(tree_)) {
            delete (TestData *)bstree_pop_front(tree_);
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

    rcn_c::bstree *tree_;
};

TEST_F(BinarySearchTreeTest, InitAndEmpty)
{
    ASSERT_TRUE(bstree_empty(tree_));
}

TEST_F(BinarySearchTreeTest, InsertAndFind)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);

    ASSERT_FALSE(bstree_empty(tree_));

    auto key = new TestData(100);
    auto found = bstree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 100);

    key->value_ = 50;
    found = bstree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 50);

    key->value_ = 150;
    found = bstree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 150);

    key->value_ = 20;
    found = bstree_find(tree_, key);
    ASSERT_EQ(found, nullptr);

    delete key;
}

TEST_F(BinarySearchTreeTest, BeginNextEnd)
{
    ASSERT_EQ(bstree_begin(tree_), nullptr);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);

    rcn_c::bsnode *node = bstree_begin(tree_);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 50);

    node = bstree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 100);

    node = bstree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 150);

    node = bstree_next(tree_, node);
    ASSERT_EQ(node, nullptr);
}

TEST_F(BinarySearchTreeTest, Erase)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);

    auto key = new TestData(50);
    auto found = bstree_find(tree_, key);
    ASSERT_NE(found, nullptr);

    rcn_c::bsnode *node_to_erase = (rcn_c::bsnode *)nullptr;

    for (rcn_c::bsnode *node = bstree_begin(tree_); node != bstree_end(tree_);
         node = bstree_next(tree_, node)) {
        if (((TestData *)node->entry_)->value_ == 50) {
            node_to_erase = node;
            break;
        }
    }

    ASSERT_NE(node_to_erase, nullptr);

    auto erased = (TestData *)bstree_erase(tree_, node_to_erase);
    ASSERT_EQ(erased, data2);

    found = bstree_find(tree_, key);
    ASSERT_EQ(found, nullptr);

    delete key;
}

TEST_F(BinarySearchTreeTest, Count)
{
    ASSERT_EQ(bstree_size(tree_), 0);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    bstree_insert(tree_, &data1->node_, data1);
    ASSERT_EQ(bstree_size(tree_), 1);
    bstree_insert(tree_, &data2->node_, data2);
    ASSERT_EQ(bstree_size(tree_), 2);
    bstree_insert(tree_, &data3->node_, data3);
    ASSERT_EQ(bstree_size(tree_), 3);
}

TEST_F(BinarySearchTreeTest, FrontAndBack)
{
    ASSERT_EQ(bstree_front(tree_), nullptr);
    ASSERT_EQ(bstree_back(tree_), nullptr);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);

    ASSERT_EQ(((TestData *)bstree_front(tree_))->value_, 50);
    ASSERT_EQ(((TestData *)bstree_back(tree_))->value_, 150);
}

TEST_F(BinarySearchTreeTest, PopFrontAndPopBack)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);

    auto front = (TestData *)bstree_pop_front(tree_);
    ASSERT_EQ(front->value_, 50);
    ASSERT_EQ(bstree_size(tree_), 2);

    auto back = (TestData *)bstree_pop_back(tree_);
    ASSERT_EQ(back->value_, 150);
    ASSERT_EQ(bstree_size(tree_), 1);

    auto remaining = (TestData *)bstree_pop_front(tree_);
    ASSERT_EQ(remaining->value_, 100);
    ASSERT_EQ(bstree_size(tree_), 0);
    ASSERT_TRUE(bstree_empty(tree_));
}

TEST_F(BinarySearchTreeTest, At)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);

    auto found = (TestData *)bstree_at(tree_, 0);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 50);

    found = (TestData *)bstree_at(tree_, 1);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 100);

    found = (TestData *)bstree_at(tree_, 2);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 150);

    found = (TestData *)bstree_at(tree_, 3);
    ASSERT_EQ(found, nullptr);
}

TEST_F(BinarySearchTreeTest, LowerBound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);
    bstree_insert(tree_, &data4->node_, data4);
    bstree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto lb = bstree_lower_bound(tree_, key);
    ASSERT_NE(lb, nullptr);
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 75);

    key->value_ = 100;
    lb = bstree_lower_bound(tree_, key);
    ASSERT_NE(lb, nullptr);
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 100);

    key->value_ = 200;
    lb = bstree_lower_bound(tree_, key);
    ASSERT_EQ(lb, nullptr);

    delete key;
}

TEST_F(BinarySearchTreeTest, UpperBound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    bstree_insert(tree_, &data1->node_, data1);
    bstree_insert(tree_, &data2->node_, data2);
    bstree_insert(tree_, &data3->node_, data3);
    bstree_insert(tree_, &data4->node_, data4);
    bstree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto ub = bstree_upper_bound(tree_, key);
    ASSERT_NE(ub, nullptr);
    ASSERT_EQ(((TestData *)ub->entry_)->value_, 75);

    key->value_ = 100;
    ub = bstree_upper_bound(tree_, key);
    ASSERT_NE(ub, nullptr);
    ASSERT_EQ(((TestData *)ub->entry_)->value_, 125);

    key->value_ = 200;
    ub = bstree_upper_bound(tree_, key);
    ASSERT_EQ(ub, nullptr);

    delete key;
}

TEST_F(BinarySearchTreeTest, BinarySearchTreeValidation)
{
    ASSERT_TRUE(bstree_validate(tree_));

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);
    auto data6 = new TestData(25);
    auto data7 = new TestData(175);

    bstree_insert(tree_, &data1->node_, data1);
    ASSERT_TRUE(bstree_validate(tree_));

    bstree_insert(tree_, &data2->node_, data2);
    ASSERT_TRUE(bstree_validate(tree_));

    bstree_insert(tree_, &data3->node_, data3);
    ASSERT_TRUE(bstree_validate(tree_));

    bstree_insert(tree_, &data4->node_, data4);
    ASSERT_TRUE(bstree_validate(tree_));

    bstree_insert(tree_, &data5->node_, data5);
    ASSERT_TRUE(bstree_validate(tree_));

    bstree_insert(tree_, &data6->node_, data6);
    ASSERT_TRUE(bstree_validate(tree_));

    bstree_insert(tree_, &data7->node_, data7);
    ASSERT_TRUE(bstree_validate(tree_));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
