/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Selection Sort */
#ifndef __RCN_C_SELECTION_SORT_H__
#define __RCN_C_SELECTION_SORT_H__

#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline void __selection_sort(void *base, void *item, size_t nmemb,
                                    size_t size,
                                    int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    for (size_t i = 0; i < nmemb - 1; i++) {
        size_t min_index = i;

        memcpy(item, __base(i), size);

        for (size_t j = i + 1; j < nmemb; j++) {
            if (compar(item, __base(j)) > 0) {
                memcpy(item, __base(j), size);
                min_index = j;
            }
        }

        if (min_index != i) {
            memcpy(__base(min_index), __base(i), size);
            memcpy(__base(i), item, size);
        }
    }

#undef __base
}

static inline void selection_sort(void *base, size_t nmemb, size_t size,
                                  int (*compar)(const void *a, const void *b))
{
    char __item[size];
    void *item = __item;

    if (nmemb == 0) {
        return;
    }

    __selection_sort(base, item, nmemb, size, compar);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_SELECTION_SORT_H__ */
