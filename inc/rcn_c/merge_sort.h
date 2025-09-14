/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Merge Sort */
#ifndef __RCN_C_MERGE_SORT_H__
#define __RCN_C_MERGE_SORT_H__

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static void __merge(void *base, void *sorted, size_t left, size_t mid,
                    size_t right, size_t size,
                    int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])
#define __sorted(n) (&((char *)sorted)[(n) * size])

    size_t i = left, j = mid + 1., k = left;

    while (i <= mid && j <= right) {
        if (compar(__base(i), __base(j)) <= 0) {
            memcpy(__sorted(k++), __base(i++), size);
        } else {
            memcpy(__sorted(k++), __base(j++), size);
        }
    }

    if (i > mid) {
        memcpy(__sorted(k), __base(j), size * (right - j + 1));
    } else {
        memcpy(__sorted(k), __base(i), size * (mid - i + 1));
    }

    memcpy(__base(left), __sorted(left), size * (right - left + 1));

#undef __base
#undef __sorted
}

static void __merge_sort(void *base, void *sorted, size_t left, size_t right,
                         size_t size,
                         int (*compar)(const void *a, const void *b))
{
    if (left < right) {
        size_t mid = (left + right) / 2;
        __merge_sort(base, sorted, left, mid, size, compar);
        __merge_sort(base, sorted, mid + 1, right, size, compar);
        __merge(base, sorted, left, mid, right, size, compar);
    }
}

static inline void merge_sort(void *base, size_t nmemb, size_t size,
                              int (*compar)(const void *a, const void *b))
{
    if (nmemb <= 1) {
        return;
    }

    void *sorted = calloc(nmemb, size);

    if (sorted == NULL) {
        return;
    }

    __merge_sort(base, sorted, 0, nmemb - 1, size, compar);
    free(sorted);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_MERGE_SORT_H__ */
