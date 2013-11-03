#include "hasht.h"

typedef struct hash_table_bucket {
    void* data;
    unsigned int hash_key;
    char removed;
} bucket;

static bucket* hash_table_bucket_create(void* data, unsigned int hash_key) {
    bucket* buck;
    buck = malloc(sizeof(bucket));
    buck->data = data;
    buck->hash_key = hash_key;
    buck->removed = 0;
    return buck;
}

hash_table* hash_table_create(unsigned int capacity,
                              float load_factor_limit,
                              hash_func hash_alg,
                              generic_comp comp_func,
                              generic_cpy copy_func,
                              generic_op free_func) {
    int i;
    hash_table* hash;
    hash = malloc(sizeof(hash_table));
    hash->hash_alg = hash_alg;
    hash->comp_func = comp_func;
    hash->copy_func = copy_func;
    hash->free_func = free_func;
    hash->load_factor_limit = (load_factor_limit > 0 &&
                               load_factor_limit <= 1) ?
                               load_factor_limit : DEFAULT_LDFACT;
    hash->capacity = (capacity > 0) ? capacity : DEFAULT_CAPACY;
    hash->size = 0;
    hash->cur_load_factor = 0;
    hash->bucket_list = malloc(hash->capacity * sizeof(bucket*));
    for(i = 0; i < hash->capacity; i++) {
        hash->bucket_list[i] = NULL;
    }
    return hash;
}

hash_table* copy_hash_table(hash_table* hash) {
    unsigned int i;
    bucket* cur_Bucket;
    bucket* new_Bucket;
    hash_table* new_Hash;
    new_Hash = NULL;
    if(hash) {
        if(!hash_table_is_empty(hash)) {
            new_Hash = hash_table_create(hash->capacity,
                                         hash->load_factor_limit,
                                         hash->hash_alg,
                                         hash->comp_func,
                                         hash->copy_func,
                                         hash->free_func);
            new_Hash->size = hash->size;
            new_Hash->cur_load_factor = hash->cur_load_factor;
            for(i = 0; i < hash->capacity; i++) {
                cur_Bucket = hash->bucket_list[i];
                if(cur_Bucket) {
                    if(!cur_Bucket->removed) {
                        new_Bucket = hash_table_bucket_create(hash->copy_func(cur_Bucket->data),
                                                              cur_Bucket->hash_key);
                        new_Hash->bucket_list[i] = new_Bucket;
                    }
                }
            }
        }
    }
    return new_Hash;
}

int hash_table_insert(hash_table* hash, void* data, int soft_insert) {
    int inserted, i, j, new_Capacity, empty;
    unsigned int hash_key;
    bucket* new_Bucket;
    bucket* cur_Bucket;
    bucket* dest_Bucket;
    hash_table* new_Hash;
    inserted = empty = 0;
    if(hash) {
        inserted = 1;
        if(hash->cur_load_factor >= hash->load_factor_limit) {
            new_Capacity = (2 * hash->capacity) + 1;
            new_Hash = hash_table_create(new_Capacity,
                                         hash->load_factor_limit,
                                         hash->hash_alg,
                                         hash->comp_func,
                                         hash->copy_func,
                                         hash->free_func);
            new_Hash->size = hash->size;
            for(i = 0; i < hash->capacity; i++) {
                cur_Bucket = hash->bucket_list[i];
                if(cur_Bucket) {
                    if(!cur_Bucket->removed) {
                        j = 0;
                        cur_Bucket->hash_key = new_Hash->hash_alg(cur_Bucket->data) % new_Capacity;
                        dest_Bucket = new_Hash->bucket_list[cur_Bucket->hash_key];
                        while(dest_Bucket) {
                            j++;
                            if(j + cur_Bucket->hash_key == new_Capacity) {
                                j = -cur_Bucket->hash_key;
                            }
                            dest_Bucket = new_Hash->bucket_list[cur_Bucket->hash_key + j];
                        }
                        new_Hash->bucket_list[cur_Bucket->hash_key + j] = cur_Bucket;
                    } else {
                        if(!soft_insert) {
                            hash->free_func(cur_Bucket->data);
                        }
                        free(cur_Bucket);
                    }
                }
            }
            free(hash->bucket_list);
            *hash = *new_Hash;
            free(new_Hash);
        }
        hash_key = hash->hash_alg(data) % hash->capacity;
        new_Bucket = hash_table_bucket_create(data, hash_key);
        dest_Bucket = hash->bucket_list[new_Bucket->hash_key];
        j = 0;
        while(dest_Bucket && !empty) {
            if(dest_Bucket->removed) {
                empty = 1;
                if(!soft_insert) {
                    hash->free_func(dest_Bucket->data);
                }
                free(dest_Bucket);
            } else if(!hash->comp_func(dest_Bucket->data, data)) {
                empty = 1;
                inserted = 0;
                free(new_Bucket);
            } else {
                j++;
                if(j + new_Bucket->hash_key == hash->capacity) {
                    j = -new_Bucket->hash_key;
                }
                dest_Bucket = hash->bucket_list[new_Bucket->hash_key + j];
            }
        }
        if(inserted) {
            hash->bucket_list[new_Bucket->hash_key + j] = new_Bucket;
            hash->size++;
            hash->cur_load_factor = (float)hash->size / hash->capacity;
        }
    }
    return inserted;
}

