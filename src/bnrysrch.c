/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include <rcn/bnrysrch.h>

#define __base(a) ((base) + (a) * (size))

void *bnrysrch(const void *key, const void *base, size_t nmemb, size_t size,
               compar_t compar)
{
    ssize_t left, right;

    left = 0;
    right = nmemb - 1;

    while (left <= right) {
        ssize_t mid = (left + right) / 2;
        int diff = compar(key, __base(mid));

        if (diff == 0)
            return (void *)__base(mid);
        else if (diff > 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return NULL;
}
