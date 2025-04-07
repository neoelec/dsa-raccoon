/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rcn/slctnsrt.h>

#include "mcpy.h"

#define __base(a) ((base) + (a) * (size))

void slctnsrt(void *base, size_t nmemb, size_t size, compar_t compar)
{
    mcpy_t mcpy;
    size_t i;
    void *min;

    min = malloc(size);
    if (min == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for sorting\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    mcpy = __pick_mcpy(size);

    for (i = 0; i < nmemb - 1; i++) {
        size_t j;
        size_t min_idx = i;

        mcpy(min, __base(i), size);

        for (j = i + 1; j < nmemb; j++) {
            if (compar(__base(j), min) < 0) {
                mcpy(min, __base(j), size);
                min_idx = j;
            }
        }

        mcpy(__base(min_idx), __base(i), size);
        mcpy(__base(i), min, size);
    }

    free(min);
}
