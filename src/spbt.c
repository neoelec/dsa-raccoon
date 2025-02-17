/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdlib.h>

#include <rcn/spbt.h>

static void __for_each_preorder(struct spbt_node *x, spbt_do_each_t do_each,
                                void *priv)
{
    if (x == NULL)
        return;

    do_each(x, priv);
    __for_each_preorder(x->left, do_each, priv);
    __for_each_preorder(x->right, do_each, priv);
}

static void __for_each_inorder(struct spbt_node *x, spbt_do_each_t do_each,
                               void *priv)
{
    if (x == NULL)
        return;

    __for_each_inorder(x->left, do_each, priv);
    do_each(x, priv);
    __for_each_inorder(x->right, do_each, priv);
}

static void __for_each_postorder(struct spbt_node *x, spbt_do_each_t do_each,
                                 void *priv)
{
    if (x == NULL)
        return;

    __for_each_postorder(x->left, do_each, priv);
    __for_each_postorder(x->right, do_each, priv);
    do_each(x, priv);
}

void spbt_preorder(struct spbt *tree, spbt_do_each_t do_each, void *priv)
{
    __for_each_preorder(tree->root, do_each, priv);
}

void spbt_inorder(struct spbt *tree, spbt_do_each_t do_each, void *priv)
{
    __for_each_inorder(tree->root, do_each, priv);
}

void spbt_postorder(struct spbt *tree, spbt_do_each_t do_each, void *priv)
{
    __for_each_postorder(tree->root, do_each, priv);
}
