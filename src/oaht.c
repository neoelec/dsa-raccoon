/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rcn/oaht.h>

#define __sz_bucket(table, nr_buckets) (nr_buckets * sizeof(*(table)->bucket))
#define __key_hash(table, e) (table)->key_hash(table, e)
#define __step_hash(table, e) (table)->step_hash(table, e)
#define __compar(table, key, in_table) (table)->compar(key, in_table)

struct oaht_bucket {
    union {
        bool is_used;
        unsigned long __reserved;
    };
    void *entry;
};

static int __initialize(struct oaht *table, size_t nr_buckets,
                        oaht_hash_t key_hash, oaht_hash_t step_hash,
                        compar_t compar)
{
    struct oaht_bucket *bucket;

    bucket = calloc(nr_buckets, __sz_bucket(table, nr_buckets));
    if (bucket == NULL)
        return -ENOMEM;

    table->nr_buckets = nr_buckets;
    table->key_hash = key_hash;
    table->step_hash = step_hash;
    table->compar = compar;
    table->count = 0;
    table->bucket = bucket;

    return 0;
}

static int __insert(struct oaht *table, void *e)
{
    struct oaht_bucket *bucket = table->bucket;
    size_t idx, step_size;

    step_size = __step_hash(table, e);
    idx = __key_hash(table, e);
    while (bucket[idx].is_used)
        idx = (idx + step_size) % table->nr_buckets;

    bucket[idx].entry = e;
    bucket[idx].is_used = true;

    table->count++;

    return 0;
}

static struct oaht_bucket *__find_used_bucket(const struct oaht *table,
                                              const void *ke)
{
    const struct oaht_bucket *bucket = table->bucket;
    size_t idx, idx_1st;
    size_t step_size;

    step_size = __step_hash(table, ke);
    idx_1st = __key_hash(table, ke);
    idx = idx_1st;

    do {
        if (bucket[idx].is_used && !__compar(table, ke, bucket[idx].entry))
            return (struct oaht_bucket *)&bucket[idx];

        idx = (idx + step_size) % table->nr_buckets;
    } while (idx != idx_1st);

    return NULL;
}

static int __remove(struct oaht *table, const void *ke, void **_e)
{
    struct oaht_bucket *bucket;

    bucket = __find_used_bucket(table, ke);
    if (bucket == NULL)
        return -ENOENT;

    if (_e != NULL)
        *_e = bucket->entry;

    bucket->is_used = false;

    table->count--;

    return 0;
}

static void __copy(const struct oaht *table, struct oaht *tmp_table)
{
    size_t i;

    for (i = 0; i < table->nr_buckets; i++) {
        struct oaht_bucket *bucket = &table->bucket[i];

        if (!bucket->is_used)
            continue;

        __insert(tmp_table, bucket->entry);
    }
}

static int __rehash(struct oaht *table, size_t nr_buckets)
{
    struct oaht __tmp_table;
    struct oaht *tmp_table = &__tmp_table;
    int err;

    err = __initialize(tmp_table, nr_buckets, table->key_hash, table->step_hash,
                       table->compar);
    if (err != 0)
        return err;

    __copy(table, tmp_table);
    free(table->bucket);
    memcpy(table, tmp_table, sizeof(*table));
    table->bucket = tmp_table->bucket;

    return 0;
}

static size_t __usage_rate(struct oaht *table)
{
    return (size_t)(((float)table->count * 100) / (float)table->nr_buckets);
}

void oaht_initialize(struct oaht *table, size_t nr_buckets,
                     oaht_hash_t key_hash, oaht_hash_t step_hash,
                     compar_t compar)
{
    int err;

    assert(nr_buckets >= OAHT_MIN_NR_BUCKETS);

    err = __initialize(table, nr_buckets, key_hash, step_hash, compar);
    if (err != 0) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for bucket\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void oaht_terminate(struct oaht *table)
{
    if (table->count > 0) {
        fprintf(stderr, "%s():%u: Table is not empty\n", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    free(table->bucket);
}

int oaht_insert(struct oaht *table, void *e)
{
    return __insert(table, e);
}

int oaht_remove(struct oaht *table, const void *ke, void **_e)
{
    return __remove(table, ke, _e);
}

int oaht_search(const struct oaht *table, const void *ke, void **_e)
{
    struct oaht_bucket *bucket;

    bucket = __find_used_bucket(table, ke);
    if (bucket == NULL)
        return -ENOENT;

    if (_e != NULL)
        *_e = bucket->entry;

    return 0;
}

void oaht_traverse(struct oaht *table, do_each_t do_each, void *priv)
{
    size_t i;

    for (i = 0; i < table->nr_buckets; i++) {
        struct oaht_bucket *bucket = &table->bucket[i];

        if (bucket->is_used)
            do_each(bucket->entry, priv);
    }
}

int oaht_expand(struct oaht *table, size_t threshold)
{
    assert(threshold < 100);

    if (__usage_rate(table) <= threshold)
        return 0;

    return __rehash(table, table->nr_buckets * 2);
}

int oaht_shrink(struct oaht *table, size_t threshold)
{
    assert(threshold < 100);

    if (table->nr_buckets < OAHT_MIN_NR_BUCKETS)
        return 0;

    if (__usage_rate(table) > threshold)
        return 0;

    return __rehash(table, table->nr_buckets / 2);
}
