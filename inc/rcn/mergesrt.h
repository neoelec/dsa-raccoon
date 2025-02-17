/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Merge Sort */
#ifndef __MERGESRT_H__
#define __MERGESRT_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void mergesrt(void *base, size_t nmemb, size_t size, compar_t compar);

#ifdef __cplusplus
}
#endif
#endif /* __MERGESRT_H__ */
