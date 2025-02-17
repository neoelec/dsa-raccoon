/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Binary Search */
#ifndef __BNRYSRCH_H__
#define __BNRYSRCH_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void *bnrysrch(const void *key, const void *base, size_t nmemb,
                      size_t size, compar_t compar);

#ifdef __cplusplus
}
#endif
#endif /* __BNRYSRCH_H__ */
