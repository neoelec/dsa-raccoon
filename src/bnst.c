/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdlib.h>

#include <rcn/bnst.h>

#define __compar(tree, key, in_tree) (tree)->compar(key, in_tree)

static struct bnst_node *__minimum(const struct bnst_node *x)
{
    while (x->left != NULL)
        x = x->left;

    return (struct bnst_node *)x;
}

static struct bnst_node *__maximum(const struct bnst_node *x)
{
    while (x->right != NULL)
        x = x->right;

    return (struct bnst_node *)x;
}

static struct bnst_node *__search(const struct bnst *tree,
                                  const struct bnst_node *kx,
                                  struct bnst_node *x)
{
    while (x != NULL) {
        int diff = __compar(tree, kx, x);

        if (diff == 0)
            break;

        x = diff < 0 ? x->left : x->right;
    }

    return x;
}

static void __insert(struct bnst *tree, struct bnst_node *z)
{
    struct bnst_node *y;
    struct bnst_node *x;

    y = NULL;
    x = tree->root;

    while (x != NULL) {
        y = x;
        x = __compar(tree, z, x) < 0 ? x->left : x->right;
    }

    z->parent = y;

    if (y == NULL)
        tree->root = z;
    else if (__compar(tree, z, y) < 0)
        y->left = z;
    else
        y->right = z;

    z->left = NULL;
    z->right = NULL;
}

static void __transplant(struct bnst *tree, struct bnst_node *u,
                         struct bnst_node *v)
{
    if (u->parent == NULL)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

static void __remove(struct bnst *tree, struct bnst_node *z)
{
    if (z->left == NULL)
        __transplant(tree, z, z->right);
    else if (z->right == NULL)
        __transplant(tree, z, z->left);
    else {
        struct bnst_node *y;

        y = __minimum(z->right);
        if (y != z->right) {
            __transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        __transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
}

static struct bnst_node *__next(const struct bnst *tree, struct bnst_node *x)
{
    struct bnst_node *parent;

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

static struct bnst_node *__prev(const struct bnst *tree, struct bnst_node *x)
{
    struct bnst_node *parent;

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

static void __for_each_preorder(struct bnst_node *x, bnst_do_each_t do_each,
                                void *priv)
{
    if (x == NULL)
        return;

    do_each(x, priv);
    __for_each_preorder(x->left, do_each, priv);
    __for_each_preorder(x->right, do_each, priv);
}

static void __for_each_inorder(struct bnst_node *x, bnst_do_each_t do_each,
                               void *priv)
{
    if (x == NULL)
        return;

    __for_each_inorder(x->left, do_each, priv);
    do_each(x, priv);
    __for_each_inorder(x->right, do_each, priv);
}

static void __for_each_postorder(struct bnst_node *x, bnst_do_each_t do_each,
                                 void *priv)
{
    if (x == NULL)
        return;

    __for_each_postorder(x->left, do_each, priv);
    __for_each_postorder(x->right, do_each, priv);
    do_each(x, priv);
}

int bnst_minimum(const struct bnst *tree, struct bnst_node **_x)
{
    if (bnst_is_empty(tree))
        return -ENODATA;

    *_x = __minimum(tree->root);

    return 0;
}
int bnst_maximum(const struct bnst *tree, struct bnst_node **_x)
{
    if (bnst_is_empty(tree))
        return -ENODATA;

    *_x = __maximum(tree->root);

    return 0;
}

int bnst_search(const struct bnst *tree, const struct bnst_node *kx,
                struct bnst_node **_x)
{
    struct bnst_node *x;

    if (bnst_is_empty(tree))
        return -ENODATA;

    x = __search(tree, kx, tree->root);
    if (x == NULL)
        return -ENOENT;

    *_x = x;

    return 0;
}

int bnst_insert(struct bnst *tree, struct bnst_node *x)
{
    if (__search(tree, x, tree->root))
        return -EEXIST;

    __insert(tree, x);

    if (tree->root != NULL)
        tree->root->parent = NULL;

    return 0;
}

int bnst_remove(struct bnst *tree, struct bnst_node *z)
{
    struct bnst_node *x;

    if (bnst_is_empty(tree))
        return -ENODATA;

    x = __search(tree, z, tree->root);
    if (x == NULL)
        return -ENOENT;

    __remove(tree, z);

    z->parent = z;

    if (tree->root != NULL)
        tree->root->parent = NULL;

    return 0;
}

void bnst_forward(struct bnst *tree, bnst_do_each_t do_each, void *priv)
{
    struct bnst_node *x;

    x = __minimum(tree->root);
    while (x != NULL) {
        do_each(x, priv);
        x = __next(tree, x);
    }
}

void bnst_backward(struct bnst *tree, bnst_do_each_t do_each, void *priv)
{
    struct bnst_node *x;

    x = __maximum(tree->root);
    while (x != NULL) {
        do_each(x, priv);
        x = __prev(tree, x);
    }
}

void bnst_preorder(struct bnst *tree, bnst_do_each_t do_each, void *priv)
{
    __for_each_preorder(tree->root, do_each, priv);
}

void bnst_inorder(struct bnst *tree, bnst_do_each_t do_each, void *priv)
{
    __for_each_inorder(tree->root, do_each, priv);
}

void bnst_postorder(struct bnst *tree, bnst_do_each_t do_each, void *priv)
{
    __for_each_postorder(tree->root, do_each, priv);
}
