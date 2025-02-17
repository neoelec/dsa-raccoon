/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Splay Tree */
#ifndef __SPLT_H__
#define __SPLT_H__

#include "common.h"

struct splt_node;

typedef int (*splt_compar_t)(const struct splt_node *key,
                             const struct splt_node *in_tree);

typedef void (*splt_do_each_t)(struct splt_node *x, void *priv);

struct splt_node {
    struct splt_node *parent;
    struct splt_node *right;
    struct splt_node *left;
};

struct splt {
    struct splt_node *root;
    splt_compar_t compar;
};

static inline void splt_initialize(struct splt *tree, splt_compar_t compar)
{
    tree->root = NULL;
    tree->compar = compar;
}

static inline bool splt_is_empty(const struct splt *tree)
{
    return tree->root == NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

extern void splt_initialize(struct splt *tree, splt_compar_t compar);
extern bool splt_is_empty(const struct splt *tree);
extern int splt_minimum(const struct splt *tree, struct splt_node **_x);
extern int splt_maximum(const struct splt *tree, struct splt_node **_x);
extern int splt_search(const struct splt *tree, const struct splt_node *kx,
                       struct splt_node **_x);
extern int splt_insert(struct splt *tree, struct splt_node *x);
extern int splt_remove(struct splt *tree, struct splt_node *z);
extern void splt_forward(struct splt *tree, splt_do_each_t do_each, void *priv);
extern void splt_backward(struct splt *tree, splt_do_each_t do_each,
                          void *priv);
extern void splt_preorder(struct splt *tree, splt_do_each_t do_each,
                          void *priv);
extern void splt_inorder(struct splt *tree, splt_do_each_t do_each, void *priv);
extern void splt_postorder(struct splt *tree, splt_do_each_t do_each,
                           void *priv);

#ifdef __cplusplus
}
#endif
#endif /* __SPLT_H__ */
