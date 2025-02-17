/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdlib.h>

#include <rcn/avlt.h>

#include "utility.h"

#define __compar(tree, key, in_tree) (tree)->compar(key, in_tree)

static struct avlt_node *__minimum(const struct avlt_node *x)
{
    while (x->left != NULL)
        x = x->left;

    return (struct avlt_node *)x;
}

static struct avlt_node *__maximum(const struct avlt_node *x)
{
    while (x->right != NULL)
        x = x->right;

    return (struct avlt_node *)x;
}

static struct avlt_node *__search(const struct avlt *tree,
                                  const struct avlt_node *kx,
                                  const struct avlt_node *x)
{
    while (x != NULL) {
        int diff;

        diff = __compar(tree, kx, x);
        if (diff == 0)
            break;

        x = diff < 0 ? x->left : x->right;
    }

    return (struct avlt_node *)x;
}

static void __set_left_child(struct avlt_node *x, struct avlt_node *c)
{
    x->left = c;

    if (c != NULL)
        c->parent = x;
}

static void __set_right_child(struct avlt_node *x, struct avlt_node *c)
{
    x->right = c;

    if (c != NULL)
        c->parent = x;
}

static ssize_t __height(struct avlt_node *x)
{
    return x != NULL ? x->height : 0;
}

static struct avlt_node *__rotate_left(struct avlt_node *x)
{
    struct avlt_node *y = x->right;
    struct avlt_node *t2 = y->left;

    __set_left_child(y, x);
    __set_right_child(x, t2);

    x->height = MAX(__height(x->left), __height(x->right)) + 1;
    y->height = MAX(__height(y->left), __height(y->right)) + 1;

    return y;
}

static struct avlt_node *__rotate_right(struct avlt_node *y)
{
    struct avlt_node *x = y->left;
    struct avlt_node *t2 = x->right;

    __set_right_child(x, y);
    __set_left_child(y, t2);

    y->height = MAX(__height(y->left), __height(y->right)) + 1;
    x->height = MAX(__height(x->left), __height(x->right)) + 1;

    return x;
}

static ssize_t __balance_factor(struct avlt_node *x)
{
    return x != NULL ? __height(x->left) - __height(x->right) : 0;
}

static struct avlt_node *__rebalance_insertion(struct avlt *tree,
                                               struct avlt_node *x,
                                               struct avlt_node *z)
{
    ssize_t bf;

    x->height = MAX(__height(x->left), __height(x->right)) + 1;

    bf = __balance_factor(x);
    if (bf > 1 && __compar(tree, z, x->left) < 0) {
        /* left left case */
        return __rotate_right(x);
    } else if (bf < -1 && __compar(tree, z, x->right) > 0) {
        /* right right case */
        return __rotate_left(x);
    } else if (bf > 1 && __compar(tree, z, x->left) > 0) {
        /* left right case */
        __set_left_child(x, __rotate_left(x->left));
        return __rotate_right(x);
    } else if (bf < -1 && __compar(tree, z, x->right) < 0) {
        /* right left case */
        __set_right_child(x, __rotate_right(x->right));
        return __rotate_left(x);
    }

    return x;
}

static struct avlt_node *__insert(struct avlt *tree, struct avlt_node *x,
                                  struct avlt_node *z)
{
    if (x == NULL)
        return z;
    else if (__compar(tree, z, x) < 0)
        __set_left_child(x, __insert(tree, x->left, z));
    else if (__compar(tree, z, x) > 0)
        __set_right_child(x, __insert(tree, x->right, z));
    else
        return x;

    return __rebalance_insertion(tree, x, z);
}

static struct avlt_node *__exchange(const struct avlt_node *a,
                                    struct avlt_node *b)
{
    __set_left_child(b, a->left);
    __set_right_child(b, a->right);

    b->height = a->height;
    b->parent = a->parent;

    if (a->parent == NULL)
        return b;
    else if (a->parent->left == a)
        a->parent->left = b;
    else
        a->parent->right = b;

    return b;
}

static struct avlt_node *__rebalance_removal(struct avlt *tree,
                                             struct avlt_node *x)
{
    ssize_t bf;

    if (x == NULL)
        return NULL;

    x->height = MAX(__height(x->left), __height(x->right)) + 1;

    bf = __balance_factor(x);
    if (bf > 1 && __balance_factor(x->left) >= 0) {
        return __rotate_right(x);
    } else if (bf > 1 && __balance_factor(x->left) < 0) {
        __set_left_child(x, __rotate_left(x->left));
        return __rotate_right(x);
    } else if (bf < -1 && __balance_factor(x->right) <= 0) {
        return __rotate_left(x);
    } else if (bf < -1 && __balance_factor(x->right) > 0) {
        __set_right_child(x, __rotate_right(x->right));
        return __rotate_left(x);
    }

    return x;
}

