/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linear Search */
#ifndef __LINRSRCH_H__
#define __LINRSRCH_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void *linrsrch(const void *key, const void *base, size_t nmemb,
                      size_t size, compar_t compar);

#ifdef __cplusplus
}
#endif
#endif /* __LINRSRCH_H__ */
