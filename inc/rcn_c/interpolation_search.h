/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Interpolation Search */
#ifndef __RCN_C_INTERPOLATION_SEARCH_H__
#define __RCN_C_INTERPOLATION_SEARCH_H__

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
__interpolation_search(const void *key, const void *base, size_t nmemb,
                       size_t size, int (*compar)(const void *a, const void *b))
{
    ssize_t left, mid, right;
    int diff;

    left = 0;
    right = nmemb - 1;

    while (left <= right && compar(key, __base(left)) >= 0 &&
           compar(key, __base(right)) <= 0) {
        mid = left + (float)(right - left) /
                         compar(__base(right), __base(left)) *
                         compar(key, __base(left));
        mid = mid < 0 ? 0 : mid;

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

static inline void *
interpolation_search(const void *key, const void *base, size_t nmemb,
                     size_t size, int (*compar)(const void *a, const void *b))
{
    ssize_t pos = __interpolation_search(key, base, nmemb, size, compar);

    return pos >= 0 ? __base(pos) : NULL;
}

static inline int
interpolation_delete(void *key, void *base, size_t *nmemb, size_t size,
                     int (*compar)(const void *a, const void *b))
{
    ssize_t pos;

    if ((pos = __interpolation_search(key, base, *nmemb, size, compar)) < 0) {
        return -EEXIST;
    }

    memcpy(__base(pos), __base(pos + 1), (*nmemb - pos) * size);
    (*nmemb)--;

    return 0;
}

#undef __base

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_INTERPOLATION_SEARCH_H__ */
