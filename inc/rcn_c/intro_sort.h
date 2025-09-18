/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Intro Sort */
#ifndef __RCN_C_INTRO_SORT_H__
#define __RCN_C_INTRO_SORT_H__

#include "heap_sort.h"
#include "ilog2.h"
#include "insertion_sort.h"
#include "quick_sort.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

static void __intro_sort(void *base, ssize_t left, ssize_t right, size_t size,
                         ssize_t depth_limit,
                         int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    if (right - left < 16) {
        insertion_sort(__base(left), right - left + 1, size, compar);
    } else if (depth_limit == 0) {
        heap_sort(__base(left), right - left + 1, size, compar);
    } else {
        ssize_t j;

        depth_limit--;
        __median_of_three(base, left, right, size, compar);
        j = __partition(base, left, right, size, compar);
        __intro_sort(base, left, j - 1, size, depth_limit, compar);
        __intro_sort(base, j + 1, right, size, depth_limit, compar);
    }

#undef __base
}

static inline void intro_sort(void *base, size_t nmemb, size_t size,
                              int (*compar)(const void *a, const void *))
{
    ssize_t depth_limit = 2 * ilog2l((unsigned long)nmemb);

    __intro_sort(base, 0, nmemb - 1, size, depth_limit, compar);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_INTRO_SORT_H__ */
