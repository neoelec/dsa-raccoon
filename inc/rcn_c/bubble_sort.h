/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Bubble Sort */
#ifndef __RCN_C_BUBBLE_SORT_H__
#define __RCN_C_BUBBLE_SORT_H__

#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline void __bubble_sort(void *base, void *item, size_t nmemb,
                                 size_t size,
                                 int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    for (size_t i = 0; i < nmemb - 1; ++i) {
        bool done = true;

        for (size_t j = 1; j < nmemb - i; ++j) {
            if (compar(__base(j - 1), __base(j)) > 0) {
                memcpy(item, __base(j - 1), size);
                memcpy(__base(j - 1), __base(j), size);
                memcpy(__base(j), item, size);
                done = false;
            }
        }

        if (done) {
            break;
        }
    }

#undef __base
}

static inline void bubble_sort(void *base, size_t nmemb, size_t size,
                               int (*compar)(const void *a, const void *b))
{
    char __item[size];
    void *item = __item;

    if (nmemb == 0) {
        return;
    }

    __bubble_sort(base, item, nmemb, size, compar);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_BUBBLE_SORT_H__ */
