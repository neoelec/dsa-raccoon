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

struct dlist;

struct dlnode {
    const struct dlist *list_;
    struct dlnode *prev_;
    struct dlnode *next_;
    void *entry_;
};

struct dlist {
    struct dlnode pool_;
    size_t size_;
};

static inline void __dlnode_link_after(struct dlnode *pos, struct dlnode *x)
{
    x->prev_ = pos;
    x->next_ = pos->next_;
    pos->next_->prev_ = x;
    pos->next_ = x;
}

static inline void dlist_insert_after(struct dlnode *pos, struct dlnode *x,
                                      void *e)
{
    struct dlist *list = (struct dlist *)pos->list_;

    list->size_++;
    x->list_ = list;
    x->entry_ = e;
    __dlnode_link_after(pos, x);
}

static inline void __dlnode_link_before(struct dlnode *pos, struct dlnode *x)
{
    x->next_ = pos;
    x->prev_ = pos->prev_;
    pos->prev_->next_ = x;
    pos->prev_ = x;
}

static inline void dlist_insert_before(struct dlnode *pos, struct dlnode *x,
                                       void *e)
{
    struct dlist *list = (struct dlist *)pos->list_;

    list->size_++;
    x->list_ = list;
    x->entry_ = e;
    __dlnode_link_before(pos, x);
}

static void __dlnode_unlink(struct dlnode *x)
{
    x->prev_->next_ = x->next_;
    x->next_->prev_ = x->prev_;
}

static inline void *dlist_erase(struct dlnode *x)
{
    struct dlist *list = (struct dlist *)x->list_;

    list->size_--;
    x->list_ = NULL;
    __dlnode_unlink(x);

    return x->entry_;
}

static inline void dlist_clear(struct dlist *self)
{
    self->pool_.prev_ = self->pool_.next_ = &self->pool_;
    self->pool_.list_ = self;
    self->pool_.entry_ = NULL;
    self->size_ = 0;
}

static inline void dlist_init(struct dlist *self)
{
    dlist_clear(self);
}

static inline struct dlnode *dlist_begin(const struct dlist *self)
{
    return self->pool_.next_;
}

static inline struct dlnode *dlist_rbegin(const struct dlist *self)
{
    return self->pool_.prev_;
}

static inline const struct dlnode *dlist_end(const struct dlist *self)
{
    return &self->pool_;
}

static inline const struct dlnode *dlist_rend(const struct dlist *self)
{
    return &self->pool_;
}

static inline size_t dlist_size(const struct dlist *self)
{
    return self->size_;
}

static inline bool dlist_empty(const struct dlist *self)
{
    return dlist_size(self) == 0;
}

static inline void *dlist_front(const struct dlist *self)
{
    return dlist_empty(self) ? NULL : dlist_begin(self)->entry_;
}

static inline void *dlist_back(const struct dlist *self)
{
    return dlist_empty(self) ? NULL : dlist_rbegin(self)->entry_;
}

static inline void dlist_push_front(struct dlist *self, struct dlnode *x,
                                    void *e)
{
    dlist_insert_after(&self->pool_, x, e);
}

static inline void dlist_push_back(struct dlist *self, struct dlnode *x,
                                   void *e)
{
    dlist_insert_before(&self->pool_, x, e);
}

static inline void *dlist_pop_front(struct dlist *self)
{
    return dlist_empty(self) ? NULL : dlist_erase(dlist_begin(self));
}

static inline void *dlist_pop_back(struct dlist *self)
{
    return dlist_empty(self) ? NULL : dlist_erase(dlist_rbegin(self));
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
    struct dlnode _y, *y = &_y;
    size_t size;

    size = self->size_;
    self->size_ = other->size_;
    other->size_ = size;

    __dlnode_link_after(&self->pool_, y);
    __dlnode_unlink(&self->pool_);
    __dlnode_link_after(&other->pool_, &self->pool_);
    __dlnode_unlink(&other->pool_);
    __dlnode_link_after(y, &other->pool_);
    __dlnode_unlink(y);

    for (struct dlnode *x = dlist_begin(self); x != dlist_end(self);
         x = x->next_) {
        x->list_ = self;
    }

    for (struct dlnode *x = dlist_begin(other); x != dlist_end(other);
         x = x->next_) {
        x->list_ = other;
    }
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
    struct dlnode _y, *y = &_y;

    y->list_ = NULL;
    y->next_ = y->prev_ = y;

    for (size_t i = dlist_size(self); i > 0; --i) {
        struct dlnode *x = dlist_begin(self);

        __dlnode_unlink(x);
        __dlnode_link_after(y, x);
    }

    __dlnode_link_after(y, &self->pool_);
    __dlnode_unlink(y);
}

static inline bool dlist_validate(const struct dlist *self)
{
    size_t size = 0;

    for (const struct dlnode *x = dlist_begin(self); x != dlist_end(self);
         x = x->next_) {
        if (x->list_ != self) {
            return false;
        }

        if ((x->prev_->next_ != x) || (x->next_->prev_ != x)) {
            return false;
        }

        size++;
    }

    return dlist_size(self) == size ? true : false;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_DLIST_H__ */
