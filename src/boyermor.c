/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>

#include <rcn/boyermor.h>

#include "utility.h"

#define ALPHABET_LEN 256

static void __build_bct(const char *pattern, ssize_t pattern_len, ssize_t *bct)
{
    ssize_t i, j;

    for (i = 0; i < ALPHABET_LEN; i++)
        bct[i] = -1;

    for (j = 0; j < pattern_len; j++)
        bct[(size_t)pattern[j]] = j;
}

static void __build_gst(const char *pattern, ssize_t pattern_len,
                        ssize_t *suffix, ssize_t *gst)
{
    /* Case 1 */
    ssize_t i = pattern_len;
    ssize_t j = pattern_len + 1;

    suffix[i] = j;

    while (i > 0) {
        while (j <= pattern_len && pattern[i - 1] != pattern[j - 1]) {
            if (gst[j] == 0)
                gst[j] = j - i;

            j = suffix[j];
        }

        i--;
        j--;

        suffix[i] = j;
    }

    /* Case 2 */
    j = suffix[0];

    for (i = 0; i <= pattern_len; i++) {
        if (gst[i] == 0)
            gst[i] = j;

        if (i == j)
            j = suffix[j];
    }
}

static ssize_t ____boyermor(const char *text, ssize_t text_len, ssize_t start,
                            const char *pattern, ssize_t pattern_len)
{
    ssize_t bct[ALPHABET_LEN];
    ssize_t *suffix;
    ssize_t *gst;
    ssize_t i = start;
    ssize_t pos = -1;

    suffix = calloc(pattern_len + 1, sizeof(*suffix));
    gst = calloc(pattern_len + 1, sizeof(*gst));
    if (suffix == NULL || gst == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for buffer\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    __build_bct(pattern, pattern_len, bct);
    __build_gst(pattern, pattern_len, suffix, gst);

    while (i <= text_len - pattern_len) {
        ssize_t j = pattern_len - 1;

        while (j >= 0 && pattern[j] == text[i + j])
            j--;

        if (j < 0) {
            pos = i;
            break;
        } else {
            i += MAX(gst[j + 1], j - bct[(size_t)text[i + j]]);
        }
    }

    free(suffix);
    free(gst);

    return pos;
}

static ssize_t __boyermor(const char *text, ssize_t text_len, ssize_t start,
                          const char *pattern, ssize_t pattern_len)
{
    assert(text_len >= 0);
    assert(start >= 0);
    assert(pattern_len >= 0);

    return ____boyermor(text, text_len, start, pattern, pattern_len);
}

ssize_t boyermor(const char *text, size_t text_len, size_t start,
                 const char *pattern, size_t pattern_len)
{
    return __boyermor(text, text_len, start, pattern, pattern_len);
}