int hash_table_remove(hash_table* hash, void* data) {
    int removed, i, j;
    unsigned int hash_key;
    bucket* cur_Bucket;
    removed = 0;
    if(hash) {
        if(!hash_table_is_empty(hash)) {
            i = 0;
            j = 0;
            hash_key = hash->hash_alg(data) % hash->capacity;
            cur_Bucket = hash->bucket_list[hash_key];
            while(cur_Bucket && j < hash->capacity && !removed) {
                i++;
                j++;
                if(!hash->comp_func(cur_Bucket->data, data)) {
                    if(!cur_Bucket->removed) {
                        --hash->size;
                        removed = 1;
                        cur_Bucket->removed = 1;
                    } else {
                        j = hash->capacity;
                    }
                }
                if(i + hash_key == hash->capacity) {
                    i = -hash_key;
                }
                cur_Bucket = hash->bucket_list[hash_key + i];
            }
        }
    }
    return removed;
}

int hash_table_contains(hash_table* hash, const void* data) {
    int i, j, contains;
    unsigned int hash_key;
    bucket* cur_Bucket;
    contains = 0;
    if(hash) {
        if(!hash_table_is_empty(hash)) {
            i = 0;
            j = 0;
            hash_key = hash->hash_alg(data) % hash->capacity;
            cur_Bucket = hash->bucket_list[hash_key];
            while(cur_Bucket && !contains && j < hash->capacity) {
                i++;
                j++;
                if(!hash->comp_func(cur_Bucket->data, data)) {
                    if(!cur_Bucket->removed) {
                        contains = 1;
                    } else {
                        j = hash->capacity;
                    }
                }
                if(i + hash_key == hash->capacity) {
                    i = -hash_key;
                }
                cur_Bucket = hash->bucket_list[hash_key + i];
            }
        }
    }
    return contains;
}

int hash_table_is_empty(hash_table* hash) {
    int empty;
    empty = 1;
    if(hash) {
        if(hash->size) {
            empty = 0;
        }
    }
    return empty;
}

size_t hash_table_size(hash_table* hash) {
    size_t size;
    size = 0;
    if(hash) {
        size = hash->size;
    }
    return size;
}

void hash_table_empty(hash_table* hash) {
    int i;
    bucket* cur_Bucket;
    if(hash) {
        hash->size = 0;
        for(i = 0; i < hash->capacity; i++) {
            cur_Bucket = hash->bucket_list[i];
            if(cur_Bucket) {
                hash->bucket_list[i] = NULL;
                hash->free_func(cur_Bucket->data);
                free(cur_Bucket);
            }
        }
    }
    return;
}

void hash_table_kill(hash_table* hash) {
    if(hash) {
        hash_table_empty(hash);
        free(hash->bucket_list);
        free(hash);
    }
    return;
}

void hash_table_dissolve(hash_table* hash) {
    int i;
    bucket* cur_Bucket;
    if(hash) {
        for(i = 0; i < hash->capacity; i++) {
            cur_Bucket = hash->bucket_list[i];
            if(cur_Bucket) {
                hash->bucket_list[i] = NULL;
                free(cur_Bucket);
            }
        }
        free(hash->bucket_list);
        free(hash);
    }
    return;
}

void hash_table_traverse(hash_table* hash, generic_op do_func) {
    int i;
    bucket* cur_Bucket;
    if(hash) {
        if(!hash_table_is_empty(hash)) {
            for(i = 0; i < hash->capacity; i++) {
                cur_Bucket = hash->bucket_list[i];
                if(cur_Bucket) {
                    if(!cur_Bucket->removed) {
                        do_func(cur_Bucket->data);
                    }
                }
            }
        }
    }
    return;
}
