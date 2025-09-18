/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linear Search */
#ifndef __RCN_C_LINEAR_SEARCH_H__
#define __RCN_C_LINEAR_SEARCH_H__

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
__linear_search(const void *key, const void *base, size_t nmemb, size_t size,
                int (*compar)(const void *a, const void *b))
{
    size_t pos = 0;

    while (pos < nmemb) {
        if (compar(key, __base(pos)) == 0) {
            return pos;
        }

        pos++;
    }

    return -EEXIST;
}

static inline void *linear_search(const void *key, const void *base,
                                  size_t nmemb, size_t size,
                                  int (*compar)(const void *a, const void *b))
{
    ssize_t pos = __linear_search(key, base, nmemb, size, compar);

    return pos >= 0 ? __base(pos) : NULL;
}

static inline void *linear_insert(void *key, void *base, size_t *nmemb,
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

    return __base(pos);
}

static inline int linear_delete(void *key, void *base, size_t *nmemb,
                                size_t size,
                                int (*compar)(const void *a, const void *b))
{
    ssize_t pos;

    if ((pos = __linear_search(key, base, *nmemb, size, compar)) < 0) {
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

#endif /* __RCN_C_LINEAR_SEARCH_H__ */
