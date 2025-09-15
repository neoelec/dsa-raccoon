/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 *
 * Reference : https://srdeveloper.tistory.com/29
 */

/* Adelson-Velsky and Landis Tree */
#ifndef __RCN_C_AVLTREE_H__
#define __RCN_C_AVLTREE_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct avlnode {
    struct avlnode *parent_;
    struct avlnode *left_;
    struct avlnode *right_;
    void *entry_;
};

struct avltree {
    struct avlnode *root_;
    int (*compar_)(const void *ke, const void *in_tree);
    size_t size_;
};

static inline void avltree_clear(struct avltree *self)
{
    self->root_ = NULL;
    self->size_ = 0;
}

static inline void avltree_init(struct avltree *self,
                                int (*compar)(const void *ke,
                                              const void *in_tree))
{
    avltree_clear(self);
    self->compar_ = compar;
}

static inline bool avltree_empty(const struct avltree *self)
{
    return self->root_ == NULL;
}

static inline struct avlnode *avltree_begin(const struct avltree *self)
{
    struct avlnode *n = self->root_;

    if (n == NULL) {
        return NULL;
    }

    while (n->left_ != NULL) {
        n = n->left_;
    }

    return n;
}

static inline struct avlnode *avltree_rbegin(const struct avltree *self)
{
    struct avlnode *n = self->root_;

    if (n == NULL) {
        return NULL;
    }

    while (n->right_ != NULL) {
        n = n->right_;
    }

    return n;
}

static inline const struct avlnode *avltree_end(const struct avltree *self)
{
    return NULL;
}

static inline const struct avlnode *avltree_rend(const struct avltree *self)
{
    return NULL;
}

static inline struct avlnode *avltree_next(const struct avltree *self,
                                           const struct avlnode *x)
{
    struct avlnode *parent;

    if (avltree_empty(self)) {
        return NULL;
    }

    if (x->right_ != NULL) {
        x = x->right_;

        while (x->left_ != NULL) {
            x = x->left_;
        }

        return (struct avlnode *)x;
    }

    while (((parent = x->parent_) != NULL) && x == parent->right_) {
        x = parent;
    }

    return parent;
}

static inline struct avlnode *avltree_prev(const struct avltree *self,
                                           const struct avlnode *x)
{
    struct avlnode *parent;

    if (avltree_empty(self)) {
        return NULL;
    }

    if (x->left_ != NULL) {
        x = x->left_;

        while (x->right_ != NULL) {
            x = x->right_;
        }

        return (struct avlnode *)x;
    }

    while (((parent = x->parent_) != NULL) && x == parent->left_) {
        x = parent;
    }

    return parent;
}

static inline size_t avltree_size(const struct avltree *self)
{
    return self->size_;
}

static inline void *avltree_front(const struct avltree *self)
{
    return avltree_empty(self) ? NULL : avltree_begin(self)->entry_;
}

static inline void *avltree_back(const struct avltree *self)
{
    return avltree_empty(self) ? NULL : avltree_rbegin(self)->entry_;
}

static inline ssize_t __avlnode_max_height(const struct avlnode *x)
{
    if (x == NULL) {
        return 0;
    }

    ssize_t left_height = __avlnode_max_height(x->left_);
    ssize_t right_height = __avlnode_max_height(x->right_);

    return (left_height > right_height ? left_height : right_height) + 1;
}

static inline ssize_t __avlnode_BF(const struct avlnode *x)
{
    return __avlnode_max_height(x->left_) - __avlnode_max_height(x->right_);
}

static inline struct avlnode *__avlnode_RR(struct avlnode *x)
{
    struct avlnode *y = x->right_;

    x->right_ = y->left_;

    if (y->left_ != NULL) {
        y->left_->parent_ = x;
    }

    y->left_ = x;
    y->parent_ = x->parent_;
    x->parent_ = y;

    return y;
}

static inline struct avlnode *__avlnode_LL(struct avlnode *x)
{
    struct avlnode *y = x->left_;

    x->left_ = y->right_;

    if (y->right_ != NULL) {
        y->right_->parent_ = x;
    }

    y->right_ = x;
    y->parent_ = x->parent_;
    x->parent_ = y;

    return y;
}

static inline struct avlnode *__avlnode_LR(struct avlnode *x)
{
    x->left_ = __avlnode_RR(x->left_);

    return __avlnode_LL(x);
}

static inline struct avlnode *__avlnode_RL(struct avlnode *x)
{
    x->right_ = __avlnode_LL(x->right_);

    return __avlnode_RR(x);
}

static inline struct avlnode *__avltree_rebalance(struct avlnode *x)
{
    ssize_t bf = __avlnode_BF(x);

    if (bf >= 2) {
        bf = __avlnode_BF(x->left_);

        if (bf >= 1) {
            x = __avlnode_LL(x);
        } else {
            x = __avlnode_LR(x);
        }
    } else if (bf <= -2) {
        bf = __avlnode_BF(x->right_);

        if (bf <= -1) {
            x = __avlnode_RR(x);
        } else {
            x = __avlnode_RL(x);
        }
    }

    return x;
}

