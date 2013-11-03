#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hasht.h"

struct hash_bucket;

typedef struct linear_hash_map {
    struct hash_bucket** bucket_list;
    size_t size;
    size_t capacity;
    float cur_load_factor;
    float load_factor_limit;
    hash_func hash_alg;
    generic_comp comp_key_func;
    generic_comp comp_value_func;
    generic_cpy copy_key_func;
    generic_cpy copy_value_func;
    generic_op free_key_func;
    generic_op free_value_func;
} hash_map;

hash_map* hash_map_create(unsigned int capacity,
                          float load_factor_limit,
                          hash_func hash_alg,
                          generic_comp comp_key_func,
                          generic_comp comp_value_func,
                          generic_cpy copy_key_func,
                          generic_cpy copy_value_func,
                          generic_op free_key_func,
                          generic_op free_value_func);

hash_map* hash_map_copy(hash_map* hash);

int hash_map_insert(hash_map* hash, void* key, void* value, int soft);
int hash_map_remove(hash_map* hash, void* key);
int hash_map_set(hash_map* hash, const void* key, void* value);
int hash_map_contains_key(hash_map* hash, const void* key);
int hash_map_contains_value(hash_map* hash, const void* value);
int hash_map_is_empty(hash_map* hash);

size_t hash_map_size(hash_map* hash);

void* hash_map_get(hash_map* hash, const void* key);

void hash_map_empty(hash_map* hash);
void hash_map_kill(hash_map* hash);
void hash_map_dissolve(hash_map* hash);
void hash_map_dissolve_keys(hash_map* hash);
void hash_map_dissolve_values(hash_map* hash);
void hash_map_traverse(hash_map* hash, generic_op do_func);

#endif
