/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Binary-Search Tree */
#ifndef __RCN_C_BSTREE_H__
#define __RCN_C_BSTREE_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct bsnode {
    struct bsnode *parent_;
    struct bsnode *left_;
    struct bsnode *right_;
    void *entry_;
};

struct bstree {
    struct bsnode *root_;
    int (*compar_)(const void *ke, const void *in_tree);
    size_t size_;
};

static inline void bstree_clear(struct bstree *self)
{
    self->root_ = NULL;
    self->size_ = 0;
}

static inline void bstree_init(struct bstree *self,
                               int (*compar)(const void *ke,
                                             const void *in_tree))
{
    bstree_clear(self);
    self->compar_ = compar;
}

static inline bool bstree_empty(const struct bstree *self)
{
    return self->root_ == NULL;
}

static inline struct bsnode *bstree_begin(const struct bstree *self)
{
    struct bsnode *n = self->root_;

    if (n == NULL) {
        return NULL;
    }

    while (n->left_ != NULL) {
        n = n->left_;
    }

    return n;
}

static inline struct bsnode *bstree_rbegin(const struct bstree *self)
{
    struct bsnode *n = self->root_;

    if (n == NULL) {
        return NULL;
    }

    while (n->right_ != NULL) {
        n = n->right_;
    }

    return n;
}

static inline const struct bsnode *bstree_end(const struct bstree *self)
{
    return NULL;
}

static inline const struct bsnode *bstree_rend(const struct bstree *self)
{
    return NULL;
}

static inline struct bsnode *bstree_next(const struct bstree *self,
                                         const struct bsnode *x)
{
    struct bsnode *parent;

    if (bstree_empty(self)) {
        return NULL;
    }

    if (x->right_ != NULL) {
        x = x->right_;

        while (x->left_ != NULL) {
            x = x->left_;
        }

        return (struct bsnode *)x;
    }

    while (((parent = x->parent_) != NULL) && x == parent->right_) {
        x = parent;
    }

    return parent;
}

static inline struct bsnode *bstree_prev(const struct bstree *self,
                                         const struct bsnode *x)
{
    struct bsnode *parent;

    if (bstree_empty(self)) {
        return NULL;
    }

    if (x->left_ != NULL) {
        x = x->left_;

        while (x->right_ != NULL) {
            x = x->right_;
        }

        return (struct bsnode *)x;
    }

    while (((parent = x->parent_) != NULL) && x == parent->left_) {
        x = parent;
    }

    return parent;
}

static inline size_t bstree_size(const struct bstree *self)
{
    return self->size_;
}

static inline void *bstree_front(const struct bstree *self)
{
    return bstree_empty(self) ? NULL : bstree_begin(self)->entry_;
}

static inline void *bstree_back(const struct bstree *self)
{
    return bstree_empty(self) ? NULL : bstree_rbegin(self)->entry_;
}

static inline int bstree_insert(struct bstree *self, struct bsnode *z, void *e)
{
    struct bsnode *x = self->root_;
    struct bsnode *y = NULL;
    int diff = 0;

    while (x != NULL) {
        y = x;
        diff = self->compar_(e, y->entry_);

        if (diff < 0) {
            x = x->left_;
        } else if (diff > 0) {
            x = x->right_;
        } else {
            return -EEXIST;
        }
    }

    z->entry_ = e;
    z->parent_ = y;
    z->left_ = z->right_ = NULL;

    if (y == NULL) {
        self->root_ = z;
    } else if (diff < 0) {
        y->left_ = z;
    } else {
        y->right_ = z;
    }

    self->size_++;

    return 0;
}

static inline void __bstree_transplant(struct bstree *self, struct bsnode *u,
                                       struct bsnode *v)
{
    if (u->parent_ == NULL) {
        self->root_ = v;
    } else if (u == u->parent_->left_) {
        u->parent_->left_ = v;
    } else {
        u->parent_->right_ = v;
    }

    if (v != NULL) {
        v->parent_ = u->parent_;
    }
}

static inline struct bsnode *bstree_find(const struct bstree *self,
                                         const void *ke)
{
    struct bsnode *x = self->root_;

    while (x != NULL) {
        int diff = self->compar_(ke, x->entry_);

        if (diff == 0) {
            return x;
        } else if (diff < 0) {
            x = x->left_;
        } else {
            x = x->right_;
        }
    }

    return NULL;
}

static inline void *bstree_erase(struct bstree *self, struct bsnode *z)
{
    struct bsnode *x = bstree_find(self, z->entry_);
    struct bsnode *y;
    void *e;

    if ((x == NULL) || (x != z)) {
        return NULL;
    }

    e = z->entry_;
    y = z;

    if (z->left_ == NULL) {
        __bstree_transplant(self, z, z->right_);
    } else if (z->right_ == NULL) {
        __bstree_transplant(self, z, z->left_);
    } else {
        y = z->right_;

        while (y->left_ != NULL) {
            y = y->left_;
        }

        if (y->parent_ != z) {
            __bstree_transplant(self, y, y->right_);
            y->right_ = z->right_;
            y->right_->parent_ = y;
        }

        __bstree_transplant(self, z, y);
        y->left_ = z->left_;
        y->left_->parent_ = y;
    }

    self->size_--;

    return e;
}

static inline void *bstree_pop_front(struct bstree *self)
{
    return bstree_erase(self, bstree_begin(self));
}

static inline void *bstree_pop_back(struct bstree *self)
{
    return bstree_erase(self, bstree_rbegin(self));
}

static inline void *bstree_at(struct bstree *self, size_t n)
{
    struct bsnode *x;

    for (x = bstree_begin(self); n != 0 && x != bstree_end(self); --n) {
        x = bstree_next(self, x);
    }

    return x == bstree_end(self) ? NULL : x->entry_;
}

static inline struct bsnode *bstree_lower_bound(const struct bstree *self,
                                                const void *ke)
{
    struct bsnode *x = self->root_;
    struct bsnode *result = NULL;

    while (x != NULL) {
        int diff = self->compar_(ke, x->entry_);

        if (diff <= 0) {
            result = x;
            x = x->left_;
        } else {
            x = x->right_;
        }
    }

    return result;
}

static inline struct bsnode *bstree_upper_bound(const struct bstree *self,
                                                const void *ke)
{
    struct bsnode *x = self->root_;
    struct bsnode *result = NULL;

    while (x != NULL) {
        int diff = self->compar_(ke, x->entry_);

        if (diff < 0) {
            result = x;
            x = x->left_;
        } else {
            x = x->right_;
        }
    }

    return result;
}

static inline bool bstree_validate(const struct bstree *self)
{
    size_t size = 0;

    for (struct bsnode *x = bstree_begin(self); x != bstree_end(self);
         x = bstree_next(self, x)) {
        size++;
    }

    return bstree_size(self) == size;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_BSTREE_H__ */
