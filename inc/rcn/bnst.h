/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Binary Search Tree */
#ifndef __BNST_H__
#define __BNST_H__

#include "common.h"

struct bnst_node;

typedef int (*bnst_compar_t)(const struct bnst_node *key,
                             const struct bnst_node *in_tree);

typedef void (*bnst_do_each_t)(struct bnst_node *x, void *priv);

struct bnst_node {
    struct bnst_node *parent;
    struct bnst_node *left;
    struct bnst_node *right;
};

struct bnst {
    struct bnst_node *root;
    bnst_compar_t compar;
};

static inline void bnst_initialize(struct bnst *tree, bnst_compar_t compar)
{
    tree->root = NULL;
    tree->compar = compar;
}

static inline bool bnst_is_empty(const struct bnst *tree)
{
    return tree->root == NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int bnst_minimum(const struct bnst *tree, struct bnst_node **_x);
extern int bnst_maximum(const struct bnst *tree, struct bnst_node **_x);
extern int bnst_search(const struct bnst *tree, const struct bnst_node *kx,
                       struct bnst_node **_x);
extern int bnst_insert(struct bnst *tree, struct bnst_node *x);
extern int bnst_remove(struct bnst *tree, struct bnst_node *z);
extern void bnst_forward(struct bnst *tree, bnst_do_each_t do_each, void *priv);
extern void bnst_backward(struct bnst *tree, bnst_do_each_t do_each,
                          void *priv);
extern void bnst_preorder(struct bnst *tree, bnst_do_each_t do_each,
                          void *priv);
extern void bnst_inorder(struct bnst *tree, bnst_do_each_t do_each, void *priv);
extern void bnst_postorder(struct bnst *tree, bnst_do_each_t do_each,
                           void *priv);

#ifdef __cplusplus
}
#endif
#endif /* __BNST_H__ */
