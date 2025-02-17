/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Brute-Force string search algorithm */
#ifndef __BRTFSRCH_H__
#define __BRTFSRCH_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ssize_t brtfsrch(const char *text, size_t text_len, size_t start,
                        const char *pattern, size_t pattern_len);

#ifdef __cplusplus
}
#endif
#endif /* __BRTFSRCH_H__ */