static struct avlt_node *__remove(struct avlt *tree, struct avlt_node *x,
                                  struct avlt_node *z)
{
    struct avlt_node *y;

    if (__compar(tree, z, x) < 0) {
        __set_left_child(x, __remove(tree, x->left, z));
    } else if (__compar(tree, z, x) > 0) {
        __set_right_child(x, __remove(tree, x->right, z));
    } else {
        if (x->left == NULL) {
            x = NULL;
        } else if (x->right == NULL) {
            x = x->left;
        } else {
            y = __minimum(x->right);
            x->right = __remove(tree, x->right, y);
            x = __exchange(x, y);
        }
    }

    return __rebalance_removal(tree, x);
}

static struct avlt_node *__next(struct avlt_node *x)
{
    struct avlt_node *parent;

    if (x->right != NULL) {
        x = x->right;
        while (x->left != NULL)
            x = x->left;

        return x;
    }

    parent = x->parent;
    while ((parent != NULL) && (x == parent->right)) {
        x = parent;
        parent = x->parent;
    }

    return parent;
}

static struct avlt_node *__prev(struct avlt_node *x)
{
    struct avlt_node *parent;

    if (x->left != NULL) {
        x = x->left;
        while (x->right != NULL)
            x = x->right;

        return x;
    }

    parent = x->parent;
    while ((parent != NULL) && (x == parent->left)) {
        x = parent;
        parent = x->parent;
    }

    return parent;
}

static void __for_each_preorder(struct avlt_node *x, avlt_do_each_t do_each,
                                void *priv)
{
    if (x == NULL)
        return;

    do_each(x, priv);
    __for_each_preorder(x->left, do_each, priv);
    __for_each_preorder(x->right, do_each, priv);
}

static void __for_each_inorder(struct avlt_node *x, avlt_do_each_t do_each,
                               void *priv)
{
    if (x == NULL)
        return;

    __for_each_inorder(x->left, do_each, priv);
    do_each(x, priv);
    __for_each_inorder(x->right, do_each, priv);
}

static void __for_each_postorder(struct avlt_node *x, avlt_do_each_t do_each,
                                 void *priv)
{
    if (x == NULL)
        return;

    __for_each_postorder(x->left, do_each, priv);
    __for_each_postorder(x->right, do_each, priv);
    do_each(x, priv);
}

int avlt_minimum(const struct avlt *tree, struct avlt_node **_x)
{
    if (avlt_is_empty(tree))
        return -ENODATA;

    *_x = __minimum(tree->root);

    return 0;
}

int avlt_maximum(const struct avlt *tree, struct avlt_node **_x)
{
    if (avlt_is_empty(tree))
        return -ENODATA;

    *_x = __maximum(tree->root);

    return 0;
}

int avlt_search(const struct avlt *tree, const struct avlt_node *kx,
                struct avlt_node **_x)
{
    struct avlt_node *x;

    if (avlt_is_empty(tree))
        return -ENODATA;

    x = __search(tree, kx, tree->root);
    if (x == NULL)
        return -ENOENT;

    *_x = x;

    return 0;
}

int avlt_insert(struct avlt *tree, struct avlt_node *x)
{
    if (__search(tree, x, tree->root))
        return -EEXIST;

    x->left = NULL;
    x->right = NULL;
    x->height = 0;

    tree->root = __insert(tree, tree->root, x);
    if (tree->root != NULL)
        tree->root->parent = NULL;

    return 0;
}

int avlt_remove(struct avlt *tree, struct avlt_node *z)
{
    struct avlt_node *x;

    if (avlt_is_empty(tree))
        return -ENODATA;

    x = __search(tree, z, tree->root);
    if (x == NULL)
        return -ENOENT;

    tree->root = __remove(tree, tree->root, z);

    if (tree->root != NULL)
        tree->root->parent = NULL;

    return 0;
}

void avlt_forward(struct avlt *tree, avlt_do_each_t do_each, void *priv)
{
    struct avlt_node *x;

    if (avlt_is_empty(tree))
        return;

    x = __minimum(tree->root);
    while (x != NULL) {
        do_each(x, priv);
        x = __next(x);
    }
}

void avlt_backward(struct avlt *tree, avlt_do_each_t do_each, void *priv)
{
    struct avlt_node *x;

    if (avlt_is_empty(tree))
        return;

    x = __maximum(tree->root);
    while (x != NULL) {
        do_each(x, priv);
        x = __prev(x);
    }
}

void avlt_preorder(struct avlt *tree, avlt_do_each_t do_each, void *priv)
{
    __for_each_preorder(tree->root, do_each, priv);
}

void avlt_inorder(struct avlt *tree, avlt_do_each_t do_each, void *priv)
{
    __for_each_inorder(tree->root, do_each, priv);
}

void avlt_postorder(struct avlt *tree, avlt_do_each_t do_each, void *priv)
{
    __for_each_postorder(tree->root, do_each, priv);
}
