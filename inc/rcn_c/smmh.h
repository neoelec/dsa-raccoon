/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 *
 * Reference : https://constructionsite.tistory.com/48
 */

/* Symmetric Min Max Heap (Array) */
#ifndef __RCN_C_SMMH_H__
#define __RCN_C_SMMH_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define SMMH_TEMP_IDX 0
#define SMMH_ROOT 1
#define SMMH_ROOT_MIN 2
#define SMMH_ROOT_MAX 3

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct smmh {
    size_t nr_entries_;
    int (*compar_)(const void *ke, const void *in_heap);
    void **entry_;
    size_t idx_;
};

static inline size_t __smmh_grandpa(size_t pos)
{
    return pos / 4;
}

static inline size_t __smmh_left(size_t pos)
{
    return pos * 2;
}

static inline int __smmh_compar(const struct smmh *self, size_t ke_i,
                                size_t in_heap_i)
{
    return self->compar_(self->entry_[ke_i], self->entry_[in_heap_i]);
}

static inline void smmh_clear(struct smmh *self)
{
    self->idx_ = SMMH_ROOT;

    if (self->entry_ != NULL) {
        self->entry_[SMMH_TEMP_IDX] = self->entry_[SMMH_ROOT] = NULL;
    }
}

static inline void smmh_init(struct smmh *self, size_t nr_entries,
                             int (*compar)(const void *ke, const void *in_heap),
                             void **entry)
{
    self->nr_entries_ = nr_entries;
    self->compar_ = compar;
    self->entry_ = entry;
    smmh_clear(self);
}

static inline size_t smmh_size(const struct smmh *self)
{
    return self->idx_ - 1; /* root is always empty */
}

static inline size_t smmh_max_size(const struct smmh *self)
{
    return self->nr_entries_ - 2; /* '0' and '1' are not used for smmh */
}

static inline bool smmh_empty(const struct smmh *self)
{
    return smmh_size(self) == 0;
}

static inline bool smmh_full(const struct smmh *self)
{
    return smmh_size(self) == smmh_max_size(self);
}

static inline void *smmh_front(const struct smmh *self)
{
    if (smmh_empty(self)) {
        return NULL;
    } else {
        return self->entry_[SMMH_ROOT_MIN];
    }
}

static inline void *smmh_back(const struct smmh *self)
{
    if (smmh_empty(self)) {
        return NULL;
    } else if (self->idx_ == SMMH_ROOT_MIN) {
        return self->entry_[SMMH_ROOT_MIN];
    } else {
        return self->entry_[SMMH_ROOT_MAX];
    }
}

static inline int smmh_push(struct smmh *self, void *e)
{
    if (smmh_full(self)) {
        return -ENOSPC;
    }

    size_t pos = ++self->idx_;

    self->entry_[SMMH_TEMP_IDX] = e;

    if ((pos % 2 == 1) && (__smmh_compar(self, SMMH_TEMP_IDX, pos - 1) < 0)) {
        self->entry_[pos] = self->entry_[pos - 1];
        pos = pos - 1;
    }

    while (4 <= pos) {
        size_t grandpa = __smmh_grandpa(pos);
        size_t left_of_grandpa = __smmh_left(grandpa);
        size_t right_of_grandpa = left_of_grandpa + 1;

        if (__smmh_compar(self, SMMH_TEMP_IDX, left_of_grandpa) < 0) {
            self->entry_[pos] = self->entry_[left_of_grandpa];
            pos = left_of_grandpa;
        } else if (__smmh_compar(self, SMMH_TEMP_IDX, right_of_grandpa) > 0) {
            self->entry_[pos] = self->entry_[right_of_grandpa];
            pos = right_of_grandpa;
        } else {
            break;
        }
    }

    self->entry_[pos] = self->entry_[SMMH_TEMP_IDX];
    self->entry_[SMMH_TEMP_IDX] = NULL;

    return 0;
}

