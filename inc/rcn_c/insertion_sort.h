/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Insertion Sort */
#ifndef __RCN_C_INSERTION_SORT_H__
#define __RCN_C_INSERTION_SORT_H__

#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline void __insertion_sort(void *base, void *item, size_t nmemb,
                                    size_t size,
                                    int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    for (size_t i = 1; i < nmemb; ++i) {
        ssize_t loc = i - 1;
        memcpy(item, __base(i), size);

        while (loc >= 0 && (compar(item, __base(loc)) < 0)) {
            memcpy(__base(loc + 1), __base(loc), size);
            loc--;
        }

        memcpy(__base(loc + 1), item, size);
    }

#undef __base
}

static inline void insertion_sort(void *base, size_t nmemb, size_t size,
                                  int (*compar)(const void *a, const void *b))
{
    char __item[size];
    void *item = __item;

    __insertion_sort(base, item, nmemb, size, compar);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_INSERTION_SORT_H__ */
