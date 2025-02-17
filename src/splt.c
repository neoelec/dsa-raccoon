/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdlib.h>

#include <rcn/splt.h>

#define __compar(tree, key, in_tree) (tree)->compar(key, in_tree)

static struct splt_node *__minimum(const struct splt_node *x)
{
    while (x->left != NULL)
        x = x->left;

    return (struct splt_node *)x;
}

static struct splt_node *__maximum(const struct splt_node *x)
{
    while (x->right != NULL)
        x = x->right;

    return (struct splt_node *)x;
}

static struct splt_node *__search(const struct splt *tree,
                                  const struct splt_node *kx,
                                  const struct splt_node *x)
{
    while (x != NULL) {
        int diff = __compar(tree, kx, x);

        if (diff == 0)
            break;

        x = diff < 0 ? x->left : x->right;
    }

    return (struct splt_node *)x;
}

static void __rotate_left(struct splt *tree, struct splt_node *x)
{
    struct splt_node *y;

    y = x->right;
    if (y != NULL) {
        x->right = y->left;
        if (y->left != NULL)
            y->left->parent = x;

        y->parent = x->parent;
    }

    if (x->parent == NULL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    if (y != NULL)
        y->left = x;

    x->parent = y;
}

static void __rotate_right(struct splt *tree, struct splt_node *x)
{
    struct splt_node *y;

    y = x->left;
    if (y != NULL) {
        x->left = y->right;
        if (y->right != NULL)
            y->right->parent = x;

        y->parent = x->parent;
    }

    if (x->parent == NULL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    if (y != NULL)
        y->right = x;

    x->parent = y;
}

static void __splay(struct splt *tree, struct splt_node *x)
{
    while (x->parent != NULL) {
        struct splt_node *gp = x->parent->parent;

        if (gp == NULL) {
            if (x->parent->left == x)
                __rotate_right(tree, x->parent);
            else
                __rotate_left(tree, x->parent);
        } else if (x->parent->left == x && gp->left == x->parent) {
            __rotate_right(tree, gp);
            __rotate_right(tree, x->parent);
        } else if (x->parent->right == x && gp->right == x->parent) {
            __rotate_left(tree, gp);
            __rotate_left(tree, x->parent);
        } else if (x->parent->left == x && gp->right == x->parent) {
            __rotate_right(tree, x->parent);
            __rotate_left(tree, x->parent);
        } else {
            __rotate_left(tree, x->parent);
            __rotate_right(tree, x->parent);
        }
    }
}

static void __insert(struct splt *tree, struct splt_node *z)
{
    struct splt_node *y;
    struct splt_node *x;

    x = tree->root;
    y = NULL;

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

    __splay(tree, z);
}

static void __transplant(struct splt *tree, struct splt_node *u,
                         struct splt_node *v)
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

static void __remove(struct splt *tree, struct splt_node *z)
{
    struct splt_node *y;

    __splay(tree, z);

    if (z->left == NULL)
        __transplant(tree, z, z->right);
    else if (z->right == NULL)
        __transplant(tree, z, z->left);
    else {
        y = __minimum(z->right);
        if (y->parent != z) {
            __transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        __transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
}

static struct splt_node *__next(const struct splt *tree, struct splt_node *x)
{
    struct splt_node *parent;

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

static struct splt_node *__prev(const struct splt *tree, struct splt_node *x)
{
    struct splt_node *parent;

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

static void __for_each_preorder(const struct splt *tree, struct splt_node *x,
                                splt_do_each_t do_each, void *priv)
{
    if (x == NULL)
        return;

    do_each(x, priv);
    __for_each_preorder(tree, x->left, do_each, priv);
    __for_each_preorder(tree, x->right, do_each, priv);
}

static void __for_each_inorder(const struct splt *tree, struct splt_node *x,
                               splt_do_each_t do_each, void *priv)
{
    if (x == NULL)
        return;

    __for_each_inorder(tree, x->left, do_each, priv);
    do_each(x, priv);
    __for_each_inorder(tree, x->right, do_each, priv);
}

static void __for_each_postorder(const struct splt *tree, struct splt_node *x,
                                 splt_do_each_t do_each, void *priv)
{
    if (x == NULL)
        return;

    __for_each_postorder(tree, x->left, do_each, priv);
    __for_each_postorder(tree, x->right, do_each, priv);
    do_each(x, priv);
}

int splt_minimum(const struct splt *tree, struct splt_node **_x)
{
    if (splt_is_empty(tree))
        return -ENODATA;

    *_x = __minimum(tree->root);

    return 0;
}

int splt_maximum(const struct splt *tree, struct splt_node **_x)
{
    if (splt_is_empty(tree))
        return -ENODATA;

    *_x = __maximum(tree->root);

    return 0;
}

int splt_search(const struct splt *tree, const struct splt_node *kx,
                struct splt_node **_x)
{
    struct splt_node *x;

    if (splt_is_empty(tree))
        return -ENODATA;

    x = __search(tree, kx, tree->root);
    if (x == NULL)
        return -ENOENT;

    *_x = x;

    return 0;
}

int splt_insert(struct splt *tree, struct splt_node *z)
{
    if (__search(tree, z, tree->root))
        return -EEXIST;

    __insert(tree, z);

    if (tree->root != NULL)
        tree->root->parent = NULL;

    return 0;
}

int splt_remove(struct splt *tree, struct splt_node *z)
{
    struct splt_node *x;

    if (splt_is_empty(tree))
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

void splt_forward(struct splt *tree, splt_do_each_t do_each, void *priv)
{
    struct splt_node *x;

    x = __minimum(tree->root);
    while (x != NULL) {
        do_each(x, priv);
        x = __next(tree, x);
    }
}

void splt_backward(struct splt *tree, splt_do_each_t do_each, void *priv)
{
    struct splt_node *x;

    x = __maximum(tree->root);
    while (x != NULL) {
        do_each(x, priv);
        x = __prev(tree, x);
    }
}

void splt_preorder(struct splt *tree, splt_do_each_t do_each, void *priv)
{
    __for_each_preorder(tree, tree->root, do_each, priv);
}

void splt_inorder(struct splt *tree, splt_do_each_t do_each, void *priv)
{
    __for_each_inorder(tree, tree->root, do_each, priv);
}

void splt_postorder(struct splt *tree, splt_do_each_t do_each, void *priv)
{
    __for_each_postorder(tree, tree->root, do_each, priv);
}
