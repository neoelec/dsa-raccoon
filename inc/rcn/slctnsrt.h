/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Selection Sort */
#ifndef __SLCTNSRT_H__
#define __SLCTNSRT_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void slctnsrt(void *base, size_t nmemb, size_t size, compar_t compar);

#ifdef __cplusplus
}
#endif
#endif /* __SLCTNSRT_H__ */
