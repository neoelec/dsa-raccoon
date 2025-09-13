/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Shell Sort */
#ifndef __RCN_C_SHELL_SORT_H__
#define __RCN_C_SHELL_SORT_H__

#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline void __shell_sort(void *base, void *item, size_t nmemb,
                                size_t size,
                                int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    size_t h;

    for (h = 1; h < nmemb; h = 3 * h + 1) {
    }

    for (h /= 3; h > 0; h /= 3) {
        for (size_t i = 0; i < h; i++) {
            for (size_t j = i + h; j < nmemb; j += h) {
                size_t k = j;

                memcpy(item, __base(j), size);

                while (k > h - 1 && compar(__base(k - h), item) > 0) {
                    memcpy(__base(k), __base(k - h), size);
                    k -= h;
                }

                memcpy(__base(k), item, size);
            }
        }
    }

#undef __base
}

static inline void shell_sort(void *base, size_t nmemb, size_t size,
                              int (*compar)(const void *a, const void *b))
{
    char __item[size];
    void *item = __item;

    __shell_sort(base, item, nmemb, size, compar);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_SHELL_SORT_H__ */
