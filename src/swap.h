/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef __RCN_SRC_INTERNAL__SWAP_H__
#define __RCN_SRC_INTERNAL__SWAP_H__

#include <stddef.h>
#include <stdint.h>

typedef void (*swap_t)(void *a, void *b, size_t size);

static void __swap(void *a, void *b, size_t size)
{
    uint8_t tmp;
    size_t i;

    for (i = 0; i < size; i++, a++, b++) {
        tmp = *(uint8_t *)a;
        *(uint8_t *)a = *(uint8_t *)b;
        *(uint8_t *)b = tmp;
    }
}

static void __swap_1_byte(void *a, void *b, size_t size)
{
    uint8_t tmp;

    tmp = *(uint8_t *)a;
    *(uint8_t *)a = *(uint8_t *)b;
    *(uint8_t *)b = tmp;
}

static void __swap_2_bytes(void *a, void *b, size_t size)
{
    uint16_t tmp;

    tmp = *(uint16_t *)a;
    *(uint16_t *)a = *(uint16_t *)b;
    *(uint16_t *)b = tmp;
}

static void __swap_4_bytes(void *a, void *b, size_t size)
{
    uint32_t tmp;

    tmp = *(uint32_t *)a;
    *(uint32_t *)a = *(uint32_t *)b;
    *(uint32_t *)b = tmp;
}

static void __swap_8_bytes(void *a, void *b, size_t size)
{
    uint64_t tmp;

    tmp = *(uint64_t *)a;
    *(uint64_t *)a = *(uint64_t *)b;
    *(uint64_t *)b = tmp;
}

static inline swap_t __pick_swap(size_t size)
{
    if (size == 8)
        return __swap_8_bytes;
    else if (size == 4)
        return __swap_4_bytes;
    else if (size == 2)
        return __swap_2_bytes;
    else if (size == 1)
        return __swap_1_byte;

    return __swap;
}

#endif /* __RCN_SRC_INTERNAL__SWAP_H__ */
