#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "ugf.h"

#define DEFAULT_LDFACT 0.75
#define DEFAULT_CAPACY 89

struct hash_table_bucket;

typedef unsigned int (*hash_func)(const void*);

typedef struct linear_hash_table {
    struct hash_table_bucket** bucket_list;
    size_t size;
    size_t capacity;
    float cur_load_factor;
    float load_factor_limit;
    hash_func hash_alg;
    generic_comp comp_func;
    generic_cpy copy_func;
    generic_op free_func;
} hash_table;

hash_table* hash_table_create(unsigned int capacity,
                              float load_factor_limit,
                              hash_func hash_alg,
                              generic_comp comp_func,
                              generic_cpy copy_func,
                              generic_op free_func);

hash_table* hash_table_copy(hash_table* hash);

int hash_table_insert(hash_table* hash, void* data, int soft_insert);
int hash_table_remove(hash_table* hash, void* data);
int hash_table_contains(hash_table* hash, const void* data);
int hash_table_is_empty(hash_table* hash);

size_t hash_table_size(hash_table* hash);

void hash_table_empty(hash_table* hash);
void hash_table_kill(hash_table* hash);
void hash_table_dissolve(hash_table* hash);
void hash_table_traverse(hash_table* hash, generic_op do_func);

#endif
