/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Boyer-Moore string search algorithm */
#ifndef __BOYERMOR_H__
#define __BOYERMOR_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ssize_t boyermor(const char *text, size_t text_len, size_t start,
                        const char *pattern, size_t pattern_len);

#ifdef __cplusplus
}
#endif
#endif /* __BOYERMOR_H__ */
