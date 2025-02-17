/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Simple Hash Table */
#ifndef __SPHT_H__
#define __SPHT_H__

#include "common.h"

struct spht;
struct spht_bucket;

typedef size_t (*spht_hash_t)(const struct spht *table, const void *e);

struct spht {
    size_t nr_buckets;
    spht_hash_t key_hash;
    size_t count;
    struct spht_bucket *bucket;
};

#define SPHT_MIN_NR_BUCKETS 8

static inline bool spht_is_empty(const struct spht *table)
{
    return table->count == 0;
}

#ifdef __cplusplus
extern "C" {
#endif

extern void spht_initialize(struct spht *table, size_t nr_buckets,
                            spht_hash_t key_hash);
extern void spht_terminate(struct spht *table);
extern int spht_insert(struct spht *table, void *e);
extern int spht_remove(struct spht *table, const void *ke, void **_e);
extern int spht_search(const struct spht *table, const void *ke, void **_e);
extern void spht_traverse(struct spht *table, do_each_t do_each, void *priv);

#ifdef __cplusplus
}
#endif

static inline int spht_insert_i(struct spht *table, intptr_t v)
{
    return spht_insert(table, (void *)v);
}

static inline int spht_remove_i(struct spht *table, intptr_t kv, intptr_t *_v)
{
    return spht_remove(table, (void *)kv, (void **)_v);
}

static inline int spht_search_i(const struct spht *table, intptr_t kv,
                                intptr_t *_v)
{
    return spht_search(table, (void *)kv, (void **)_v);
}

#endif /* __SPHT_H__ */
