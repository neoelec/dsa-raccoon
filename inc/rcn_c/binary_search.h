/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Binary Search */
#ifndef __RCN_C_BINARY_SEARCH_H__
#define __RCN_C_BINARY_SEARCH_H__

#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

#define __base(n) (&((char *)base)[(n) * size])

static inline ssize_t
__binary_search(const void *key, const void *base, size_t nmemb, size_t size,
                int (*compar)(const void *a, const void *b))
{
    ssize_t left, mid, right;
    int diff;

    left = 0;
    right = nmemb - 1;

    while (left <= right) {
        mid = (left + right) / 2;
        diff = compar(key, __base(mid));

        if (diff == 0) {
            return mid;
        } else if (diff > 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -EEXIST;
}

static inline void *binary_search(const void *key, const void *base,
                                  size_t nmemb, size_t size,
                                  int (*compar)(const void *a, const void *b))
{
    ssize_t pos = __binary_search(key, base, nmemb, size, compar);

    return pos >= 0 ? __base(pos) : NULL;
}

static inline size_t binary_insert(void *key, void *base, size_t *nmemb,
                                   size_t size,
                                   int (*compar)(const void *a, const void *b))
{
    size_t pos = 0;

    while (compar(key, __base(pos)) > 0 && pos < *nmemb) {
        pos++;
    }

    for (size_t i = *nmemb; i > pos; --i) {
        memcpy(__base(i), __base(i - 1), size);
    }

    memcpy(__base(pos), key, size);
    (*nmemb)++;

    return pos;
}

static inline ssize_t binary_delete(void *key, void *base, size_t *nmemb,
                                    size_t size,
                                    int (*compar)(const void *a, const void *b))
{
    ssize_t pos;

    if (*nmemb <= 0) {
        return -ERANGE;
    } else if ((pos = __binary_search(key, base, *nmemb, size, compar)) < 0) {
        return -EEXIST;
    }

    memcpy(__base(pos), __base(pos + 1), (*nmemb - pos) * size);
    (*nmemb)--;

    return pos;
}

#undef __base

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_BINARY_SEARCH_H__ */
