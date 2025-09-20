/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Singly Linked List */
#ifndef __RCN_C_SLIST_H__
#define __RCN_C_SLIST_H__

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct slist;

struct slnode {
    const struct slist *list_;
    struct slnode *next_;
    void *entry_;
};

struct slist {
    struct slnode pool_;
    size_t size_;
};

static inline void __slnode_link_after(struct slnode *pos, struct slnode *x)
{
    x->next_ = pos->next_;
    pos->next_ = x;
}

static inline void slist_insert_after(struct slnode *pos, struct slnode *x,
                                      void *e)
{
    struct slist *list = (struct slist *)pos->list_;

    list->size_++;
    x->list_ = list;
    x->entry_ = e;
    __slnode_link_after(pos, x);
}

static inline void __slnode_unlink_after(struct slnode *pos)
{
    struct slnode *x = pos->next_;

    pos->next_ = x->next_;
}

static inline void *slist_erase_after(struct slnode *pos)
{
    struct slist *list = (struct slist *)pos->list_;
    struct slnode *x = pos->next_;

    list->size_--;
    x->list_ = NULL;
    __slnode_unlink_after(pos);

    return x->entry_;
}

static inline void slist_clear(struct slist *self)
{
    self->pool_.next_ = &self->pool_;
    self->pool_.list_ = self;
    self->pool_.entry_ = NULL;
    self->size_ = 0;
}

static inline void slist_init(struct slist *self)
{
    slist_clear(self);
}

static inline struct slnode *slist_begin(const struct slist *self)
{
    return self->pool_.next_;
}

static inline const struct slnode *slist_end(const struct slist *self)
{
    return &self->pool_;
}

static inline size_t slist_size(const struct slist *self)
{
    return self->size_;
}

static inline bool slist_empty(const struct slist *self)
{
    return slist_size(self) == 0;
}

static inline void *slist_front(const struct slist *self)
{
    return slist_empty(self) ? NULL : slist_begin(self)->entry_;
}

static inline void slist_push_front(struct slist *self, struct slnode *x,
                                    void *e)
{
    slist_insert_after(&self->pool_, x, e);
}

static inline void *slist_pop_front(struct slist *self)
{
    return slist_empty(self) ? NULL : slist_erase_after(&self->pool_);
}

static inline void *slist_at(const struct slist *self, size_t n)
{
    struct slnode *x;

    for (x = slist_begin(self); n != 0 && x != slist_end(self); --n) {
        x = x->next_;
    }

    return x == slist_end(self) ? NULL : x->entry_;
}

static inline void slist_swap(struct slist *self, struct slist *other)
{
    struct slnode *x;
    size_t size;

    size = self->size_;
    self->size_ = other->size_;
    other->size_ = size;

    x = slist_begin(self);

    while (x->next_ != slist_end(self)) {
        x->list_ = other;
        x = x->next_;
    }

    x->next_ = &other->pool_;

    x = slist_begin(other);

    while (x->next_ != slist_end(other)) {
        x->list_ = self;
        x = x->next_;
    }

    x->next_ = &self->pool_;

    x = slist_begin(self);
    self->pool_.next_ = slist_begin(other);
    other->pool_.next_ = x;
}

static inline struct slnode *slist_remove_if(struct slist *self, const void *ke,
                                             int (*compar)(const void *ke,
                                                           const void *e))
{
    struct slnode *p, *x;

    p = &self->pool_;

    for (x = slist_begin(self); x != slist_end(self); p = x, x = x->next_) {
        if (compar(ke, x->entry_) == 0) {
            slist_erase_after(p);
            return x;
        }
    }

    return NULL;
}

static inline void slist_reverse(struct slist *self)
{
    struct slnode _y, *y = &_y;

    y->list_ = NULL;
    y->next_ = &self->pool_;

    for (size_t i = slist_size(self); i > 0; --i) {
        struct slnode *x = slist_begin(self);

        __slnode_unlink_after(&self->pool_);
        __slnode_link_after(y, x);
    }

    self->pool_.next_ = y->next_;
}

static inline bool slist_validate(const struct slist *self)
{
    size_t size = 0;

    for (const struct slnode *x = slist_begin(self); x != slist_end(self);
         x = x->next_) {
        if (x->list_ != self) {
            return false;
        }

        size++;
    }

    return slist_size(self) == size ? true : false;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_SLIST_H__ */
