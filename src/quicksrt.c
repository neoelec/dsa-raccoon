/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <rcn/quicksrt.h>

#include "swap.h"

#define __base(a) ((base) + (a) * (size))
#define __compar(a, b) compar(__base(a), __base(b))
#define __swap(a, b) swap(__base(a), __base(b), size)

static ssize_t __partition(void *base, size_t size, compar_t compar,
                           swap_t swap, ssize_t left, ssize_t right)
{
    ssize_t first = left;

    left++;
    while (left <= right) {
        while ((__compar(left, first) <= 0) && (left < right))
            left++;

        while ((__compar(right, first) > 0) && (left <= right))
            right--;

        if (left < right)
            __swap(left, right);
        else
            break;
    }

    __swap(first, right);

    return right;
}

static void __qsort(void *base, size_t size, compar_t compar, swap_t swap,
                    ssize_t left, ssize_t right)
{
    if (left < right) {
        ssize_t idx;

        idx = __partition(base, size, compar, swap, left, right);

        __qsort(base, size, compar, swap, left, idx - 1);
        __qsort(base, size, compar, swap, idx + 1, right);
    }
}

static size_t __median_index(void *base, size_t size, compar_t compar, size_t a,
                             size_t b, size_t c)
{
    if (__compar(a, b) < 0) {
        if (__compar(b, c) < 0)
            return b;
        else if (__compar(a, c) < 0)
            return c;
        else
            return a;
    } else {
        if (__compar(a, c) < 0)
            return a;
        else if (__compar(b, c) < 0)
            return c;
        else
            return b;
    }
}

static void __qsort_m(void *base, size_t size, compar_t compar, swap_t swap,
                      ssize_t left, ssize_t right)
{
    if (left < right) {
        ssize_t pos;
        ssize_t idx;

        pos =
            __median_index(base, size, compar, left, (left + right) / 2, right);
        if (pos != left && __compar(pos, left) > 0)
            __swap(left, pos);

        idx = __partition(base, size, compar, swap, left, right);

        __qsort_m(base, size, compar, swap, left, idx - 1);
        __qsort_m(base, size, compar, swap, idx + 1, right);
    }
}

static void __qsort_r(void *base, size_t size, compar_t compar, swap_t swap,
                      ssize_t left, ssize_t right, size_t r)
{
    if (left < right) {
        ssize_t pos;
        ssize_t idx;

        pos = left + r % (right - left + 1);
        if (pos != left && __compar(pos, left) > 0)
            __swap(left, pos);

        idx = __partition(base, size, compar, swap, left, right);

        __qsort_r(base, size, compar, swap, left, idx - 1, r);
        __qsort_r(base, size, compar, swap, idx + 1, right, r);
    }
}

static void __shuffle(void *base, size_t size, size_t nmemb, swap_t swap)
{
    size_t i;
    size_t r;

    r = rand();

    for (i = 0; i < nmemb; i++) {
        size_t j = i + r / (RAND_MAX / (nmemb - i) + 1);

        if (i != j)
            __swap(i, j);
    }
}

void quicksrt(void *base, size_t nmemb, size_t size, compar_t compar)
{
    swap_t swap;

    swap = __pick_swap(size);

    __qsort(base, size, compar, swap, 0, nmemb - 1);
}

void quicksrt_m(void *base, size_t nmemb, size_t size, compar_t compar)
{
    swap_t swap;

    swap = __pick_swap(size);

    __qsort_m(base, size, compar, swap, 0, nmemb - 1);
}

void quicksrt_r(void *base, size_t nmemb, size_t size, compar_t compar)
{
    swap_t swap;

    swap = __pick_swap(size);

    srand(time(NULL));
    __qsort_r(base, size, compar, swap, 0, nmemb - 1, rand());
}

void quicksrt_s(void *base, size_t nmemb, size_t size, compar_t compar)
{
    swap_t swap;

    swap = __pick_swap(size);

    srand(time(NULL));
    __shuffle(base, size, nmemb, swap);
    __qsort(base, size, compar, swap, 0, nmemb - 1);
}
