/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef __RCN_C_SDBM_HASH_H__
#define __RCN_C_SDBM_HASH_H__

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline unsigned long long sdbm_hash(const char *str)

{
    unsigned long long hash = 0;

    while (*str != '\0') {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_SDBM_HASH_H__ */
