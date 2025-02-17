/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Adelson-Velsky and Landis Tree */
#ifndef __AVLT_H__
#define __AVLT_H__

#include "common.h"

struct avlt_node;

typedef int (*avlt_compar_t)(const struct avlt_node *key,
                             const struct avlt_node *in_tree);

typedef void (*avlt_do_each_t)(struct avlt_node *x, void *priv);

struct avlt_node {
    struct avlt_node *parent;
    struct avlt_node *right;
    struct avlt_node *left;
    ssize_t height;
};

struct avlt {
    struct avlt_node *root;
    avlt_compar_t compar;
};

static inline void avlt_initialize(struct avlt *tree, avlt_compar_t compar)
{
    tree->root = NULL;
    tree->compar = compar;
}

static inline bool avlt_is_empty(const struct avlt *tree)
{
    return tree->root == NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int avlt_minimum(const struct avlt *tree, struct avlt_node **_x);
extern int avlt_maximum(const struct avlt *tree, struct avlt_node **_x);
extern int avlt_search(const struct avlt *tree, const struct avlt_node *kx,
                       struct avlt_node **_x);
extern int avlt_insert(struct avlt *tree, struct avlt_node *x);
extern int avlt_remove(struct avlt *tree, struct avlt_node *z);
extern void avlt_forward(struct avlt *tree, avlt_do_each_t do_each, void *priv);
extern void avlt_backward(struct avlt *tree, avlt_do_each_t do_each,
                          void *priv);
extern void avlt_preorder(struct avlt *tree, avlt_do_each_t do_each,
                          void *priv);
extern void avlt_inorder(struct avlt *tree, avlt_do_each_t do_each, void *priv);
extern void avlt_postorder(struct avlt *tree, avlt_do_each_t do_each,
                           void *priv);

#ifdef __cplusplus
}
#endif
#endif /* __AVLT_H__ */
