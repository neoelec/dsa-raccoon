/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <rcn/btre.h>

#define __compar(tree, key, in_tree) (tree)->compar(key, in_tree)

struct btre_node;

struct btre_slot {
    struct btre_node *child;
    void *entry;
};

struct btre_node {
    size_t count;
    struct btre_slot slot[];
};

#define BTRE_NODE_BYTES(t) \
    (sizeof(struct btre_node) + (2 * (t)) * sizeof(struct btre_slot))

#define M(tree) (2 * (tree)->t - 1)

static bool __is_leaf(const struct btre_node *x)
{
    return !x->slot[0].child;
}

static struct btre_node *__alloc_node(const struct btre *tree)
{
    struct btre_node *x;

    x = calloc(BTRE_NODE_BYTES(tree->t), 1);
    if (x == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for node\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    return x;
}

static size_t __find_index(const struct btre *tree, const struct btre_node *x,
                           const void *ke)
{
    const struct btre_slot *slot = x->slot;
    size_t idx = 0;

    while (idx < x->count && __compar(tree, ke, slot[idx].entry) > 0)
        idx++;

    return idx;
}

static void *__get_preceding(const struct btre_node *x, size_t idx)
{
    const struct btre_slot *slot = x->slot;
    const struct btre_node *y = slot[idx].child;

    while (!__is_leaf(y))
        y = y->slot[y->count].child;

    return (void *)y->slot[y->count - 1].entry;
}

static void *__get_succeeding(const struct btre_node *x, size_t idx)
{
    const struct btre_slot *slot = x->slot;
    const struct btre_node *y = slot[idx + 1].child;

    while (!__is_leaf(y))
        y = y->slot[0].child;

    return (void *)y->slot[0].entry;
}

static struct btre_node *__minimum(const struct btre_node *x)
{
    while (!__is_leaf(x))
        x = x->slot[0].child;

    return (struct btre_node *)x;
}

static struct btre_node *__maximum(const struct btre_node *x)
{
    while (!__is_leaf(x))
        x = x->slot[x->count].child;

    return (struct btre_node *)x;
}

static struct btre_node *__search(const struct btre *tree,
                                  const struct btre_node *x, const void *ke,
                                  size_t *idx)
{
    const struct btre_slot *slot = x->slot;
    size_t i = 0;

    while (i < x->count && __compar(tree, ke, slot[i].entry) > 0)
        i++;

    if (i < x->count && __compar(tree, ke, slot[i].entry) == 0) {
        *idx = i;
        return (struct btre_node *)x;
    }

    if (__is_leaf(x))
        return NULL;

    return __search(tree, slot[i].child, ke, idx);
}

static void __split_child(struct btre *tree, struct btre_node *x, ssize_t idx,
                          struct btre_node *y)
{
    const size_t t = tree->t;
    struct btre_slot *slot = x->slot;
    struct btre_node *z = __alloc_node(tree);
    ssize_t i;

    z->count = t - 1;

    for (i = 0; i < (ssize_t)t - 1; i++)
        z->slot[i].entry = y->slot[i + t].entry;

    if (!__is_leaf(y)) {
        for (i = 0; i < (ssize_t)t; i++)
            z->slot[i].child = y->slot[i + t].child;
    }

    y->count = t - 1;

    for (i = x->count; i >= idx + 1; i--)
        slot[i + 1].child = slot[i].child;

    slot[idx + 1].child = z;

    for (i = x->count - 1; i >= idx; i--)
        slot[i + 1].entry = slot[i].entry;

    slot[idx].entry = y->slot[t - 1].entry;

