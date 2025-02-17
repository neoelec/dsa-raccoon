/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Radix Sort */
#ifndef __RADIXSRT_H__
#define __RADIXSRT_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void radixsrt(void *base, size_t nmemb, size_t size,
                     unsigned long long order);
#ifdef __cplusplus
}
#endif
#endif /* __RADIXSRT_H__ */
