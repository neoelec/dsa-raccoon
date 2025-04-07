/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <rcn/instnsrt.h>

#include "mcpy.h"

#define __base(a) ((base) + (a) * (size))

void instnsrt(void *base, size_t nmemb, size_t size, compar_t compar)
{
    mcpy_t mcpy;
    size_t i;
    void *item;

    item = malloc(size);
    if (item == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for sorting\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    mcpy = __pick_mcpy(size);

    for (i = 1; i < nmemb; i++) {
        ssize_t loc = i - 1;

        mcpy(item, __base(i), size);

        while (loc >= 0 && (compar(item, __base(loc)) < 0)) {
            mcpy(__base(loc + 1), __base(loc), size);
            loc--;
        }

        mcpy(__base(loc + 1), item, size);
    }

    free(item);
}
