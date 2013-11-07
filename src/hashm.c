#include "hashm.h"

typedef struct hash_map_bucket {
    void* key;
    void* value;
    unsigned int hash_key;
    char removed;
} bucket;

static bucket* hash_map_bucket_create(void* key, void* value,
                                  unsigned int hash_key) {
    bucket* new_bucket;
    new_bucket = malloc(sizeof(bucket));
    new_bucket->key = key;
    new_bucket->value = value;
    new_bucket->hash_key = hash_key;
    new_bucket->removed = 0;
    return new_bucket;
}

hash_map* hash_map_create(unsigned int capacity,
                          float load_factor_limit,
                          hash_func hash_alg,
                          generic_comp comp_key_func,
                          generic_comp comp_value_func,
                          generic_cpy copy_key_func,
                          generic_cpy copy_value_func,
                          generic_op free_key_func,
                          generic_op free_value_func) {
    int i;
    hash_map* new_hash;
    new_hash = malloc(sizeof(hash_map));
    new_hash->hash_alg = hash_alg;
    new_hash->comp_key_func = comp_key_func;
    new_hash->comp_value_func = comp_value_func;
    new_hash->copy_key_func = copy_key_func;
    new_hash->copy_value_func = copy_value_func;
    new_hash->free_key_func = free_key_func;
    new_hash->free_value_func = free_value_func;
    new_hash->load_factor_limit = (load_factor_limit > 0 &&
                                   load_factor_limit <= 1) ?
                                   load_factor_limit : DEFAULT_LDFACT;
    new_hash->capacity = (capacity > 0) ? capacity : DEFAULT_CAPACY;
    new_hash->size = 0;
    new_hash->cur_load_factor = 0;
    new_hash->bucket_list = malloc(new_hash->capacity * sizeof(bucket*));
    for(i = 0; i < new_hash->capacity; i++) {
        new_hash->bucket_list[i] = NULL;
    }
    return new_hash;
}

hash_map* hash_map_copy(hash_map* hash) {
    unsigned int i;
    bucket* cur_bucket;
    bucket* new_bucket;
    hash_map* new_hash;
    new_hash = NULL;
    if(hash) {
        if(!hash_map_is_empty(hash)) {
            new_hash = hash_map_create(hash->capacity,
                                       hash->load_factor_limit,
                                       hash->hash_alg,
                                       hash->comp_key_func,
                                       hash->comp_value_func,
                                       hash->copy_key_func,
                                       hash->copy_value_func,
                                       hash->free_key_func,
                                       hash->free_value_func);
            new_hash->size = hash->size;
            new_hash->cur_load_factor = hash->cur_load_factor;
            for(i = 0; i < hash->capacity; i++) {
                cur_bucket = hash->bucket_list[i];
                if(cur_bucket) {
                    if(!cur_bucket->removed) {
                        new_bucket = hash_map_bucket_create(hash->copy_key_func(cur_bucket->key),
                                                        hash->copy_value_func(cur_bucket->value),
                                                        cur_bucket->hash_key);
                        new_hash->bucket_list[i] = new_bucket;
                    }
                }
            }
        }
    }
    return new_hash;
}

int hash_map_insert(hash_map* hash, void* key, void* value, int soft_insert) {
    int inserted, i, j, new_Capacity, empty;
    unsigned int hash_key;
    bucket* new_bucket;
    bucket* cur_bucket;
    bucket* dest_bucket;
    hash_map* new_hash;
    inserted = empty = 0;
    if(hash) {
        inserted = 1;
        if(hash->cur_load_factor >= hash->load_factor_limit) {
            new_Capacity = (2 * hash->capacity) + 1;
            new_hash = hash_map_create(new_Capacity,
                                       hash->load_factor_limit,
                                       hash->hash_alg,
                                       hash->comp_key_func,
                                       hash->comp_value_func,
                                       hash->copy_key_func,
                                       hash->copy_value_func,
                                       hash->free_key_func,
                                       hash->free_value_func);
            new_hash->size = hash->size;
            for(i = 0; i < hash->capacity; i++) {
                cur_bucket = hash->bucket_list[i];
                if(cur_bucket) {
                    if(!cur_bucket->removed) {
                        j = 0;
                        cur_bucket->hash_key = new_hash->hash_alg(cur_bucket->key) % new_Capacity;
                        dest_bucket = new_hash->bucket_list[cur_bucket->hash_key];
                        while(dest_bucket) {
                            j++;
                            if(j + cur_bucket->hash_key == new_Capacity) {
                                j = -cur_bucket->hash_key;
                            }
                            dest_bucket = new_hash->bucket_list[cur_bucket->hash_key + j];
                        }
                        new_hash->bucket_list[cur_bucket->hash_key + j] = cur_bucket;
                    } else {
                        if(!soft_insert) {
                            hash->free_key_func(cur_bucket->key);
                            hash->free_value_func(cur_bucket->value);
                        }
                        free(cur_bucket);
                    }
                }
            }
            free(hash->bucket_list);
            *hash = *new_hash;
            free(new_hash);
        }
        hash_key = hash->hash_alg(key) % hash->capacity;
        new_bucket = hash_map_bucket_create(key, value, hash_key);
        dest_bucket = hash->bucket_list[new_bucket->hash_key];
        j = 0;
        while(dest_bucket && !empty) {
            if(dest_bucket->removed) {
                empty = 1;
                if(!soft_insert) {
                    hash->free_key_func(dest_bucket->key);
                    hash->free_value_func(dest_bucket->value);
                }
                free(dest_bucket);
            } else if(!hash->comp_key_func(dest_bucket->key, key)) {
                empty = 1;
                inserted = 0;
                free(new_bucket);
            } else {
                j++;
                if(j + new_bucket->hash_key == hash->capacity) {
                    j = -new_bucket->hash_key;
                }
                dest_bucket = hash->bucket_list[new_bucket->hash_key + j];
            }
        }
        if(inserted) {
            hash->bucket_list[new_bucket->hash_key + j] = new_bucket;
            hash->size++;
            hash->cur_load_factor = (float)hash->size / hash->capacity;
        }
    }
    return inserted;
}

