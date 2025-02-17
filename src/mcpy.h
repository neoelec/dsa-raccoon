/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef __RCN_SRC_INTERNAL__MCPY_H__
#define __RCN_SRC_INTERNAL__MCPY_H__

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef void *(*mcpy_t)(void *a, const void *b, size_t size);

static void *__mcpy_1_byte(void *a, const void *b, size_t size)
{
    *(uint8_t *)a = *(uint8_t *)b;

    return a;
}

static void *__mcpy_2_bytes(void *a, const void *b, size_t size)
{
    *(uint16_t *)a = *(uint16_t *)b;

    return a;
}

static void *__mcpy_4_bytes(void *a, const void *b, size_t size)
{
    *(uint32_t *)a = *(uint32_t *)b;

    return a;
}

static void *__mcpy_8_bytes(void *a, const void *b, size_t size)
{
    *(uint64_t *)a = *(uint64_t *)b;

    return a;
}

static inline mcpy_t __pick_mcpy(size_t size)
{
    if (size == 8)
        return __mcpy_8_bytes;
    else if (size == 4)
        return __mcpy_4_bytes;
    else if (size == 2)
        return __mcpy_2_bytes;
    else if (size == 1)
        return __mcpy_1_byte;
    else
        return memcpy;
}

#endif /* __RCN_SRC_INTERNAL__MCPY_H__ */
