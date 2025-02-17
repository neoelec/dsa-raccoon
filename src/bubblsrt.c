/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdbool.h>

#include <rcn/bubblsrt.h>

#include "swap.h"
#include "utility.h"

#define __base(a) ((base) + (a) * (size))

void bubblsrt(void *base, size_t nmemb, size_t size, compar_t compar)
{
    swap_t swap;
    size_t i, j;
    bool updated;

    swap = __pick_swap(size);

    for (i = 0; i < nmemb - 1; i++) {
        updated = false;

        for (j = 1; j < nmemb - i; j++) {
            if (compar(__base(j - 1), __base(j)) > 0) {
                swap(__base(j - 1), __base(j), size);
                updated = true;
            }
        }

        if (updated == false)
            break;
    }
}