static struct avlnode *__avltree_insert(struct avltree *self, struct avlnode *x,
                                        struct avlnode *z, int *err)
{
    if (x == NULL) {
        return z;
    }

    int diff = self->compar_(z->entry_, x->entry_);

    if (diff < 0) {
        x->left_ = __avltree_insert(self, x->left_, z, err);
        x->left_->parent_ = x;
    } else if (diff > 0) {
        x->right_ = __avltree_insert(self, x->right_, z, err);
        x->right_->parent_ = x;
    } else {
        *err = -EEXIST;
    }

    return __avltree_rebalance(x);
}

static inline int avltree_insert(struct avltree *self, struct avlnode *z,
                                 void *e)
{
    int err = 0;

    z->parent_ = z->left_ = z->right_ = NULL;
    z->entry_ = e;

    self->root_ = __avltree_insert(self, self->root_, z, &err);
    self->size_++;

    return 0;
}

static struct avlnode *__avlnode_successor(struct avlnode *x,
                                           struct avlnode *parent)
{
    if (x->left_ == NULL) {
        if (x->parent_ != NULL) {
            if (parent != x->parent_) {
                x->parent_->left_ = x->right_;
            } else {
                x->parent_->right_ = x->right_;
            }

            if (x->right_ != NULL) {
                x->right_->parent_ = x->parent_;
            }
        }

        return x;
    } else {
        return __avlnode_successor(x->left_, parent);
    }
}

static struct avlnode *__avltree_erase(struct avltree *self, struct avlnode *x,
                                       struct avlnode *z)
{
    if (x == NULL) {
        return NULL;
    }

    int diff = self->compar_(z->entry_, x->entry_);

    if (diff < 0) {
        x->left_ = __avltree_erase(self, x->left_, z);
        x = __avltree_rebalance(x);
    } else if (diff > 0) {
        x->right_ = __avltree_erase(self, x->right_, z);
        x = __avltree_rebalance(x);
    } else {
        if ((x->left_ != NULL) && (x->right_ != NULL)) {
            struct avlnode *y = __avlnode_successor(x->right_, x);

            y->parent_ = x->parent_;
            y->left_ = x->left_;
            y->right_ = x->right_;

            if (x->left_ != NULL) {
                x->left_->parent_ = y;
            }

            if (x->right_ != NULL) {
                x->right_->parent_ = y;
            }

            x = y;
        } else if (x->left_ != NULL) {
            x->left_->parent_ = x->parent_;
            x = x->left_;
        } else if (x->right_ != NULL) {
            x->right_->parent_ = x->parent_;
            x = x->right_;
        } else {
            x = NULL;
        }
    }

    return x;
}

static inline struct avlnode *avltree_find(const struct avltree *self,
                                           const void *ke)
{
    struct avlnode *x = self->root_;

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

static inline void *avltree_erase(struct avltree *self, struct avlnode *z)
{
    struct avlnode *x = avltree_find(self, z->entry_);

    if ((x == NULL) || (x != z)) {
        return NULL;
    }

    self->root_ = __avltree_erase(self, self->root_, z);
    self->size_--;

    return z->entry_;
}

static inline void *avltree_pop_front(struct avltree *self)
{
    return avltree_erase(self, avltree_begin(self));
}

static inline void *avltree_pop_back(struct avltree *self)
{
    return avltree_erase(self, avltree_rbegin(self));
}

static inline void *avltree_at(struct avltree *self, size_t n)
{
    struct avlnode *x;

    for (x = avltree_begin(self); n != 0 && x != avltree_end(self); --n) {
        x = avltree_next(self, x);
    }

    return x == avltree_end(self) ? NULL : x->entry_;
}

static inline struct avlnode *avltree_lower_bound(const struct avltree *self,
                                                  const void *ke)
{
    struct avlnode *x = self->root_;
    struct avlnode *result = NULL;

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

static inline struct avlnode *avltree_upper_bound(const struct avltree *self,
                                                  const void *ke)
{
    struct avlnode *x = self->root_;
    struct avlnode *result = NULL;

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

static inline bool __avltree_validate_node(const struct avlnode *x)
{
    if (x == NULL) {
        return true;
    }

    ssize_t bf = __avlnode_BF(x);

    if ((bf > 2) || (bf < -2)) {
        return false;
    }

    return __avltree_validate_node(x->left_) &&
           __avltree_validate_node(x->right_);
}

static inline bool avltree_validate(const struct avltree *self)
{
    if (!__avltree_validate_node(self->root_)) {
        return false;
    }

    size_t size = 0;

    for (struct avlnode *x = avltree_begin(self); x != avltree_end(self);
         x = avltree_next(self, x)) {
        size++;
    }

    return avltree_size(self) == size;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_AVLTREE_H__ */
