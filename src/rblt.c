/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include <rcn/rblt.h>

#define __compar(tree, key, in_tree) (tree)->compar(key, in_tree)

static struct rblt_node *__minimum(const struct rblt *tree,
                                   const struct rblt_node *x)
{
    while (x->left != tree->NiL)
        x = x->left;

    return (struct rblt_node *)x;
}

static struct rblt_node *__maximum(const struct rblt *tree,
                                   const struct rblt_node *x)
{
    while (x->right != tree->NiL)
        x = x->right;

    return (struct rblt_node *)x;
}

static struct rblt_node *__search(const struct rblt *tree,
                                  const struct rblt_node *kx,
                                  const struct rblt_node *x)
{
    while (x != tree->NiL) {
        int diff = __compar(tree, kx, x);

        if (diff == 0)
            break;

        x = diff < 0 ? x->left : x->right;
    }

    return (struct rblt_node *)x;
}

static void __rotate_left(struct rblt *tree, struct rblt_node *x)
{
    struct rblt_node *y;

    y = x->right;
    x->right = y->left;

    if (y->left != tree->NiL)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == tree->NiL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void __rotate_right(struct rblt *tree, struct rblt_node *x)
{
    struct rblt_node *y;

    y = x->left;
    x->left = y->right;

    if (y->right != tree->NiL)
        y->right->parent = x;

    y->parent = x->parent;
    if (x->parent == tree->NiL)
        tree->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

static void __fix_up_insertion(struct rblt *tree, struct rblt_node *z)
{
    struct rblt_node *y;

    while (z->parent->color == RBLT_RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;

            if (y->color == RBLT_RED) {
                /* Case 1. */
                z->parent->color = RBLT_BLACK;
                y->color = RBLT_BLACK;
                z->parent->parent->color = RBLT_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    /* Case 2. */
                    z = z->parent;
                    __rotate_left(tree, z);
                }

                /* Case 3. */
                z->parent->color = RBLT_BLACK;
                z->parent->parent->color = RBLT_RED;
                __rotate_right(tree, z->parent->parent);
            }
        } else { /* same as previous, but with "right" and "left" exchanged. */
            y = z->parent->parent->left;

            if (y->color == RBLT_RED) {
                z->parent->color = RBLT_BLACK;
                y->color = RBLT_BLACK;
                z->parent->parent->color = RBLT_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    __rotate_right(tree, z);
                }

                z->parent->color = RBLT_BLACK;
                z->parent->parent->color = RBLT_RED;
                __rotate_left(tree, z->parent->parent);
            }
        }
    }

    tree->root->color = RBLT_BLACK;
}

static void __insert(struct rblt *tree, struct rblt_node *z)
{
    struct rblt_node *y;
    struct rblt_node *x;

    x = tree->root;
    y = tree->NiL;

    while (x != tree->NiL) {
        y = x;
        x = __compar(tree, z, x) < 0 ? x->left : x->right;
    }

    z->parent = y;

    if (y == tree->NiL)
        tree->root = z;
    else if (__compar(tree, z, y) < 0)
        y->left = z;
    else
        y->right = z;

    z->left = tree->NiL;
    z->right = tree->NiL;
    z->color = RBLT_RED;

    __fix_up_insertion(tree, z);
}

