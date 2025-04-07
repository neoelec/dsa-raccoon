/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <rcn/mergesrt.h>

#include "mcpy.h"

#define __base(a) ((base) + (a) * (size))
#define __mcpy(dest, d_idx, base, b_idx, size) \
    mcpy(dest + d_idx * size, base + b_idx * size, size)

void mergesrt(void *base, size_t nmemb, size_t size, compar_t compar)
{
    mcpy_t mcpy;
    size_t idx;
    void *dest;

    dest = calloc(nmemb, size);
    if (dest == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for sorting\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    mcpy = __pick_mcpy(size);

    for (idx = 1; idx < nmemb; idx <<= 1) {
        size_t first = -2 * idx;
        size_t second = first + idx;

        while (second + idx * 2 < nmemb) {
            size_t i, j, k;

            i = k = first = second + idx;
            j = second = first + idx;

            while (i < first + idx || (j < second + idx && j < nmemb)) {
                if (compar(__base(i), __base(j)) <= 0) {
                    if (i < first + idx)
                        __mcpy(dest, k++, base, i++, size);
                    else
                        __mcpy(dest, k++, base, j++, size);
                } else {
                    if (j < second + idx && j < nmemb)
                        __mcpy(dest, k++, base, j++, size);
                    else
                        __mcpy(dest, k++, base, i++, size);
                }
            }
        }

        memcpy(base, dest, nmemb * size);
    }

    free(dest);
}
