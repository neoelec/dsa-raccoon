/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <rcn/heapsort.h>

#include "mcpy.h"
#include "swap.h"
#include "utility.h"

#define __base(a) ((base) + (a) * (size))

static void __down_heap(void *base, size_t nmemb, size_t size, compar_t compar,
                        mcpy_t mcpy, size_t k, void *v)
{
    size_t i;

    mcpy(v, __base(k - 1), size);

    while (k <= nmemb / 2) {
        i = k << 1;

        if (i < nmemb && compar(__base(i - 1), __base(i)) < 0)
            i++;

        if (compar(v, __base(i - 1)) >= 0)
            break;

        mcpy(__base(k - 1), __base(i - 1), size);

        k = i;
    }

    mcpy(__base(k - 1), v, size);
}

void heapsort(void *base, size_t nmemb, size_t size, compar_t compar)
{
    mcpy_t mcpy;
    swap_t swap;
    size_t k;
    void *v;

    v = malloc(size);
    if (v == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for sorting\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    mcpy = __pick_mcpy(size);
    swap = __pick_swap(size);

    for (k = nmemb / 2; k >= 1; k--)
        __down_heap(base, nmemb, size, compar, mcpy, k, v);

    while (nmemb > 1) {
        swap(base, __base(nmemb - 1), size);
        __down_heap(base, --nmemb, size, compar, mcpy, 1, v);
    }

    free(v);
}