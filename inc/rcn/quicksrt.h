/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Quick Sort */
#ifndef __QUICKSRT_H__
#define __QUICKSRT_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void quicksrt(void *base, size_t nmemb, size_t size, compar_t compar);
extern void quicksrt_m(void *base, size_t nmemb, size_t size, compar_t compar);
extern void quicksrt_r(void *base, size_t nmemb, size_t size, compar_t compar);
extern void quicksrt_s(void *base, size_t nmemb, size_t size, compar_t compar);

#ifdef __cplusplus
}
#endif
#endif /* __QUICKSRT_H__ */
