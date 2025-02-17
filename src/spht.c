/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <rcn/spht.h>

#define __sz_bucket(table, nr_buckets) (nr_buckets * sizeof(*(table)->bucket))
#define __key_hash(table, entry) (table)->key_hash(table, entry)

struct spht_bucket {
    union {
        bool is_used;
        unsigned long __reserved;
    };
    void *entry;
};

static int __find_bucket(const struct spht *table, const void *ke,
                         struct spht_bucket **_bucket)
{
    size_t idx;
    struct spht_bucket *bucket;
    int err;

    idx = __key_hash(table, ke);
    if (idx >= table->nr_buckets)
        return -ERANGE;

    bucket = &table->bucket[idx];
    err = bucket->is_used ? -EEXIST : -ENOENT;

    *_bucket = bucket;

    return err;
}

void spht_initialize(struct spht *table, size_t nr_buckets,
                     spht_hash_t key_hash)
{
    struct spht_bucket *bucket;

    assert(nr_buckets >= SPHT_MIN_NR_BUCKETS);

    bucket = calloc(nr_buckets, __sz_bucket(table, nr_buckets));
    if (bucket == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for bucket\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    table->nr_buckets = nr_buckets;
    table->key_hash = key_hash;
    table->count = 0;
    table->bucket = bucket;
}

void spht_terminate(struct spht *table)
{
    if (table->count > 0) {
        fprintf(stderr, "%s():%u: Table is not empty\n", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

    free(table->bucket);
}

int spht_insert(struct spht *table, void *e)
{
    struct spht_bucket *bucket;
    int err;

    err = __find_bucket(table, e, &bucket);
    if (err != -ENOENT)
        return err;

    bucket->entry = e;
    bucket->is_used = true;

    table->count++;

    return 0;
}

int spht_remove(struct spht *table, const void *ke, void **_e)
{
    struct spht_bucket *bucket;
    int err;

    err = __find_bucket(table, ke, &bucket);
    if (err != -EEXIST)
        return err;

    if (_e != NULL)
        *_e = bucket->entry;

    bucket->is_used = false;

    table->count--;

    return 0;
}

int spht_search(const struct spht *table, const void *ke, void **_e)
{
    struct spht_bucket *bucket;
    int err;

    err = __find_bucket(table, ke, &bucket);
    if (err != -EEXIST)
        return err;

    if (_e != NULL)
        *_e = bucket->entry;

    return 0;
}

void spht_traverse(struct spht *table, do_each_t do_each, void *priv)
{
    size_t i;

    for (i = 0; i < table->nr_buckets; i++) {
        struct spht_bucket *bucket = &table->bucket[i];

        if (bucket->is_used)
            do_each(bucket->entry, priv);
    }
}