static void __transplant(struct rblt *tree, struct rblt_node *u,
                         struct rblt_node *v)
{
    if (u->parent == tree->NiL)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

static void __fix_up_removal(struct rblt *tree, struct rblt_node *x)
{
    struct rblt_node *w;

    while (x != tree->root && x->color == RBLT_BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;

            if (w->color == RBLT_RED) {
                /* Case 1. */
                w->color = RBLT_BLACK;
                x->parent->color = RBLT_RED;
                __rotate_left(tree, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == RBLT_BLACK && w->right->color == RBLT_BLACK) {
                /* Case 2. */
                w->color = RBLT_RED;
                x = x->parent;
            } else {
                if (w->right->color == RBLT_BLACK) {
                    /* Case 3. */
                    w->left->color = RBLT_BLACK;
                    w->color = RBLT_RED;
                    __rotate_right(tree, w);
                    w = x->parent->right;
                }

                /* Case 4. */
                w->color = x->parent->color;
                x->parent->color = RBLT_BLACK;
                w->right->color = RBLT_BLACK;
                __rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else { /* same as previous, but with "right" and "left" exchanged. */
            w = x->parent->left;

            if (w->color == RBLT_RED) {
                w->color = RBLT_BLACK;
                x->parent->color = RBLT_RED;
                __rotate_right(tree, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == RBLT_BLACK && w->left->color == RBLT_BLACK) {
                w->color = RBLT_RED;
                x = x->parent;
            } else {
                if (w->left->color == RBLT_BLACK) {
                    w->right->color = RBLT_BLACK;
                    w->color = RBLT_RED;
                    __rotate_left(tree, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = RBLT_BLACK;
                w->left->color = RBLT_BLACK;
                __rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }

    x->color = RBLT_BLACK;
}

static void __remove(struct rblt *tree, struct rblt_node *z)
{
    struct rblt_node *y;
    struct rblt_node *x;
    enum rblt_color_t y_orignal_color;

    y = z;
    y_orignal_color = y->color;

    if (z->left == tree->NiL) {
        x = z->right;
        __transplant(tree, z, z->right);
    } else if (z->right == tree->NiL) {
        x = z->left;
        __transplant(tree, z, z->left);
    } else {
        y = __minimum(tree, z->right); /* successor */
        y_orignal_color = y->color;
        x = y->right;
        if (y != z->right) {
            __transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        } else
            x->parent = y;

        __transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_orignal_color == RBLT_BLACK)
        __fix_up_removal(tree, x);
}

static struct rblt_node *__next(const struct rblt *tree, struct rblt_node *x)
{
    struct rblt_node *parent;

    if (x->right != tree->NiL) {
        x = x->right;
        while (x->left != tree->NiL)
            x = x->left;

        return x;
    }

    parent = x->parent;
    while ((parent != tree->NiL) && (x == parent->right)) {
        x = parent;
        parent = x->parent;
    }

    return parent;
}

static struct rblt_node *__prev(const struct rblt *tree, struct rblt_node *x)
{
    struct rblt_node *parent;

    if (x->left != tree->NiL) {
        x = x->left;
        while (x->right != tree->NiL)
            x = x->right;

        return x;
    }

    parent = x->parent;
    while ((parent != tree->NiL) && (x == parent->left)) {
        x = parent;
        parent = x->parent;
    }

    return parent;
}

static void __for_each_preorder(const struct rblt *tree, struct rblt_node *x,
                                rblt_do_each_t do_each, void *priv)
{
    if (x == tree->NiL)
        return;

    do_each(x, priv);
    __for_each_preorder(tree, x->left, do_each, priv);
    __for_each_preorder(tree, x->right, do_each, priv);
}

static void __for_each_inorder(const struct rblt *tree, struct rblt_node *x,
                               rblt_do_each_t do_each, void *priv)
{
    if (x == tree->NiL)
        return;

    __for_each_inorder(tree, x->left, do_each, priv);
    do_each(x, priv);
    __for_each_inorder(tree, x->right, do_each, priv);
}

static void __for_each_postorder(const struct rblt *tree, struct rblt_node *x,
                                 rblt_do_each_t do_each, void *priv)
{
    if (x == tree->NiL)
        return;

    __for_each_postorder(tree, x->left, do_each, priv);
    __for_each_postorder(tree, x->right, do_each, priv);
    do_each(x, priv);
}

int rblt_minimum(const struct rblt *tree, struct rblt_node **_x)
{
    if (rblt_is_empty(tree))
        return -ENODATA;

    *_x = __minimum(tree, tree->root);

    return 0;
}

int rblt_maximum(const struct rblt *tree, struct rblt_node **_x)
{
    if (rblt_is_empty(tree))
        return -ENODATA;

    *_x = __maximum(tree, tree->root);

    return 0;
}

int rblt_search(const struct rblt *tree, const struct rblt_node *kx,
                struct rblt_node **_x)
{
    struct rblt_node *x;

    if (rblt_is_empty(tree))
        return -ENODATA;

    x = __search(tree, kx, tree->root);
    if (x == tree->NiL)
        return -ENOENT;

    *_x = x;

    return 0;
}

int rblt_insert(struct rblt *tree, struct rblt_node *z)
{
    const struct rblt_node *x;

    x = __search(tree, z, tree->root);
    if (x != tree->NiL)
        return -EEXIST;

    __insert(tree, z);

    if (tree->root != tree->NiL)
        tree->root->parent = tree->NiL;

    return 0;
}

int rblt_remove(struct rblt *tree, struct rblt_node *z)
{
    const struct rblt_node *x;

    if (rblt_is_empty(tree))
        return -ENODATA;

    x = __search(tree, z, tree->root);
    if (x == tree->NiL)
        return -ENOENT;

    __remove(tree, z);

    z->parent = z;

    if (tree->root != tree->NiL)
        tree->root->parent = tree->NiL;

    return 0;
}

void rblt_forward(struct rblt *tree, rblt_do_each_t do_each, void *priv)
{
    struct rblt_node *x;

    x = __minimum(tree, tree->root);
    while (x != tree->NiL) {
        do_each(x, priv);
        x = __next(tree, x);
    }
}

void rblt_backward(struct rblt *tree, rblt_do_each_t do_each, void *priv)
{
    struct rblt_node *x;

    x = __maximum(tree, tree->root);
    while (x != tree->NiL) {
        do_each(x, priv);
        x = __prev(tree, x);
    }
}

void rblt_preorder(struct rblt *tree, rblt_do_each_t do_each, void *priv)
{
    __for_each_preorder(tree, tree->root, do_each, priv);
}

void rblt_inorder(struct rblt *tree, rblt_do_each_t do_each, void *priv)
{
    __for_each_inorder(tree, tree->root, do_each, priv);
}

void rblt_postorder(struct rblt *tree, rblt_do_each_t do_each, void *priv)
{
    __for_each_postorder(tree, tree->root, do_each, priv);
}