int hash_map_remove(hash_map* hash, void* key) {
    int removed, i, j;
    unsigned int hash_key;
    bucket* cur_bucket;
    removed = 0;
    if(hash) {
        if(!hash_map_is_empty(hash)) {
            i = 0;
            j = 0;
            hash_key = hash->hash_alg(key) % hash->capacity;
            cur_bucket = hash->bucket_list[hash_key];
            while(cur_bucket && j < hash->capacity && !removed) {
                i++;
                j++;
                if(!hash->comp_key_func(cur_bucket->key, key)) {
                    if(!cur_bucket->removed) {
                        --hash->size;
                        removed = 1;
                        cur_bucket->removed = 1;
                    } else {
                        j = hash->capacity;
                    }
                }
                if(i + hash_key == hash->capacity) {
                    i = -hash_key;
                }
                cur_bucket = hash->bucket_list[hash_key + i];
            }
        }
    }
    return removed;
}

int hash_map_set(hash_map* hash, const void* key, void* value) {
    int i, j, set;
    unsigned int hash_key;
    bucket* cur_bucket;
    set = 0;
    if(hash) {
        if(!hash_map_is_empty(hash)) {
            i = 0;
            j = 0;
            hash_key = hash->hash_alg(key) % hash->capacity;
            cur_bucket = hash->bucket_list[hash_key];
            while(cur_bucket && !set && j < hash->capacity) {
                i++;
                j++;
                if(!hash->comp_key_func(cur_bucket->key, key)) {
                    if(!cur_bucket->removed) {
                        set = 1;
                        hash->free_value_func(cur_bucket->value);
                        cur_bucket->value = value;
                    } else {
                        j = hash->capacity;
                    }
                }
                if(i + hash_key == hash->capacity) {
                    i = -hash_key;
                }
                cur_bucket = hash->bucket_list[hash_key + i];
            }
        }
    }
    return set;
}

int hash_map_contains_key(hash_map* hash, const void* key) {
    int i, j, contains_key;
    unsigned int hash_key;
    bucket* cur_bucket;
    contains_key = 0;
    if(hash) {
        if(!hash_map_is_empty(hash)) {
            i = 0;
            j = 0;
            hash_key = hash->hash_alg(key) % hash->capacity;
            cur_bucket = hash->bucket_list[hash_key];
            while(cur_bucket && !contains_key && j < hash->capacity) {
                i++;
                j++;
                if(!hash->comp_key_func(cur_bucket->key, key)) {
                    if(!cur_bucket->removed) {
                        contains_key = 1;
                    } else {
                        j = hash->capacity;
                    }
                }
                if(i + hash_key == hash->capacity) {
                    i = -hash_key;
                }
                cur_bucket = hash->bucket_list[hash_key + i];
            }
        }
    }
    return contains_key;
}

int hash_map_contains_value(hash_map* hash, const void* value) {
    int i, contains_value;
    bucket* cur_bucket;
    contains_value = 0;
    if(hash) {
        if(!hash_map_is_empty(hash)) {
            i = 0;
            cur_bucket = hash->bucket_list[0];
            while(!contains_value && i < hash->capacity) {
                i++;
                if(cur_bucket) {
                    if(!hash->comp_value_func(cur_bucket->value, value)) {
                        if(!cur_bucket->removed) {
                            contains_value = 1;
                        }
                    }
                }
                cur_bucket = hash->bucket_list[i];
            }
        }
    }
    return contains_value;
}

