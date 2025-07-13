/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef __RCN_C_DJ2B_HASH_H__
#define __RCN_C_DJ2B_HASH_H__

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline unsigned long long djb2_hash(const char *str)
{
    unsigned long long hash = 5381;

    while (*str != '\0') {
        hash = (hash << 5) + hash + *str++;
    }

    return hash;
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_DJ2B_HASH_H__ */
