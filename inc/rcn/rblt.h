/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Red Black Tree */
#ifndef __RBLT_H__
#define __RBLT_H__

#include "common.h"

struct rblt_node;

typedef int (*rblt_compar_t)(const struct rblt_node *key,
                             const struct rblt_node *in_tree);

typedef void (*rblt_do_each_t)(struct rblt_node *x, void *priv);

enum rblt_color_t { RBLT_RED = 0, RBLT_BLACK };

struct rblt_node {
    struct rblt_node *parent;
    struct rblt_node *right;
    struct rblt_node *left;
    enum rblt_color_t color;
};

struct rblt {
    struct rblt_node *root;
    struct rblt_node *NiL;
    struct rblt_node __NiL;
    rblt_compar_t compar;
};

static inline void rblt_initialize(struct rblt *tree, rblt_compar_t compar)
{
    struct rblt_node *NiL = &tree->__NiL;

    tree->NiL = NiL;
    tree->root = NiL;
    tree->compar = compar;

    NiL->left = tree->NiL;
    NiL->right = tree->NiL;
    NiL->color = RBLT_BLACK;
}

static inline bool rblt_is_empty(const struct rblt *tree)
{
    return tree->root == tree->NiL ? true : false;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int rblt_minimum(const struct rblt *tree, struct rblt_node **_x);
extern int rblt_maximum(const struct rblt *tree, struct rblt_node **_x);
extern int rblt_search(const struct rblt *tree, const struct rblt_node *kx,
                       struct rblt_node **_x);
extern int rblt_insert(struct rblt *tree, struct rblt_node *x);
extern int rblt_remove(struct rblt *tree, struct rblt_node *z);
extern void rblt_forward(struct rblt *tree, rblt_do_each_t do_each, void *priv);
extern void rblt_backward(struct rblt *tree, rblt_do_each_t do_each,
                          void *priv);
extern void rblt_preorder(struct rblt *tree, rblt_do_each_t do_each,
                          void *priv);
extern void rblt_inorder(struct rblt *tree, rblt_do_each_t do_each, void *priv);
extern void rblt_postorder(struct rblt *tree, rblt_do_each_t do_each,
                           void *priv);

#ifdef __cplusplus
}
#endif
#endif /* __RBLT_H__ */
