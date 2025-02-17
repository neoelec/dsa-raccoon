/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Chaining Hash Table */
#ifndef __CHHT_H__
#define __CHHT_H__

#include "dbll.h"

struct chht;

typedef size_t (*chht_hash_t)(const struct chht *, const struct dbll_node *);
typedef int (*chht_compar_t)(const struct dbll_node *key,
                             const struct dbll_node *in_table);

struct chht {
    size_t nr_buckets;
    size_t (*key_hash)(const struct chht *, const struct dbll_node *);
    chht_compar_t compar;
    struct dbll_node bucket[];
};

#define CHHT_BYTES(nr_buckets) \
    (sizeof(struct chht) + (nr_buckets) * (sizeof(struct dbll_node)))

#define CHHT_MIN_NR_BUCKETS 8

static inline void chht_remove(struct chht *table, struct dbll_node *z)
{
    dbll_unlink(z);
}

#ifdef __cplusplus
extern "C" {
#endif

extern void chht_initialize(struct chht *table, size_t nr_buckets,
                            chht_hash_t key_hash, chht_compar_t compar);
extern void chht_insert(struct chht *table, struct dbll_node *x);
extern int chht_search(struct chht *table, const struct dbll_node *kx,
                       struct dbll_node **_x);
extern void chht_traverse(struct chht *table, dbll_do_each_t do_each,
                          void *priv);

#ifdef __cplusplus
}
#endif
#endif /* __CHHT_H__ */
