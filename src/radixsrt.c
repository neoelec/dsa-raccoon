/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <rcn/radixsrt.h>

#include "utility.h"

#define __base(st, a) (((st)->base) + (a) * (st)->size)

struct radixsrt_state {
    void *base;
    ssize_t nmemb;
    size_t size;
    unsigned long long order;
    unsigned long long mask_ull;
    unsigned long long mask_radix;
};

static unsigned long long __mask_ull(size_t size)
{
    unsigned long long ull = 0;

    return (~ull) >> ((sizeof(ull) - (unsigned long long)size) * 8);
}

static void __init_state(struct radixsrt_state *st, void *base, size_t nmemb,
                         size_t size, unsigned long long order)
{
    st->base = base;
    st->nmemb = nmemb;
    st->size = size;
    st->order = order;

    st->mask_ull = __mask_ull(size);
    st->mask_radix = (1 << order) - 1;
}

static unsigned long long __get_max(const struct radixsrt_state *st)
{
    unsigned long long max = 0;
    unsigned long long tmp;
    size_t i;

    for (i = 0; i < st->nmemb / st->size; i++) {
        tmp = *(unsigned long long *)__base(st, i) & st->mask_ull;
        max = max > tmp ? max : tmp;
    }

    return max;
}

static unsigned long long __to_idx(const struct radixsrt_state *st, ssize_t i,
                                   unsigned long long shift)
{
    unsigned long long tmp;

    tmp = *(unsigned long long *)__base(st, i) & st->mask_ull;

    return (tmp >> shift) & st->mask_radix;
}

static void __countsrt(struct radixsrt_state *st, unsigned long long shift,
                       unsigned long long *output, unsigned long long *count)
{
    unsigned long long idx;
    ssize_t i;
    unsigned long long j;

    memset(count, 0, sizeof(*count) << st->order);

    for (i = 0; i < st->nmemb; i++) {
        idx = __to_idx(st, i, shift);
        count[idx]++;
    }

    for (j = 1; j <= st->mask_radix; j++)
        count[j] += count[j - 1];

    for (i = st->nmemb - 1; i >= 0; i--) {
        idx = __to_idx(st, i, shift);
        output[count[idx] - 1] = *(unsigned long long *)__base(st, i) &
                                 st->mask_ull;
        count[idx]--;
    }

    switch (st->size) {
    case 1:
        for (i = 0; i < st->nmemb; i++)
            *(uint8_t *)__base(st, i) = output[i];
        break;
    case 2:
        for (i = 0; i < st->nmemb; i++)
            *(uint16_t *)__base(st, i) = output[i];
        break;
    case 4:
        for (i = 0; i < st->nmemb; i++)
            *(uint32_t *)__base(st, i) = output[i];
        break;
    case 8:
        for (i = 0; i < st->nmemb; i++)
            *(uint64_t *)__base(st, i) = output[i];
        break;
    default:
        for (i = 0; i < st->nmemb; i++)
            memcpy(__base(st, i), &output[i], st->size);
        break;
    }
}

static void __radixsrt(struct radixsrt_state *st)
{
    unsigned long long max;
    unsigned long long *output;
    unsigned long long *count;
    unsigned long long shift;

    output = calloc(st->nmemb, sizeof(*output));
    count = calloc(1 << st->order, sizeof(*output));
    if (output == NULL || count == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for sorting\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    max = __get_max(st);
    for (shift = 0; (max >> shift) > 0; shift += st->order)
        __countsrt(st, shift, output, count);

    free(count);
    free(output);
}

void radixsrt(void *base, size_t nmemb, size_t size, unsigned long long order)
{
    struct radixsrt_state _st;
    struct radixsrt_state *st = &_st;

    assert(size <= sizeof(unsigned long long));
    assert(order > 0);

    __init_state(st, base, nmemb, size, order);
    __radixsrt(st);
}