static inline void __smmh_pop_front(struct smmh *self)
{
    self->entry_[SMMH_TEMP_IDX] = self->entry_[self->idx_--];

    size_t pos = SMMH_ROOT_MIN;
    size_t min_child = __smmh_left(pos);

    while (min_child <= self->idx_) {
        size_t left_of_sibling = __smmh_left(pos + 1);

        if ((left_of_sibling <= self->idx_) &&
            (__smmh_compar(self, min_child, left_of_sibling) > 0)) {
            min_child = left_of_sibling;
        }

        if (__smmh_compar(self, SMMH_TEMP_IDX, min_child) > 0) {
            self->entry_[pos] = self->entry_[min_child];
            pos = min_child;

            if ((pos + 1 <= self->idx_) &&
                (__smmh_compar(self, SMMH_TEMP_IDX, pos + 1) > 0)) {
                self->entry_[pos] = self->entry_[pos + 1];
                self->entry_[pos + 1] = self->entry_[SMMH_TEMP_IDX];
                self->entry_[SMMH_TEMP_IDX] = self->entry_[pos];
            }
        } else {
            break;
        }

        min_child = __smmh_left(pos);
    }

    self->entry_[pos] = self->entry_[SMMH_TEMP_IDX];
    self->entry_[SMMH_TEMP_IDX] = NULL;
}

static inline void *smmh_pop_front(struct smmh *self)
{
    void *e = smmh_front(self);

    if (self->idx_ == SMMH_ROOT_MIN) {
        self->idx_--;
    } else if (self->idx_ > SMMH_ROOT_MIN) {
        __smmh_pop_front(self);
    }

    return e;
}

static inline void __smmh_pop_back(struct smmh *self)
{
    self->entry_[SMMH_TEMP_IDX] = self->entry_[self->idx_--];

    size_t pos = SMMH_ROOT_MAX;
    size_t max_child = __smmh_left(pos - 1) + 1;

    while (max_child <= self->idx_) {
        size_t right = __smmh_left(pos) + 1;

        if ((right <= self->idx_) &&
            (__smmh_compar(self, max_child, right) < 0)) {
            max_child = right;
        }

        if (__smmh_compar(self, SMMH_TEMP_IDX, max_child) < 0) {
            self->entry_[pos] = self->entry_[max_child];
            pos = max_child;

            if (__smmh_compar(self, SMMH_TEMP_IDX, pos - 1) < 0) {
                self->entry_[pos] = self->entry_[pos - 1];
                self->entry_[pos - 1] = self->entry_[SMMH_TEMP_IDX];
                self->entry_[SMMH_TEMP_IDX] = self->entry_[pos];
            }
        } else {
            break;
        }

        max_child = __smmh_left(pos - 1) + 1;
    }

    self->entry_[pos] = self->entry_[SMMH_TEMP_IDX];
    self->entry_[SMMH_TEMP_IDX] = NULL;
}

static inline void *smmh_pop_back(struct smmh *self)
{
    void *e = smmh_back(self);

    if (self->idx_ == SMMH_ROOT_MIN) {
        self->idx_--;
    } else if (self->idx_ >= SMMH_ROOT_MAX) {
        __smmh_pop_back(self);
    }

    return e;
}

static inline void smmh_swap(struct smmh *self, struct smmh *other)
{
    struct smmh tmp;

    memcpy(&tmp, self, sizeof(tmp));
    memcpy(self, other, sizeof(tmp));
    memcpy(other, &tmp, sizeof(tmp));
}

static inline int __smmh_alloc_entry(struct smmh *self)
{
    self->entry_ = (void **)calloc(self->nr_entries_, sizeof(*self->entry_));
    return self->entry_ == NULL ? -ENOMEM : 0;
}

static inline void __smmh_free_entry(struct smmh *self)
{
    free(self->entry_);
}

static inline bool smmh_validate(const struct smmh *self)
{
    for (size_t i = SMMH_ROOT_MIN; i <= self->idx_; ++i) {
        size_t grandpa = __smmh_grandpa(i);

        if (grandpa >= SMMH_ROOT_MIN) {
            if (((i % 4 == 0) || (i % 4 == 1)) &&
                (__smmh_compar(self, i, grandpa) > 0)) {
                return false;
            } else if (((i % 4 == 2) || (i % 4 == 3)) &&
                       (__smmh_compar(self, i, grandpa) < 0)) {
                return false;
            }
        }
    }

    return true;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_SMMH_H__ */
