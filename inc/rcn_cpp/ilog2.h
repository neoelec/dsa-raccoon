/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 *
 * Reference:
 *  - https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
 */

/* log2 */
#ifndef __RCN_CPP_ILOG2_H__
#define __RCN_CPP_ILOG2_H__

#ifdef __cplusplus
namespace rcn_cpp
{
#endif

static inline unsigned int __ilog32(unsigned int x)
{
    static const unsigned int table[32] = { 0,  9,  1,  10, 13, 21, 2,  29,
                                            11, 14, 16, 18, 22, 25, 3,  30,
                                            8,  12, 20, 28, 15, 17, 24, 7,
                                            19, 27, 23, 6,  26, 5,  4,  31 };

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return table[(x * 0x07C4ACDD) >> 27];
}

static inline unsigned long long __ilog64(unsigned long long x)
{
    static const unsigned long long table[64] = {
        63, 0,  58, 1,  59, 47, 53, 2,  60, 39, 48, 27, 54, 33, 42, 3,
        61, 51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4,
        62, 57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19, 29, 10, 13, 21,
        56, 45, 25, 31, 35, 16, 9,  12, 44, 24, 15, 8,  23, 7,  6,  5
    };

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return table[(((x - (x >> 1ULL)) * 0x07EDD5E59A4E28C2)) >> 58ULL];
}

template <typename T>
static inline T ilog2(T x)
{
    return sizeof(T) == 8 ? __ilog64(x) : __ilog32(x);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_cpp */

#endif /* __RCN_CPP_ILOG2_H__ */
