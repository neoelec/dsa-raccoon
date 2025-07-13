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

struct slnode {
    struct slnode *next_;
    void *entry_;
};

static inline void __slist_insert_after(struct slnode *pos, struct slnode *x)
{
    x->next_ = pos->next_;
    pos->next_ = x;
}

static inline void slist_insert_after(struct slnode *pos, struct slnode *x,
                                      void *e)
{
    x->entry_ = e;
    __slist_insert_after(pos, x);
}

static inline void *slist_erase_after(struct slnode *pos)
{
    if (pos->next_ == NULL) {
        return NULL;
    }

    struct slnode *x = pos->next_;
    pos->next_ = x->next_;
    return x->entry_;
}

struct slist {
    struct slnode pool_;
};

static inline void slist_clear(struct slist *self)
{
    self->pool_.next_ = NULL;
}

static inline void slist_init(struct slist *self)
{
    slist_clear(self);
}

static inline struct slnode *slist_begin(const struct slist *self)
{
    return self->pool_.next_;
}

static inline struct slnode *slist_end(const struct slist *self)
{
    return NULL;
}

static inline bool slist_empty(const struct slist *self)
{
    return slist_begin(self) == NULL;
}

static inline size_t slist_size(const struct slist *self)
{
    struct slnode *x;
    size_t count = 0;

    for (x = slist_begin(self); x != slist_end(self); x = x->next_) {
        count++;
    }

    return count;
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
    return slist_erase_after(&self->pool_);
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
    struct slnode tmp;

    tmp.next_ = slist_begin(self);
    self->pool_.next_ = slist_begin(other);
    other->pool_.next_ = tmp.next_;
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
    struct slnode head = { .next_ = NULL, .entry_ = NULL };

    while (!slist_empty(self)) {
        struct slnode *x = slist_begin(self);
        slist_pop_front(self);
        __slist_insert_after(&head, x);
    }

    self->pool_.next_ = head.next_;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_SLIST_H__ */
