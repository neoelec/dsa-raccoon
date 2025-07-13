/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* The Babylonian Method */
#ifndef __RCN_CPP_BABYLONIAN_SQRT_H__
#define __RCN_CPP_BABYLONIAN_SQRT_H__

namespace rcn_cpp
{
template <typename T>
static T babylonian_sqrt(T y, T tolerance)
{
    T x = y, x_next;

    if (y <= 0) {
        return 0;
    }

    do {
        x_next = (x + y / x) / 2;

        if (((x_next > x) ? x_next - x : x - x_next) <= tolerance) {
            break;
        }

        x = x_next;
    } while (true);

    return x_next;
}
}

#endif /* __RCN_CPP_BABYLONIAN_SQRT_H__ */
