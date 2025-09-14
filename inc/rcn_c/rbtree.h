/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Red-Black Tree */
#ifndef __RCN_C_RBTREE_H__
#define __RCN_C_RBTREE_H__

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

enum rbcolor {
    RBCOLOR_RED = 0,
    RBCOLOR_BLACK,
};

struct rbnode {
    enum rbcolor color_;
    struct rbnode *parent_;
    struct rbnode *left_;
    struct rbnode *right_;
    void *entry_;
};

struct rbtree {
    struct rbnode *root_;
    int (*compar_)(const void *ke, const void *in_tree);
    struct rbnode NIL_;
    size_t size_;
};

static inline const struct rbnode *rbtree_nil(const struct rbtree *self)
{
    return &self->NIL_;
}

static inline void rbtree_clear(struct rbtree *self)
{
    const struct rbnode *const NIL = rbtree_nil(self);

    self->root_ = (struct rbnode *)NIL;
    self->size_ = 0;

    self->NIL_.color_ = RBCOLOR_BLACK;
    self->NIL_.left_ = (struct rbnode *)NIL;
    self->NIL_.right_ = (struct rbnode *)NIL;
    self->NIL_.entry_ = NULL;
}

static inline void rbtree_init(struct rbtree *self,
                               int (*compar)(const void *ke,
                                             const void *in_tree))
{
    rbtree_clear(self);
    self->compar_ = compar;
}

static inline bool rbtree_empty(const struct rbtree *self)
{
    return self->root_ == rbtree_nil(self);
}

static inline struct rbnode *rbtree_begin(const struct rbtree *self)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *n = self->root_;

    if (n == NIL) {
        return (struct rbnode *)NIL;
    }

    while (n->left_ != NIL) {
        n = n->left_;
    }

    return n;
}

static inline struct rbnode *rbtree_rbegin(const struct rbtree *self)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *n = self->root_;

    if (n == NIL) {
        return (struct rbnode *)NIL;
    }

    while (n->right_ != NIL) {
        n = n->right_;
    }

    return n;
}

static inline const struct rbnode *rbtree_end(const struct rbtree *self)
{
    return rbtree_nil(self);
}

static inline const struct rbnode *rbtree_rend(const struct rbtree *self)
{
    return rbtree_nil(self);
}

static inline struct rbnode *rbtree_next(const struct rbtree *self,
                                         const struct rbnode *x)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *parent;

    if (rbtree_empty(self)) {
        return (struct rbnode *)NIL;
    }

    if (x->right_ != NIL) {
        x = x->right_;

        while (x->left_ != NIL) {
            x = x->left_;
        }

        return (struct rbnode *)x;
    }

    while (((parent = x->parent_) != NIL) && x == parent->right_) {
        x = parent;
    }

    return parent;
}

static inline struct rbnode *rbtree_prev(const struct rbtree *self,
                                         const struct rbnode *x)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *parent;

    if (rbtree_empty(self)) {
        return (struct rbnode *)NIL;
    }

    if (x->left_ != NIL) {
        x = x->left_;

        while (x->right_ != NIL) {
            x = x->right_;
        }

        return (struct rbnode *)x;
    }

    while (((parent = x->parent_) != NIL) && x == parent->left_) {
        x = parent;
    }

    return parent;
}

static inline size_t rbtree_size(const struct rbtree *self)
{
    return self->size_;
}

static inline void *rbtree_front(const struct rbtree *self)
{
    return rbtree_empty(self) ? NULL : rbtree_begin(self)->entry_;
}

static inline void *rbtree_back(const struct rbtree *self)
{
    return rbtree_empty(self) ? NULL : rbtree_rbegin(self)->entry_;
}

static inline void __rbtree_left_rotate(struct rbtree *self, struct rbnode *x)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *y = x->right_;

    x->right_ = y->left_;

    if (y->left_ != NIL) {
        y->left_->parent_ = x;
    }

    y->parent_ = x->parent_;

    if (x->parent_ == NIL) {
        self->root_ = y;
    } else if (x == x->parent_->left_) {
        x->parent_->left_ = y;
    } else {
        x->parent_->right_ = y;
    }

    y->left_ = x;
    x->parent_ = y;
}

static inline void __rbtree_right_rotate(struct rbtree *self, struct rbnode *x)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *y = x->left_;

    x->left_ = y->right_;

    if (y->right_ != NIL) {
        y->right_->parent_ = x;
    }

    y->parent_ = x->parent_;

    if (x->parent_ == NIL) {
        self->root_ = y;
    } else if (x == x->parent_->left_) {
        x->parent_->left_ = y;
    } else {
        x->parent_->right_ = y;
    }

    y->right_ = x;
    x->parent_ = y;
}

