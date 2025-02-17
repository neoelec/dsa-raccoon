/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Open Addressing Hash Table */
#ifndef __OAHT_H__
#define __OAHT_H__

#include "common.h"

struct oaht;
struct oaht_bucket;

typedef size_t (*oaht_hash_t)(const struct oaht *table, const void *e);

struct oaht {
    size_t nr_buckets;
    oaht_hash_t key_hash;
    oaht_hash_t step_hash;
    compar_t compar;
    size_t count;
    struct oaht_bucket *bucket;
};

#define OAHT_BYTES(nr_buckets) \
    (sizeof(struct oht) + (nr_buckets) * (sizeof(struct oaht_head *)))

#define OAHT_MIN_NR_BUCKETS 2

static inline bool oaht_is_empty(const struct oaht *table)
{
    return table->count == 0;
}

#ifdef __cplusplus
extern "C" {
#endif

extern void oaht_initialize(struct oaht *table, size_t nr_buckets,
                            oaht_hash_t key_hash, oaht_hash_t step_hash,
                            compar_t compar);
extern void oaht_terminate(struct oaht *table);
extern int oaht_insert(struct oaht *table, void *e);
extern int oaht_remove(struct oaht *table, const void *ke, void **_e);
extern int oaht_search(const struct oaht *table, const void *ke, void **_e);
extern void oaht_traverse(struct oaht *table, do_each_t do_each, void *priv);
extern int oaht_expand(struct oaht *table, size_t threshold);
extern int oaht_shrink(struct oaht *table, size_t threshold);

#ifdef __cplusplus
}
#endif

static inline int oaht_insert_i(struct oaht *table, intptr_t v)
{
    return oaht_insert(table, (void *)v);
}

static inline int oaht_remove_i(struct oaht *table, intptr_t kv, intptr_t *_v)
{
    return oaht_remove(table, (void *)kv, (void **)_v);
}

static inline int oaht_search_i(const struct oaht *table, intptr_t kv,
                                intptr_t *_v)
{
    return oaht_search(table, (void *)kv, (void **)_v);
}

#endif /* __OAHT_H__ */
