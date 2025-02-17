/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <rcn/brtfsrch.h>

static ssize_t ____brtfsrch(const char *text, ssize_t text_len, ssize_t start,
                            const char *pattern, ssize_t pattern_len)
{
    ssize_t i;

    for (i = start; i <= text_len - pattern_len; i++) {
        ssize_t j;

        for (j = 0; j < pattern_len; j++)
            if (text[i + j] != pattern[j])
                break;

        if (j >= pattern_len)
            return i;
    }

    return -1;
}

static ssize_t __brtfsrch(const char *text, ssize_t text_len, ssize_t start,
                          const char *pattern, ssize_t pattern_len)
{
    assert(text_len >= 0);
    assert(start >= 0);
    assert(pattern_len >= 0);

    return ____brtfsrch(text, text_len, start, pattern, pattern_len);
}

ssize_t brtfsrch(const char *text, size_t text_len, size_t start,
                 const char *pattern, size_t pattern_len)
{
    return __brtfsrch(text, text_len, start, pattern, pattern_len);
}