static inline void __rbtree_insert_fixup(struct rbtree *self, struct rbnode *z)
{
    while (z->parent_->color_ == RBCOLOR_RED) {
        if (z->parent_ == z->parent_->parent_->left_) {
            struct rbnode *y = z->parent_->parent_->right_;

            if (y->color_ == RBCOLOR_RED) {
                z->parent_->color_ = RBCOLOR_BLACK;
                y->color_ = RBCOLOR_BLACK;
                z->parent_->parent_->color_ = RBCOLOR_RED;
                z = z->parent_->parent_;
            } else {
                if (z == z->parent_->right_) {
                    z = z->parent_;
                    __rbtree_left_rotate(self, z);
                }

                z->parent_->color_ = RBCOLOR_BLACK;
                z->parent_->parent_->color_ = RBCOLOR_RED;
                __rbtree_right_rotate(self, z->parent_->parent_);
            }
        } else {
            struct rbnode *y = z->parent_->parent_->left_;

            if (y->color_ == RBCOLOR_RED) {
                z->parent_->color_ = RBCOLOR_BLACK;
                y->color_ = RBCOLOR_BLACK;
                z->parent_->parent_->color_ = RBCOLOR_RED;
                z = z->parent_->parent_;
            } else {
                if (z == z->parent_->left_) {
                    z = z->parent_;
                    __rbtree_right_rotate(self, z);
                }

                z->parent_->color_ = RBCOLOR_BLACK;
                z->parent_->parent_->color_ = RBCOLOR_RED;
                __rbtree_left_rotate(self, z->parent_->parent_);
            }
        }
    }

    self->root_->color_ = RBCOLOR_BLACK;
}

static inline void rbtree_insert(struct rbtree *self, struct rbnode *z, void *e)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *x = self->root_;
    struct rbnode *y = (struct rbnode *)NIL;
    int diff = 0;

    while (x != NIL) {
        y = x;
        diff = self->compar_(e, y->entry_);

        if (diff < 0) {
            x = x->left_;
        } else {
            x = x->right_;
        }
    }

    z->entry_ = e;
    z->parent_ = y;
    z->left_ = z->right_ = (struct rbnode *)NIL;
    z->color_ = RBCOLOR_RED;

    if (y == NIL) {
        self->root_ = z;
    } else if (diff < 0) {
        y->left_ = z;
    } else {
        y->right_ = z;
    }

    self->size_++;
    __rbtree_insert_fixup(self, z);
}

static inline void __rbtree_transplant(struct rbtree *self, struct rbnode *u,
                                       struct rbnode *v)
{
    const struct rbnode *const NIL = rbtree_nil(self);

    if (u->parent_ == NIL) {
        self->root_ = v;
    } else if (u == u->parent_->left_) {
        u->parent_->left_ = v;
    } else {
        u->parent_->right_ = v;
    }

    v->parent_ = u->parent_;
}

static inline void __rbtree_delete_fixup(struct rbtree *self, struct rbnode *x)
{
    while (x != self->root_ && x->color_ == RBCOLOR_BLACK) {
        if (x == x->parent_->left_) {
            struct rbnode *w = x->parent_->right_;

            if (w->color_ == RBCOLOR_RED) {
                w->color_ = RBCOLOR_BLACK;
                x->parent_->color_ = RBCOLOR_RED;
                __rbtree_left_rotate(self, x->parent_);
                w = x->parent_->right_;
            }

            if ((w->left_->color_ == RBCOLOR_BLACK) &&
                (w->right_->color_ == RBCOLOR_BLACK)) {
                w->color_ = RBCOLOR_RED;
                x = x->parent_;
            } else {
                if (w->right_->color_ == RBCOLOR_BLACK) {
                    w->left_->color_ = RBCOLOR_BLACK;
                    w->color_ = RBCOLOR_RED;
                    __rbtree_right_rotate(self, w);
                    w = x->parent_->right_;
                }

                w->color_ = x->parent_->color_;
                x->parent_->color_ = RBCOLOR_BLACK;
                w->right_->color_ = RBCOLOR_BLACK;
                __rbtree_left_rotate(self, x->parent_);
                x = self->root_;
            }
        } else {
            struct rbnode *w = x->parent_->left_;

            if (w->color_ == RBCOLOR_RED) {
                w->color_ = RBCOLOR_BLACK;
                x->parent_->color_ = RBCOLOR_RED;
                __rbtree_right_rotate(self, x->parent_);
                w = x->parent_->left_;
            }

            if ((w->right_->color_ == RBCOLOR_BLACK) &&
                (w->left_->color_ == RBCOLOR_BLACK)) {
                w->color_ = RBCOLOR_RED;
                x = x->parent_;
            } else {
                if (w->left_->color_ == RBCOLOR_BLACK) {
                    w->right_->color_ = RBCOLOR_BLACK;
                    w->color_ = RBCOLOR_RED;
                    __rbtree_left_rotate(self, w);
                    w = x->parent_->left_;
                }

                w->color_ = x->parent_->color_;
                x->parent_->color_ = RBCOLOR_BLACK;
                w->left_->color_ = RBCOLOR_BLACK;
                __rbtree_right_rotate(self, x->parent_);
                x = self->root_;
            }
        }
    }

    x->color_ = RBCOLOR_BLACK;
}

