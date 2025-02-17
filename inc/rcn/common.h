/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef __RCN_COMMON_H__
#define __RCN_COMMON_H__

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)(((char *)ptr) - offsetof(type, member)))
#endif

#ifndef NR_ELEM
#define NR_ELEM(a) ((sizeof(a)) / (sizeof((a)[0])))
#endif

typedef void (*do_each_t)(void *a, void *b);
/* a == key , b == base */
typedef int (*compar_t)(const void *a, const void *b);

#endif /* __RCN_COMMON_H__ */
