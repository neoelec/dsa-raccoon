/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Tim Sort */
#ifndef __RCN_C_TIM_SORT_H__
#define __RCN_C_TIM_SORT_H__

#include "insertion_sort.h"
#include "merge_sort.h"

#ifndef TIM_SORT_RUN
#define TIM_SORT_RUN 32
#endif /* TIM_SORT_RUN */

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline void __tim_sort(void *base, void *sorted, size_t nmemb,
                              size_t width,
                              int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * width])
#define __tim_min(a, b) ((a) < (b) ? (a) : (b))

    for (size_t i = 0; i < nmemb; i += TIM_SORT_RUN) {
        insertion_sort(__base(i), __tim_min(TIM_SORT_RUN, nmemb - i), width,
                       compar);
    }

    for (size_t size = TIM_SORT_RUN; size < nmemb; size = 2 * size) {
        for (size_t left = 0; left < nmemb; left += 2 * size) {
            size_t mid = left + size - 1;
            size_t right = __tim_min((left + 2 * size - 1), (nmemb - 1));

            if (mid < right) {
                __merge(base, sorted, left, mid, right, width, compar);
            }
        }
    }

#undef __base
#undef __tim_min
}

static inline void tim_sort(void *base, size_t nmemb, size_t size,
                            int (*compar)(const void *a, const void *b))
{
    if (nmemb <= 1) {
        return;
    }

    void *sorted = calloc(nmemb, size);

    if (sorted == NULL) {
        return;
    }

    __tim_sort(base, sorted, nmemb, size, compar);
    free(sorted);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_TIM_SORT_H__ */
