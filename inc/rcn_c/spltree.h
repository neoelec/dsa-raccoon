/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 *
 * Reference : https://cubelover.tistory.com/10
 */

/* Splay Tree */
#ifndef __RCN_C_SPLTREE_H__
#define __RCN_C_SPLTREE_H__

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct splnode {
    struct splnode *parent_;
    struct splnode *left_;
    struct splnode *right_;
    size_t count_;
    void *entry_;
};

struct spltree {
    struct splnode *root_;
    int (*compar_)(const void *ke, const void *in_tree);
};

static inline void spltree_clear(struct spltree *self)
{
    self->root_ = NULL;
}

static inline void spltree_init(struct spltree *self,
                                int (*compar)(const void *ke,
                                              const void *in_tree))
{
    spltree_clear(self);
    self->compar_ = compar;
}

static inline bool spltree_empty(const struct spltree *self)
{
    return self->root_ == NULL;
}

static inline struct splnode *spltree_begin(const struct spltree *self)
{
    struct splnode *n = self->root_;

    if (n == NULL) {
        return NULL;
    }

    while (n->left_ != NULL) {
        n = n->left_;
    }

    return n;
}

static inline struct splnode *spltree_rbegin(const struct spltree *self)
{
    struct splnode *n = self->root_;

    if (n == NULL) {
        return NULL;
    }

    while (n->right_ != NULL) {
        n = n->right_;
    }

    return n;
}

static inline const struct splnode *spltree_end(const struct spltree *self)
{
    return NULL;
}

static inline const struct splnode *spltree_rend(const struct spltree *self)
{
    return NULL;
}

static inline struct splnode *spltree_next(const struct spltree *self,
                                           const struct splnode *x)
{
    struct splnode *parent;

    if (spltree_empty(self)) {
        return NULL;
    }

    if (x->right_ != NULL) {
        x = x->right_;

        while (x->left_ != NULL) {
            x = x->left_;
        }

        return (struct splnode *)x;
    }

    while (((parent = x->parent_) != NULL) && x == parent->right_) {
        x = parent;
    }

    return parent;
}

static inline struct splnode *spltree_prev(const struct spltree *self,
                                           const struct splnode *x)
{
    struct splnode *parent;

    if (spltree_empty(self)) {
        return NULL;
    }

    if (x->left_ != NULL) {
        x = x->left_;

        while (x->right_ != NULL) {
            x = x->right_;
        }

        return (struct splnode *)x;
    }

    while (((parent = x->parent_) != NULL) && x == parent->left_) {
        x = parent;
    }

    return parent;
}

static inline size_t spltree_count(const struct spltree *self)
{
    return spltree_empty(self) ? 0 : self->root_->count_;
}

static inline void *spltree_front(const struct spltree *self)
{
    return spltree_empty(self) ? NULL : spltree_begin(self)->entry_;
}

static inline void *spltree_back(const struct spltree *self)
{
    return spltree_empty(self) ? NULL : spltree_rbegin(self)->entry_;
}

static inline void __splnode_update(struct splnode *x)
{
    x->count_ = 1;

    if (x->left_ != NULL) {
        x->count_ += x->left_->count_;
    }

    if (x->right_ != NULL) {
        x->count_ += x->right_->count_;
    }
}

static inline void __spltree_rotate(struct spltree *self, struct splnode *x)
{
    struct splnode *p = x->parent_;
    struct splnode *b;

    if (x == p->left_) {
        p->left_ = b = x->right_;
        x->right_ = p;
    } else {
        p->right_ = b = x->left_;
        x->left_ = p;
    }

    x->parent_ = p->parent_;
    p->parent_ = x;

    if (b != NULL) {
        b->parent_ = p;
    }

    if (x->parent_ != NULL) {
        if (p == x->parent_->left_) {
            x->parent_->left_ = x;
        } else {
            x->parent_->right_ = x;
        }
    } else {
        self->root_ = x;
    }

    __splnode_update(p);
    __splnode_update(x);
}

static inline void __spltree_splay(struct spltree *self, struct splnode *x)
{
    while (x->parent_ != NULL) {
        struct splnode *p = x->parent_;
        struct splnode *g = p->parent_;

        if (g != NULL) {
            __spltree_rotate(self, (x == p->left_) == (p == g->left_) ? p : x);
        }

        __spltree_rotate(self, x);
    }
}

static inline void spltree_insert(struct spltree *self, struct splnode *z,
                                  void *e)
{
    struct splnode *p = self->root_, **pp;

    z->entry_ = e;
    z->left_ = z->right_ = NULL;

    if (p == NULL) {
        self->root_ = z;
        z->parent_ = NULL;
        return;
    }

    while (true) {
        int diff = self->compar_(e, p->entry_);

        if (diff == 0) {
            return;
        } else if (diff < 0) {
            if (p->left_ == NULL) {
                pp = &p->left_;
                break;
            }

            p = p->left_;
        } else {
            if (p->right_ == NULL) {
                pp = &p->right_;
                break;
            }

            p = p->right_;
        }
    }

    *pp = z;
    z->parent_ = p;
    __spltree_splay(self, z);
}

static inline struct splnode *spltree_find(const struct spltree *self,
                                           const void *ke)
{
    const struct splnode *x = self->root_;

    while (x != NULL) {
        int diff = self->compar_(ke, x->entry_);

        if (diff == 0) {
            __spltree_splay((struct spltree *)self, (struct splnode *)x);
            return (struct splnode *)x;
        } else if (diff < 0) {
            x = x->left_;
        } else {
            x = x->right_;
        }
    }

    return NULL;
}

static inline void *spltree_erase(struct spltree *self, struct splnode *z)
{
    struct splnode *p;
    struct splnode *x = spltree_find(self, z->entry_);
    void *e;

    if (x == NULL || x != z) {
        return NULL;
    }

    e = x->entry_;
    p = self->root_;

    if (p->left_ != NULL) {
        if (p->right_ != NULL) {
            self->root_ = p->left_;
            self->root_->parent_ = NULL;
            x = self->root_;

            while (x->right_ != NULL) {
                x = x->right_;
            }

            x->right_ = p->right_;
            p->right_->parent_ = x;
            __spltree_splay(self, x);
            return e;
        }

        self->root_ = p->left_;
        self->root_->parent_ = NULL;
        return e;
    }

    if (p->right_ != NULL) {
        self->root_ = p->right_;
        self->root_->parent_ = NULL;
        return e;
    }

    self->root_ = NULL;

    return e;
}

static inline void *spltree_pop_front(struct spltree *self)
{
    struct splnode *x = spltree_begin(self);

    return x == NULL ? NULL : spltree_erase(self, x);
}

static inline void *spltree_pop_back(struct spltree *self)
{
    struct splnode *x = spltree_rbegin(self);

    return x == NULL ? NULL : spltree_erase(self, x);
}

static inline void *spltree_at(struct spltree *self, size_t n)
{
    if (spltree_empty(self) || (n >= spltree_count(self))) {
        return NULL;
    }

    struct splnode *x = self->root_;

    while (true) {
        while (x->left_ != NULL && x->left_->count_ > n) {
            x = x->left_;
        }

        if (x->left_ != NULL) {
            n -= x->left_->count_;
        }

        if (n == 0) {
            break;
        }

        n--;
        x = x->right_;
    }

    __spltree_splay(self, x);

    return x->entry_;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_SPLTREE_H__ */
