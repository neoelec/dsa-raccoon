/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Euclidean algorithm */
#ifndef __RCN_C_EUCLIDEAN_H__
#define __RCN_C_EUCLIDEAN_H__

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline unsigned long long euclidean_gcd(unsigned long long a,
                                               unsigned long long b)
{
    if (a < b) {
        unsigned long long tmp = a;
        a = b;
        b = tmp;
    }

    while (b != 0) {
        unsigned long long r = a % b;
        a = b;
        b = r;
    }

    return a;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_EUCLIDEAN_H__ */
