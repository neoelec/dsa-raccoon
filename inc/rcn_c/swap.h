/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef __RCN_C_INTERNAL__SWAP_H__
#define __RCN_C_INTERNAL__SWAP_H__

#include <stddef.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline void swap(void *_a, void *_b, size_t size)
{
    char *a = (char *)_a;
    char *b = (char *)_b;
    for (size_t i = 0; i < size; ++i) {
        char tmp = a[i];
        a[i] = b[i];
        b[i] = tmp;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_INTERNAL__SWAP_H__ */
