/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef __RCN_C_COMMON_H__
#define __RCN_C_COMMON_H__

#include <stddef.h>

#define NR_ELEM(a) (sizeof(a) / sizeof(a[0]))

#define CONTAINER_OF(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })

#ifdef __cplusplus
namespace rcn_c
{
#endif

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_COMMON_H__ */
