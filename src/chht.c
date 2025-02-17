/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/chht.h>

#define __key_hash(table, x) (table)->key_hash(table, x)
#define __compar(table, key, in_table) (table)->compar(key, in_table)

static struct dbll_node *__get_head(struct chht *table,
                                    const struct dbll_node *kx)
{
    size_t hash;

    hash = __key_hash(table, kx);
    if (hash > table->nr_buckets)
        return NULL;

    return &table->bucket[hash];
}

void chht_initialize(struct chht *table, size_t nr_buckets,
                     chht_hash_t key_hash, chht_compar_t compar)
{
    struct dbll_node *bucket = table->bucket;
    size_t i;

    assert(nr_buckets >= CHHT_MIN_NR_BUCKETS);

    table->key_hash = key_hash;
    table->compar = compar;

    table->nr_buckets = nr_buckets;
    for (i = 0; i < nr_buckets; i++)
        dbll_initialize(&bucket[i]);
}

void chht_insert(struct chht *table, struct dbll_node *x)
{
    struct dbll_node *head;
    size_t addr;

    addr = __key_hash(table, x);
    head = &table->bucket[addr];

    dbll_insert_tail(head, x);
}

int chht_search(struct chht *table, const struct dbll_node *kx,
                struct dbll_node **_x)
{
    struct dbll_node *head;
    struct dbll_node *x;

    head = __get_head(table, kx);
    if (head == NULL)
        return -ERANGE;

    for (x = head->next; x != head; x = x->next) {
        if (__compar(table, kx, x) == 0) {
            if (_x != NULL)
                *_x = x;

            return 0;
        }
    }

    return -ENOENT;
}

void chht_traverse(struct chht *table, dbll_do_each_t do_each, void *priv)
{
    size_t i;

    for (i = 0; i < table->nr_buckets; i++)
        dbll_forward(&table->bucket[i], do_each, priv);
}
