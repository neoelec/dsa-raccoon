/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Doubly Linked List */
#ifndef __RCN_C_DLIST_H__
#define __RCN_C_DLIST_H__

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct dlnode {
    struct dlnode *prev_;
    struct dlnode *next_;
    void *entry_;
};

static inline void __dlist_insert_after(struct dlnode *pos, struct dlnode *x)
{
    x->prev_ = pos;
    x->next_ = pos->next_;
    pos->next_->prev_ = x;
    pos->next_ = x;
}

static inline void dlist_insert_after(struct dlnode *pos, struct dlnode *x,
                                      void *e)
{
    x->entry_ = e;
    __dlist_insert_after(pos, x);
}

static inline void *dlist_erase(struct dlnode *pos)
{
    pos->prev_->next_ = pos->next_;
    pos->next_->prev_ = pos->prev_;
    return pos->entry_;
}

struct dlist {
    struct dlnode pool_;
};

static inline void dlist_clear(struct dlist *self)
{
    self->pool_.prev_ = self->pool_.next_ = &self->pool_;
}

static inline void dlist_init(struct dlist *self)
{
    dlist_clear(self);
}

static inline struct dlnode *dlist_begin(const struct dlist *self)
{
    return self->pool_.next_;
}

static inline const struct dlnode *dlist_end(const struct dlist *self)
{
    return &self->pool_;
}

static inline bool dlist_empty(const struct dlist *self)
{
    return dlist_begin(self) == &self->pool_;
}

static inline size_t dlist_size(const struct dlist *self)
{
    struct dlnode *x;
    size_t count = 0;
    for (x = dlist_begin(self); x != dlist_end(self); x = x->next_) {
        count++;
    }
    return count;
}

static inline void *dlist_front(const struct dlist *self)
{
    return dlist_empty(self) ? NULL : dlist_begin(self)->entry_;
}

static inline void *dlist_back(const struct dlist *self)
{
    return dlist_empty(self) ? NULL : dlist_end(self)->prev_->entry_;
}

static inline void dlist_push_front(struct dlist *self, struct dlnode *x,
                                    void *e)
{
    dlist_insert_after(&self->pool_, x, e);
}

static inline void dlist_push_back(struct dlist *self, struct dlnode *x,
                                   void *e)
{
    dlist_insert_after(dlist_end(self)->prev_, x, e);
}

static inline void *dlist_pop_front(struct dlist *self)
{
    return dlist_erase(dlist_begin(self));
}

static inline void *dlist_pop_back(struct dlist *self)
{
    return dlist_erase(dlist_end(self)->prev_);
}

static inline void *dlist_at(const struct dlist *self, size_t n)
{
    struct dlnode *x;
    for (x = dlist_begin(self); n != 0 && x != dlist_end(self); --n) {
        x = x->next_;
    }
    return x == dlist_end(self) ? NULL : x->entry_;
}

static inline void dlist_swap(struct dlist *self, struct dlist *other)
{
    struct dlnode tmp;
    __dlist_insert_after(&self->pool_, &tmp);
    dlist_erase(&self->pool_);
    __dlist_insert_after(&other->pool_, &self->pool_);
    dlist_erase(&other->pool_);
    __dlist_insert_after(&tmp, &other->pool_);
    dlist_erase(&tmp);
}

static inline struct dlnode *dlist_remove_if(struct dlist *self, const void *ke,
                                             int (*compar)(const void *ke,
                                                           const void *e))
{
    struct dlnode *x;
    for (x = dlist_begin(self); x != dlist_end(self); x = x->next_) {
        if (compar(ke, x->entry_) == 0) {
            dlist_erase(x);
            return x;
        }
    }
    return NULL;
}

static inline void dlist_reverse(struct dlist *self)
{
    struct dlnode tmp = { .prev_ = &tmp, .next_ = &tmp, .entry_ = NULL };
    while (!dlist_empty(self)) {
        struct dlnode *x = dlist_begin(self);
        dlist_pop_front(self);
        __dlist_insert_after(&tmp, x);
    }
    __dlist_insert_after(&tmp, &self->pool_);
    dlist_erase(&tmp);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_DLIST_H__ */
