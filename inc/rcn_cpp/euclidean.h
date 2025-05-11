// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
//

// Euclidean algorithm
#ifndef __RCN_CPP_EUCLIDEAN_H__
#define __RCN_CPP_EUCLIDEAN_H__

namespace rcn_cpp
{

template <typename T>
static inline T euclidean_gcd(T a, T b)
{
    if (a < b) {
        T tmp = a;
        a = b;
        b = tmp;
    }
    while (b != 0) {
        T r = a % b;
        a = b;
        b = r;
    }
    return a;
}

}

#endif // __RCN_CPP_EUCLIDEAN_H__