static inline struct rbnode *rbtree_find(const struct rbtree *self,
                                         const void *ke)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *x = self->root_;

    while (x != NIL) {
        int diff;

        diff = self->compar_(ke, x->entry_);

        if (diff == 0) {
            return x;
        } else if (diff < 0) {
            x = x->left_;
        } else {
            x = x->right_;
        }
    }

    return (struct rbnode *)NIL;
}

static inline void *rbtree_erase(struct rbtree *self, struct rbnode *z)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *x = rbtree_find(self, z->entry_);
    struct rbnode *y;
    enum rbcolor y_original_color;
    void *e;

    if ((x == NIL) || (x != z)) {
        return NULL;
    }

    e = z->entry_;
    y = z;
    y_original_color = y->color_;

    if (z->left_ == NIL) {
        x = z->right_;
        __rbtree_transplant(self, z, z->right_);
    } else if (z->right_ == NIL) {
        x = z->left_;
        __rbtree_transplant(self, z, z->left_);
    } else {
        y = z->right_;

        while (y->left_ != NIL) {
            y = y->left_;
        }

        y_original_color = y->color_;
        x = y->right_;

        if (y->parent_ == z) {
            x->parent_ = y;
        } else {
            __rbtree_transplant(self, y, y->right_);
            y->right_ = z->right_;
            y->right_->parent_ = y;
        }

        __rbtree_transplant(self, z, y);
        y->left_ = z->left_;
        y->left_->parent_ = y;
        y->color_ = z->color_;
    }

    if (y_original_color == RBCOLOR_BLACK) {
        __rbtree_delete_fixup(self, x);
    }

    self->size_--;

    return e;
}

static inline void *rbtree_pop_front(struct rbtree *self)
{
    return rbtree_erase(self, rbtree_begin(self));
}

static inline void *rbtree_pop_back(struct rbtree *self)
{
    return rbtree_erase(self, rbtree_rbegin(self));
}

static inline void *rbtree_at(struct rbtree *self, size_t n)
{
    struct rbnode *x;

    for (x = rbtree_begin(self); n != 0 && x != rbtree_end(self); --n) {
        x = rbtree_next(self, x);
    }

    return x == rbtree_end(self) ? NULL : x->entry_;
}

static inline struct rbnode *rbtree_lower_bound(const struct rbtree *self,
                                                const void *ke)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *x = self->root_;
    struct rbnode *result = (struct rbnode *)NIL;

    while (x != NIL) {
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

static inline struct rbnode *rbtree_upper_bound(const struct rbtree *self,
                                                const void *ke)
{
    const struct rbnode *const NIL = rbtree_nil(self);
    struct rbnode *x = self->root_;
    struct rbnode *result = (struct rbnode *)NIL;

    while (x != NIL) {
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

static inline bool __rbtree_validate_node(const struct rbtree *self,
                                          const struct rbnode *x,
                                          size_t nr_black_expected,
                                          size_t nr_black_counted)
{
    const struct rbnode *NIL = rbtree_nil(self);

    if (x == NIL) {
        return nr_black_expected == nr_black_counted;
    }

    const struct rbnode *left = x->left_;
    const struct rbnode *right = x->right_;

    if (x->color_ == RBCOLOR_RED) {
        if (((left != NIL) && (left->color_ == RBCOLOR_RED)) ||
            ((right != NIL) && (right->color_ == RBCOLOR_RED))) {
            return false;
        }
    } else {
        nr_black_counted++;
    }

    return __rbtree_validate_node(self, left, nr_black_expected,
                                  nr_black_counted) &&
           __rbtree_validate_node(self, right, nr_black_expected,
                                  nr_black_counted);
}

static inline bool rbtree_validate(const struct rbtree *self)
{
    const struct rbnode *NIL = rbtree_nil(self);

    if (self->root_ == NIL) {
        return true;
    }

    if (self->root_->color_ != RBCOLOR_BLACK) {
        return false;
    }

    const struct rbnode *x = self->root_;
    size_t nr_black_expected = 0;

    while (x != NIL) {
        if (x->color_ == RBCOLOR_BLACK) {
            nr_black_expected++;
        }

        x = x->left_;
    }

    return __rbtree_validate_node(self, self->root_, nr_black_expected, 0);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_RBTREE_H__ */
