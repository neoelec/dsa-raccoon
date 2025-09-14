#include "gtest/gtest.h"
#include "rcn_c/avltree.h"

struct TestData {
    TestData(int value)
        : value_(value)
    {
    }

    rcn_c::avlnode node_;
    int value_;
};

class AVLTreeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        tree_ = new rcn_c::avltree;
        avltree_init(tree_, _ValueCompare);
    }

    void TearDown() override
    {
        while (!avltree_empty(tree_)) {
            delete (TestData *)avltree_pop_front(tree_);
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

    rcn_c::avltree *tree_;
};

TEST_F(AVLTreeTest, InitAndEmpty)
{
    ASSERT_TRUE(avltree_empty(tree_));
}

TEST_F(AVLTreeTest, InsertAndFind)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);

    ASSERT_FALSE(avltree_empty(tree_));

    auto key = new TestData(100);
    auto found = avltree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 100);

    key->value_ = 50;
    found = avltree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 50);

    key->value_ = 150;
    found = avltree_find(tree_, key);
    ASSERT_NE(found, nullptr);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(((TestData *)found->entry_)->value_, 150);

    key->value_ = 20;
    found = avltree_find(tree_, key);
    ASSERT_EQ(found, nullptr);

    delete key;
}

TEST_F(AVLTreeTest, BeginNextEnd)
{
    ASSERT_EQ(avltree_begin(tree_), nullptr);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);

    rcn_c::avlnode *node = avltree_begin(tree_);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 50);

    node = avltree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 100);

    node = avltree_next(tree_, node);
    ASSERT_NE(node, nullptr);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(((TestData *)node->entry_)->value_, 150);

    node = avltree_next(tree_, node);
    ASSERT_EQ(node, nullptr);
}

TEST_F(AVLTreeTest, Erase)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);

    auto key = new TestData(50);
    auto found = avltree_find(tree_, key);
    ASSERT_NE(found, nullptr);

    rcn_c::avlnode *node_to_erase = (rcn_c::avlnode *)nullptr;

    for (rcn_c::avlnode *node = avltree_begin(tree_);
         node != avltree_end(tree_); node = avltree_next(tree_, node)) {
        if (((TestData *)node->entry_)->value_ == 50) {
            node_to_erase = node;
            break;
        }
    }

    ASSERT_NE(node_to_erase, nullptr);
    ASSERT_NE(node_to_erase, nullptr);

    auto erased = (TestData *)avltree_erase(tree_, node_to_erase);
    ASSERT_EQ(erased, data2);

    found = avltree_find(tree_, key);
    ASSERT_EQ(found, nullptr);

    delete key;
}

TEST_F(AVLTreeTest, Count)
{
    ASSERT_EQ(avltree_size(tree_), 0);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    avltree_insert(tree_, &data1->node_, data1);
    ASSERT_EQ(avltree_size(tree_), 1);
    avltree_insert(tree_, &data2->node_, data2);
    ASSERT_EQ(avltree_size(tree_), 2);
    avltree_insert(tree_, &data3->node_, data3);
    ASSERT_EQ(avltree_size(tree_), 3);
}

TEST_F(AVLTreeTest, FrontAndBack)
{
    ASSERT_EQ(avltree_front(tree_), nullptr);
    ASSERT_EQ(avltree_back(tree_), nullptr);

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);

    ASSERT_EQ(((TestData *)avltree_front(tree_))->value_, 50);
    ASSERT_EQ(((TestData *)avltree_back(tree_))->value_, 150);
}

TEST_F(AVLTreeTest, PopFrontAndPopBack)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);

    auto front = (TestData *)avltree_pop_front(tree_);
    ASSERT_EQ(front->value_, 50);
    ASSERT_EQ(avltree_size(tree_), 2);

    auto back = (TestData *)avltree_pop_back(tree_);
    ASSERT_EQ(back->value_, 150);
    ASSERT_EQ(avltree_size(tree_), 1);

    auto remaining = (TestData *)avltree_pop_front(tree_);
    ASSERT_EQ(remaining->value_, 100);
    ASSERT_EQ(avltree_size(tree_), 0);
    ASSERT_TRUE(avltree_empty(tree_));
}

TEST_F(AVLTreeTest, At)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);

    auto found = (TestData *)avltree_at(tree_, 0);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 50);

    found = (TestData *)avltree_at(tree_, 1);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 100);

    found = (TestData *)avltree_at(tree_, 2);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->value_, 150);

    found = (TestData *)avltree_at(tree_, 3);
    ASSERT_EQ(found, nullptr);
}

TEST_F(AVLTreeTest, Loweavlound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);
    avltree_insert(tree_, &data4->node_, data4);
    avltree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto lb = avltree_lower_bound(tree_, key);
    ASSERT_NE(lb, nullptr);
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 75);

    key->value_ = 100;
    lb = avltree_lower_bound(tree_, key);
    ASSERT_NE(lb, nullptr);
    ASSERT_EQ(((TestData *)lb->entry_)->value_, 100);

    key->value_ = 200;
    lb = avltree_lower_bound(tree_, key);
    ASSERT_EQ(lb, nullptr);

    delete key;
}

TEST_F(AVLTreeTest, Uppeavlound)
{
    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);

    avltree_insert(tree_, &data1->node_, data1);
    avltree_insert(tree_, &data2->node_, data2);
    avltree_insert(tree_, &data3->node_, data3);
    avltree_insert(tree_, &data4->node_, data4);
    avltree_insert(tree_, &data5->node_, data5);

    auto key = new TestData(70);
    auto ub = avltree_upper_bound(tree_, key);
    ASSERT_NE(ub, nullptr);
    ASSERT_EQ(((TestData *)ub->entry_)->value_, 75);

    key->value_ = 100;
    ub = avltree_upper_bound(tree_, key);
    ASSERT_NE(ub, nullptr);
    ASSERT_EQ(((TestData *)ub->entry_)->value_, 125);

    key->value_ = 200;
    ub = avltree_upper_bound(tree_, key);
    ASSERT_EQ(ub, nullptr);

    delete key;
}

TEST_F(AVLTreeTest, AVLTreeValidation)
{
    ASSERT_TRUE(avltree_validate(tree_));

    auto data1 = new TestData(100);
    auto data2 = new TestData(50);
    auto data3 = new TestData(150);
    auto data4 = new TestData(75);
    auto data5 = new TestData(125);
    auto data6 = new TestData(25);
    auto data7 = new TestData(175);

    avltree_insert(tree_, &data1->node_, data1);
    ASSERT_TRUE(avltree_validate(tree_));

    avltree_insert(tree_, &data2->node_, data2);
    ASSERT_TRUE(avltree_validate(tree_));

    avltree_insert(tree_, &data3->node_, data3);
    ASSERT_TRUE(avltree_validate(tree_));

    avltree_insert(tree_, &data4->node_, data4);
    ASSERT_TRUE(avltree_validate(tree_));

    avltree_insert(tree_, &data5->node_, data5);
    ASSERT_TRUE(avltree_validate(tree_));

    avltree_insert(tree_, &data6->node_, data6);
    ASSERT_TRUE(avltree_validate(tree_));

    avltree_insert(tree_, &data7->node_, data7);
    ASSERT_TRUE(avltree_validate(tree_));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