    x->count = x->count + 1;
}

static void __insert_non_full(struct btre *tree, struct btre_node *x, void *e)
{
    struct btre_slot *slot = x->slot;
    ssize_t i = x->count - 1;

    if (__is_leaf(x)) {
        while (i >= 0 && __compar(tree, slot[i].entry, e) > 0) {
            slot[i + 1].entry = slot[i].entry;
            i--;
        }

        slot[i + 1].entry = e;
        x->count = x->count + 1;
    } else {
        while (i >= 0 && __compar(tree, slot[i].entry, e) > 0)
            i--;

        if (slot[i + 1].child->count == M(tree)) {
            __split_child(tree, x, i + 1, slot[i + 1].child);

            if (__compar(tree, slot[i + 1].entry, e) < 0)
                i++;
        }

        __insert_non_full(tree, slot[i + 1].child, e);
    }
}

static int __insert_for_empty(struct btre *tree, void *e)
{
    struct btre_node *root = __alloc_node(tree);

    root->slot[0].entry = e;
    root->count = 1;

    tree->root = root;

    return 0;
}

static int __insert_for_non_empty(struct btre *tree, void *e)
{
    struct btre_node *root;
    size_t idx;

    if (__search(tree, tree->root, e, &idx))
        return -EEXIST;

    root = tree->root;

    if (root->count == M(tree)) {
        struct btre_node *x = __alloc_node(tree);
        size_t i;

        x->slot[0].child = root;

        __split_child(tree, x, 0, root);

        i = 0;
        if (__compar(tree, x->slot[0].entry, e) < 0)
            i++;

        __insert_non_full(tree, x->slot[i].child, e);

        tree->root = x;
    } else
        __insert_non_full(tree, root, e);

    return 0;
}

static void __merge_children(struct btre *tree, struct btre_node *x, size_t idx)
{
    const size_t t = tree->t;
    struct btre_slot *slot = x->slot;
    struct btre_node *y = slot[idx].child;
    struct btre_node *z = slot[idx + 1].child;
    size_t i;

    y->slot[t - 1].entry = slot[idx].entry;

    for (i = 0; i < z->count; i++)
        y->slot[i + t].entry = z->slot[i].entry;

    if (!__is_leaf(y)) {
        for (i = 0; i <= z->count; i++)
            y->slot[i + t].child = z->slot[i].child;
    }

    for (i = idx + 1; i < x->count; i++)
        slot[i - 1].entry = slot[i].entry;

    for (i = idx + 2; i <= x->count; i++)
        slot[i - 1].child = slot[i].child;

    y->count += z->count + 1;
    x->count--;

    free(z);
}

static void __borrow_from_prev(struct btre *tree, struct btre_node *x,
                               size_t idx)
{
    struct btre_slot *slot = x->slot;
    struct btre_node *y = slot[idx].child;
    struct btre_node *z = slot[idx - 1].child;
    ssize_t i;

    for (i = y->count - 1; i >= 0; i--)
        y->slot[i + 1].entry = y->slot[i].entry;

    if (!__is_leaf(y)) {
        for (i = y->count; i >= 0; i--)
            y->slot[i + 1].child = y->slot[i].child;
    }

    y->slot[0].entry = slot[idx - 1].entry;

    if (!__is_leaf(y))
        y->slot[0].child = z->slot[z->count].child;

    slot[idx - 1].entry = z->slot[z->count - 1].entry;

    y->count += 1;
    z->count -= 1;
}

static void __borrow_from_next(struct btre *tree, struct btre_node *x,
                               size_t idx)
{
    struct btre_slot *slot = x->slot;
    struct btre_node *y = slot[idx].child;
    struct btre_node *z = slot[idx + 1].child;
    size_t i;

    y->slot[(y->count)].entry = slot[idx].entry;

    if (!__is_leaf(y))
        y->slot[(y->count) + 1].child = z->slot[0].child;

    slot[idx].entry = z->slot[0].entry;

    for (i = 1; i < z->count; i++)
        z->slot[i - 1].entry = z->slot[i].entry;

    if (!__is_leaf(z)) {
        for (i = 1; i <= z->count; i++)
            z->slot[i - 1].child = z->slot[i].child;
    }

    y->count += 1;
    z->count -= 1;
}

static void __fill_from_children(struct btre *tree, struct btre_node *x,
                                 size_t idx)
{
    struct btre_slot *slot = x->slot;

    if (idx != 0 && slot[idx - 1].child->count >= tree->t)
        __borrow_from_prev(tree, x, idx);
    else if (idx != x->count && slot[idx + 1].child->count >= tree->t)
        __borrow_from_next(tree, x, idx);
    else {
        if (idx != x->count)
            __merge_children(tree, x, idx);
        else
            __merge_children(tree, x, idx - 1);
    }
}

static void __remove_from_leaf(struct btre *tree, struct btre_node *x,
                               size_t idx);
static void __remove_from_non_leaf(struct btre *tree, struct btre_node *x,
                                   size_t idx);

static void __remove(struct btre *tree, struct btre_node *x, const void *e)
{
    struct btre_slot *slot = x->slot;
    size_t idx;

    idx = __find_index(tree, x, e);
    if (idx < x->count && __compar(tree, slot[idx].entry, e) == 0) {
        if (__is_leaf(x))
            __remove_from_leaf(tree, x, idx);
        else
            __remove_from_non_leaf(tree, x, idx);
    } else {
        bool flag;

        if (__is_leaf(x))
            return;

        flag = idx == x->count ? true : false;

        if (slot[idx].child->count < tree->t)
            __fill_from_children(tree, x, idx);

        if (flag && idx > x->count)
            __remove(tree, slot[idx - 1].child, e);
        else
            __remove(tree, slot[idx].child, e);
    }
}

static void __remove_from_leaf(struct btre *tree, struct btre_node *x,
                               size_t idx)
{
    struct btre_slot *slot = x->slot;
    size_t i;

    for (i = idx + 1; i < x->count; i++)
        slot[i - 1].entry = slot[i].entry;

    x->count--;
}

static void __remove_from_non_leaf(struct btre *tree, struct btre_node *x,
                                   size_t idx)
{
    struct btre_slot *slot = x->slot;

    if (slot[idx].child->count >= tree->t) {
        void *pred;

        pred = __get_preceding(x, idx);
        slot[idx].entry = pred;
        __remove(tree, slot[idx].child, pred);
    } else if (slot[idx + 1].child->count >= tree->t) {
        void *succ;

        succ = __get_succeeding(x, idx);
        slot[idx].entry = succ;
        __remove(tree, slot[idx + 1].child, succ);
    } else {
        void *del = slot[idx].entry;

        __merge_children(tree, x, idx);
        __remove(tree, slot[idx].child, del);
    }
}

static void __forward(struct btre_node *x, do_each_t do_each, void *priv)
{
    struct btre_slot *slot = x->slot;
    size_t i;

    for (i = 0; i < x->count; i++) {
        if (!__is_leaf(x))
            __forward(slot[i].child, do_each, priv);

        do_each(slot[i].entry, priv);
    }

    if (!__is_leaf(x))
        __forward(slot[i].child, do_each, priv);
}

static void __backward(struct btre_node *x, do_each_t do_each, void *priv)
{
    struct btre_slot *slot = x->slot;
    ssize_t i;

    for (i = x->count; i > 0; i--) {
        if (!__is_leaf(x))
            __backward(slot[i].child, do_each, priv);

        do_each(slot[i - 1].entry, priv);
    }

    if (!__is_leaf(x))
        __backward(slot[0].child, do_each, priv);
}

static void __for_each_preorder(struct btre_node *x, do_each_t do_each,
                                void *priv)
{
    struct btre_slot *slot = x->slot;
    size_t i;

    for (i = 0; i < x->count; i++)
        do_each(slot[i].entry, priv);

    if (!__is_leaf(x))
        for (i = 0; i <= x->count; i++)
            __for_each_preorder(slot[i].child, do_each, priv);
}

static void __for_each_inorder(struct btre_node *x, do_each_t do_each,
                               void *priv)
{
    __forward(x, do_each, priv);
}

static void __for_each_postorder(struct btre_node *x, do_each_t do_each,
                                 void *priv)
{
    struct btre_slot *slot = x->slot;
    size_t i;

    if (!__is_leaf(x))
        for (i = 0; i <= x->count; i++)
            __for_each_postorder(slot[i].child, do_each, priv);

    for (i = 0; i < x->count; i++)
        do_each(slot[i].entry, priv);
}

int btre_minimum(const struct btre *tree, void **_e)
{
    struct btre_node *x;

    if (btre_is_empty(tree))
        return -ENODATA;

    x = __minimum(tree->root);

    if (_e != NULL)
        *_e = x->slot[0].entry;

    return 0;
}

int btre_maximum(const struct btre *tree, void **_e)
{
    struct btre_node *x;

    if (btre_is_empty(tree))
        return -ENODATA;

    x = __maximum(tree->root);

    if (_e != NULL)
        *_e = x->slot[x->count - 1].entry;

    return 0;
}

int btre_search(const struct btre *tree, const void *ke, void **_e)
{
    const struct btre_node *e;
    size_t idx;

    if (btre_is_empty(tree))
        return -ENODATA;

    e = __search(tree, tree->root, ke, &idx);
    if (e == NULL)
        return -ENONET;

    if (_e != NULL)
        *_e = (void *)e->slot[idx].entry;

    return 0;
}

int btre_insert(struct btre *tree, void *e)
{
    return btre_is_empty(tree) ? __insert_for_empty(tree, e) :
                                 __insert_for_non_empty(tree, e);
}

int btre_remove(struct btre *tree, const void *ke, void **_e)
{
    struct btre_node *root;
    void *e;
    int err;

    err = btre_search(tree, ke, &e);
    if (err != 0)
        return err;

    if (_e != NULL)
        *_e = e;

    root = tree->root;
    __remove(tree, root, ke);

    if (root->count == 0) {
        struct btre_node *tmp = root;

        if (__is_leaf(root))
            tree->root = NULL;
        else
            tree->root = root->slot[0].child;

        free(tmp);
    }

    return 0;
}

void btre_forward(struct btre *tree, do_each_t do_each, void *priv)
{
    if (btre_is_empty(tree))
        return;

    __forward(tree->root, do_each, priv);
}

void btre_backward(struct btre *tree, do_each_t do_each, void *priv)
{
    if (btre_is_empty(tree))
        return;

    __backward(tree->root, do_each, priv);
}

void btre_preorder(struct btre *tree, do_each_t do_each, void *priv)
{
    if (btre_is_empty(tree))
        return;

    __for_each_preorder(tree->root, do_each, priv);
}

void btre_inorder(struct btre *tree, do_each_t do_each, void *priv)
{
    if (btre_is_empty(tree))
        return;

    __for_each_inorder(tree->root, do_each, priv);
}

void btre_postorder(struct btre *tree, do_each_t do_each, void *priv)
{
    if (btre_is_empty(tree))
        return;

    __for_each_postorder(tree->root, do_each, priv);
}
