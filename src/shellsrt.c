/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <rcn/shellsrt.h>

#include "mcpy.h"
#include "utility.h"

#define __base(a) ((base) + (a) * (size))

static void __intvlsrt(void *base, size_t size, ssize_t begin, ssize_t end,
                       ssize_t intv, compar_t compar, mcpy_t mcpy)
{
    ssize_t i;
    void *min;

    min = malloc(size);
    if (min == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for sorting\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    for (i = begin + intv; i <= end; i = i + intv) {
        ssize_t j;

        mcpy(min, __base(i), size);

        j = i - intv;
        while (j >= begin) {
            if (compar(min, __base(j)) >= 0)
                break;

            mcpy(__base(j + intv), __base(j), size);
            j -= intv;
        }

        mcpy(__base(j + intv), min, size);
    }

    free(min);
}

void shellsrt(void *base, size_t nmemb, size_t size, compar_t compar)
{
    mcpy_t mcpy;
    size_t intv;
    size_t i;

    mcpy = __pick_mcpy(size);

    intv = nmemb / 2;
    while (intv >= 1) {
        for (i = 0; i < intv; i++)
            __intvlsrt(base, size, i, nmemb - 1, intv, compar, mcpy);

        intv /= 2;
    }
}
