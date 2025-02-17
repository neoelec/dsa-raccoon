/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* B-Tree */
#ifndef __BTRE_H__
#define __BTRE_H__

#include "common.h"

struct btre {
    size_t t;
    struct btre_node *root;
    compar_t compar;
};

static inline void btre_initialize(struct btre *tree, size_t t, compar_t compar)
{
    assert(t > 1);

    tree->t = t;
    tree->root = NULL;
    tree->compar = compar;
}

static inline bool btre_is_empty(const struct btre *tree)
{
    return !tree->root;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int btre_minimum(const struct btre *tree, void **_e);
extern int btre_maximum(const struct btre *tree, void **_e);
extern int btre_search(const struct btre *tree, const void *ke, void **_e);
extern int btre_insert(struct btre *tree, void *e);
extern int btre_remove(struct btre *tree, const void *ke, void **_e);
extern void btre_forward(struct btre *tree, do_each_t do_each, void *priv);
extern void btre_backward(struct btre *tree, do_each_t do_each, void *priv);
extern void btre_preorder(struct btre *tree, do_each_t do_each, void *priv);
extern void btre_inorder(struct btre *tree, do_each_t do_each, void *priv);
extern void btre_postorder(struct btre *tree, do_each_t do_each, void *priv);

#ifdef __cplusplus
}
#endif

static inline int btre_minimum_i(const struct btre *tree, intptr_t *_v)
{
    return btre_minimum(tree, (void **)_v);
}

static inline int btre_maximum_i(const struct btre *tree, intptr_t *_v)
{
    return btre_maximum(tree, (void **)_v);
}

static inline int btre_search_i(const struct btre *tree, intptr_t kv,
                                intptr_t *_v)
{
    return btre_search(tree, (void *)kv, (void **)_v);
}

static inline int btre_insert_i(struct btre *tree, intptr_t v)
{
    return btre_insert(tree, (void *)v);
}

static inline int btre_remove_i(struct btre *tree, intptr_t kv, intptr_t *_v)
{
    return btre_remove(tree, (void *)kv, (void **)_v);
}

#endif /* __BTRE_H__ */
