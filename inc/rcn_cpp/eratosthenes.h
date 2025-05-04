// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
//

// Sieve of Eratosthenes
#ifndef __RCN_CPP_ERATOSTHENES_H__
#define __RCN_CPP_ERATOSTHENES_H__

namespace rcn_cpp
{

template <typename T>
bool *eratosthenes(T n)
{
    bool *sieve = new bool[n + 1];
    for (T j = 0; j <= n; ++j) {
        sieve[j] = true;
    }
    sieve[0] = sieve[1] = false;
    for (T i = 2; i * i <= n; ++i) {
        if (!sieve[i]) {
            continue;
        }
        for (T j = i + i; j <= n; j += i) {
            sieve[j] = false;
        }
    }
    return sieve;
}

}

#endif // __RCN_CPP_ERATOSTHENES_H__
