/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Simple Binary Tree */
#ifndef __SPBT_H__
#define __SPBT_H__

#include "common.h"

struct spbt_node;

typedef int (*spbt_compar_t)(const struct spbt_node *key,
                             const struct spbt_node *in_tree);
typedef void (*spbt_do_each_t)(struct spbt_node *x, void *priv);

struct spbt_node {
    struct spbt_node *left;
    struct spbt_node *right;
};

struct spbt {
    struct spbt_node *root;
};

static inline void spbt_initialize(struct spbt *tree)
{
    tree->root = NULL;
}

static inline void spbt_init_node(struct spbt_node *x)
{
    x->left = NULL;
    x->right = NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

extern void spbt_preorder(struct spbt *tree, spbt_do_each_t do_each,
                          void *priv);
extern void spbt_inorder(struct spbt *tree, spbt_do_each_t do_each, void *priv);
extern void spbt_postorder(struct spbt *tree, spbt_do_each_t do_each,
                           void *priv);

#ifdef __cplusplus
}
#endif
#endif /* __SPBT_H__ */
