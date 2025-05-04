/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Binary Search */
#ifndef __RCN_C_BINARY_SEARCH_H__
#define __RCN_C_BINARY_SEARCH_H__

#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline void *binary_search(const void *key, const void *base,
                                  size_t nmemb, size_t size,
                                  int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])
    ssize_t left, mid, right;
    int diff;
    left = 0;
    right = nmemb - 1;
    while (left <= right) {
        mid = (left + right) / 2;
        diff = compar(key, __base(mid));
        if (diff == 0) {
            return (void *)__base(mid);
        } else if (diff > 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return NULL;
#undef __base
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_BINARY_SEARCH_H__ */
