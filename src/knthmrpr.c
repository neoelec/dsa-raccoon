/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>

#include <rcn/knthmrpr.h>

static void __preprocess(const char *pattern, ssize_t pattern_len,
                         ssize_t *border)
{
    ssize_t i = 0;
    ssize_t j = -1;

    border[0] = -1;

    while (i < pattern_len) {
        while (j > -1 && pattern[i] != pattern[j])
            j = border[j];

        i++;
        j++;

        border[i] = j;
    }
}

static ssize_t ____knthmrpr(const char *text, ssize_t text_len, ssize_t start,
                            const char *pattern, ssize_t pattern_len)
{
    ssize_t i = start;
    ssize_t j = 0;
    ssize_t pos = -1;
    ssize_t *border;

    border = calloc(pattern_len + 1, sizeof(*border));
    if (border == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for border array\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    __preprocess(pattern, pattern_len, border);

    while (i < text_len) {
        while (j >= 0 && text[i] != pattern[j])
            j = border[j];

        i++;
        j++;

        if (j == pattern_len) {
            pos = i - j;
            break;
        }
    }

    free(border);

    return pos;
}
static ssize_t __knthmrpr(const char *text, ssize_t text_len, ssize_t start,
                          const char *pattern, ssize_t pattern_len)
{
    assert(text_len >= 0);
    assert(start >= 0);
    assert(pattern_len >= 0);

    return ____knthmrpr(text, text_len, start, pattern, pattern_len);
}

ssize_t knthmrpr(const char *text, size_t text_len, size_t start,
                 const char *pattern, size_t pattern_len)
{
    return __knthmrpr(text, text_len, start, pattern, pattern_len);
}
