/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* The Babylonian Method */
#ifndef __RCN_C_BABYLONIAN_SQRT_H__
#define __RCN_C_BABYLONIAN_SQRT_H__

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline double babylonian_sqrt(double y, double tolerance)
{
    double x = y, x_next;

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

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_BABYLONIAN_SQRT_H__ */
