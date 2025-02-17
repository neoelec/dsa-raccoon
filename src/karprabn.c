/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <rcn/karprabn.h>

static size_t __hash(const char *string, size_t len)
{
    size_t i;
    size_t hash_value = 0;

    for (i = 0; i < len; i++)
        hash_value += string[i] * (1 << (len - (1 + i)));

    return hash_value;
}

static size_t __re_hash(const char *string, int len, int start, int hash_prev,
                        int coefficient)
{
    if (start == 0)
        return hash_prev;

    return ((hash_prev - coefficient * string[start - 1]) * 2) +
           string[start + len - 1];
}

static ssize_t ____karprabin(const char *text, ssize_t text_len, ssize_t start,
                             const char *pattern, ssize_t pattern_len)
{
    ssize_t i;
    ssize_t coefficient = 1 << (pattern_len - 1);
    ssize_t hash_text;
    ssize_t hash_pattern;

    hash_text = __hash(text, pattern_len);
    hash_pattern = __hash(pattern, pattern_len);

    for (i = start; i <= text_len - pattern_len; i++) {
        hash_text = __re_hash(text, pattern_len, i, hash_text, coefficient);

        if (hash_pattern == hash_text) {
            ssize_t j;
            for (j = 0; j < pattern_len; j++) {
                if (text[i + j] != pattern[j])
                    break;
            }

            if (j >= pattern_len)
                return i;
        }
    }

    return -1;
}

static ssize_t __karprabin(const char *text, ssize_t text_len, ssize_t start,
                           const char *pattern, ssize_t pattern_len)
{
    assert(text_len >= 0);
    assert(start >= 0);
    assert(pattern_len >= 0);

    return ____karprabin(text, text_len, start, pattern, pattern_len);
}

ssize_t karprabn(const char *text, size_t text_len, size_t start,
                 const char *pattern, size_t pattern_len)
{
    return __karprabin(text, text_len, start, pattern, pattern_len);
}