int hash_map_is_empty(hash_map* hash) {
    int empty;
    empty = 1;
    if(hash) {
        if(hash->size) {
            empty = 0;
        }
    }
    return empty;
}

size_t hash_map_size(hash_map* hash) {
    size_t size;
    size = 0;
    if(hash) {
        size = hash->size;
    }
    return size;
}

void* hash_map_get(hash_map* hash, const void* key) {
    int i, j, found;
    unsigned int  hash_key;
    bucket* cur_bucket;
    void* value;
    value = NULL;
    if(hash) {
        if(!hash_map_is_empty(hash)) {
            i = 0;
            j = 0;
            found = 0;
            hash_key = hash->hash_alg(key) % hash->capacity;
            cur_bucket = hash->bucket_list[hash_key];
            while(cur_bucket && !found && j < hash->capacity) {
                i++;
                j++;
                if(!hash->comp_key_func(cur_bucket->key, key)) {
                    if(!cur_bucket->removed) {
                        found = 1;
                        value = cur_bucket->value;
                    } else {
                        j = hash->capacity;
                    }
                }
                if(i + hash_key == hash->capacity) {
                    i = -hash_key;
                }
                cur_bucket = hash->bucket_list[hash_key + i];
            }
        }
    }
    return value;
}

void hash_map_empty(hash_map* hash) {
    int i;
    bucket* cur_bucket;
    if(hash) {
        hash->size = 0;
        for(i = 0; i < hash->capacity; i++) {
            cur_bucket = hash->bucket_list[i];
            if(cur_bucket) {
                hash->bucket_list[i] = NULL;
                hash->free_key_func(cur_bucket->key);
                hash->free_value_func(cur_bucket->value);
                free(cur_bucket);
            }
        }
    }
    return;
}

void hash_map_kill(hash_map* hash) {
    if(hash) {
        hash_map_empty(hash);
        free(hash->bucket_list);
        free(hash);
    }
    return;
}

void hash_map_dissolve(hash_map* hash) {
    int i;
    bucket* cur_bucket;
    if(hash) {
        for(i = 0; i < hash->capacity; i++) {
            cur_bucket = hash->bucket_list[i];
            if(cur_bucket) {
                hash->bucket_list[i] = NULL;
                free(cur_bucket);
            }
        }
        free(hash->bucket_list);
        free(hash);
    }
    return;
}

void hash_map_dissolve_keys(hash_map* hash) {
    int i;
    bucket* cur_bucket;
    if(hash) {
        for(i = 0; i < hash->capacity; i++) {
            cur_bucket = hash->bucket_list[i];
            if(cur_bucket) {
                hash->bucket_list[i] = NULL;
                hash->free_key_func(cur_bucket->key);
                free(cur_bucket);
            }
        }
        free(hash->bucket_list);
        free(hash);
    }
    return;
}

void hash_map_dissolve_values(hash_map* hash) {
    int i;
    bucket* cur_bucket;
    if(hash) {
        for(i = 0; i < hash->capacity; i++) {
            cur_bucket = hash->bucket_list[i];
            if(cur_bucket) {
                hash->bucket_list[i] = NULL;
                hash->free_value_func(cur_bucket->value);
                free(cur_bucket);
            }
        }
        free(hash->bucket_list);
        free(hash);
    }
    return;
}

void hash_map_traverse(hash_map* hash, generic_op do_func) {
    int i;
    bucket* cur_bucket;
    if(hash) {
        if(!hash_map_is_empty(hash)) {
            for(i = 0; i < hash->capacity; i++) {
                cur_bucket = hash->bucket_list[i];
                if(cur_bucket) {
                    if(!cur_bucket->removed) {
                        do_func(cur_bucket->value);
                    }
                }
            }
        }
    }
    return;
}

/*!
  \file hashm.c
  \brief Hash Map source file.
  \details Source file for hash map, contains all functions and declarations
  a user may call.
  \warning It is assumed that all keys and values inserted into the hash map are heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
  \var typedef struct hash_map_bucket bucket
  \brief Typedef of struct hash_map_bucket to 'bucket'.
*/

/*!
  \struct hash_map_bucket
  \brief Fundamental hash map bucket structure.
  \var hash_map_bucket::key
  Member 'key' represents the allocated key in the bucket.
  \var hash_map_bucket::value
  Member 'value' represents the allocated value to be paired with the bucket's key.
  \var hash_map_bucket::hash_key
  Member 'hash_key' represents the numberical hash value of the 'key' member.
  \var hash_map_bucket::removed
  Member 'removed' represents the flag denoting if the bucket has been removed, and is therefore valid.
*/

/*!
  \static
  \fn bucket* hash_map_bucket_create(void* key, void* value, unsigned int hash_key)
  \brief Creates and returns a newly allocated bucket.
  \param key Bucket's key.
  \param value Bucket's value.
  \param hash_key Hashed form of bucket's key.
  \return Newly allocated bucket.
*/

