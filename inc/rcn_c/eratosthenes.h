/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Sieve of Eratosthenes */
#ifndef __RCN_C_ERATOSTHENES_H__
#define __RCN_C_ERATOSTHENES_H__

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline bool *eratosthenes(unsigned long long n)
{
    bool *sieve = (bool *)calloc(n + 1, sizeof(*sieve));

    for (unsigned long long j = 0; j <= n; ++j) {
        sieve[j] = true;
    }

    sieve[0] = sieve[1] = false;

    for (unsigned long long i = 2; i * i <= n; ++i) {
        if (!sieve[i]) {
            continue;
        }

        for (unsigned long long j = i + i; j <= n; j += i) {
            sieve[j] = false;
        }
    }

    return sieve;
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_ERATOSTHENES_H__ */
