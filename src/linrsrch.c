/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdlib.h>
#include <sys/types.h>

#include <rcn/linrsrch.h>

#include "utility.h"

#define __base(a) ((base) + (a) * (size))

void *linrsrch(const void *key, const void *base, size_t nmemb, size_t size,
               compar_t compar)
{
    size_t i;

    for (i = 0; i < nmemb; i++) {
        const void *v;

        v = __base(i);
        if (compar(key, v) == 0)
            return (void *)v;
    }

    return NULL;
}
