#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hasht.h"

struct hash_map_bucket;

typedef struct linear_hash_map {
    struct hash_map_bucket** bucket_list;
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

int hash_map_insert(hash_map* hash, void* key, void* value, int soft_insert);
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

/*!
  \file hashm.h
  \brief Hash Map header file.
  \details Header file for hash map, contains all functions and declarations
  a user may call.
  \warning It is assumed that all keys and values inserted into the hash map are heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
  \var typedef struct linear_hash_map hash_map
  \brief Typedef of struct linear_hash_map to 'hash_map'.
*/

/*!
  \struct linear_hash_map hashm.h "include/hashm.h"
  \brief Fundamental hash map structure.
  \var linear_hash_map::bucket_list
  Member 'bucket_list' represents a pointer to a series of bucket pointers. This is the basis of the hash map's
  storage.
  \var linear_hash_map::size
  Member 'size' represents how many of the buckets in the hash map are in use, and not marked as removed.
  \var linear_hash_map::capacity
  Member 'capacity' represents how many total buckets there are in the hash map.
  \var linear_hash_map:: cur_load_factor
  Member 'cur_load_factor' represents a ratio which in turn represents number of occupied buckets in respects to total capacity.
  \var linear_hash_map::load_factor_limit
  Member 'load_factor_limit' represents a fixed ratio regarding the allotted limit of occupied buckets to total capacity.
  \var linear_hash_map::hash_alg
  Member 'hash_alg' represents the hash_func to be used by the hash map for indexing operations.
  \var linear_hash_map::comp_key_func
  Member 'comp_key_func' represents a generic_comp function pointer to be used by the hash map's comparision based functions regarding keys.
  \var linear_hash_map::copy_key_func
  Member 'copy_key_func' represents a generic_cpy function pointer to be used by the hash map's copy based functions regarding keys.
  \var linear_hash_map::free_key_func
  Member 'free_key_func' represents a generic_op function pointer to be used by the hash map's deconstruction based functions regarding keys.
  \var linear_hash_map::comp_value_func
  Member 'comp_value_func' represents a generic_comp function pointer to be used by the hash map's comparision based functions regarding values.
  \var linear_hash_map::copy_value_func
  Member 'copy_value_func' represents a generic_cpy function pointer to be used by the hash map's copy based functions regarding values.
  \var linear_hash_map::free_value_func
  Member 'free_value_func' represents a generic_op function pointer to be used by the hash map's deconstruction based functions regarding values.
*/

/*!
  \fn hash_map* hash_map_create(unsigned int capacity,
                                float load_factor_limit,
                                hash_func hash_alg,
                                generic_comp comp_key_func,
                                generic_comp comp_value_func,
                                generic_cpy copy_key_func,
                                generic_cpy copy_value_func,
                                generic_op free_key_func,
                                generic_op free_value_func)
  \brief Creates and returns an allocated hash map.
  \param capacity Total number of buckets hash map could be initiated to contain.
  \param load_factor_limit Desired maximum ratio between occupied buckets and total buckets before regrowing is triggered.
  \param hash_alg Hashing algorithm function pointer for keys.
  \param comp_key_func A generic_comp style function pointer, used to compare keys held within hash map.
  \param copy_key_func A generic_cpy style function pointer, used to copy keys held within hash map.
  \param free_key_func A generic_op style function pointer, used to deallocate keys held within hash map.
  \param comp_value_func A generic_comp style function pointer, used to compare values held within hash map.
  \param copy_value_func A generic_cpy style function pointer, used to copy data values within hash map.
  \param free_value_func A generic_op style function pointer, used to deallocate values held within hash map.
  \return Newly allocated hash map with given function pointer, capacity, and load factor limit properties.
*/

/*!
  \fn hash_map* hash_map_copy(hash_map* hash)
  \brief Creates and returns an allocated copy of the inputted hash map.
  \param hash Hash map to be copied.
  \return Newly allocated hash map that is a copy of the inputted hash map.
*/

/*!
  \fn int hash_map_insert(hash_map* hash, void* key, void* value, int soft_insert)
  \brief Inserts a new key-value pair into given hash map.
  \param hash Hash map to have key and value inserted into.
  \param key Key to be used to look up value.
  \param value Value to be stored.
  \param soft_insert If 0, then data that has been marked for deletion will be freed upon regowth/collision,
  if 0 then the key-value pairs not be deallocated.
  \return 1 if insertion was successful, 0 if not.
  \note If the key is a duplicate of an already existing key in a valid bucket, insertion will fail.
  \note If the bucket is marked as removed, then the key-value pairing passed into the function overrides the previous value,
  and the old bucket is deallocated.
  \note Neither the value nor key passed into the function will be deallocated a failure is detected.
*/

/*!
  \fn int hash_map_remove(hash_map* hash, void* key)
  \brief Removes a key-value pair from the hash map, given the key.
  \param hash Hash map to have key-value pairing removed.
  \param key Key with which to index and search the hash map.
  \return 1 if removal was sucessful, 0 if not.
  \note Genuine removal in the form of deallocation will not occur within this fucntion call.
  This merely marks a bucket as not valid.
  \note Selective deallocation is reserved for the regrowing phase, which is handled in the insertion function,
  and therein deallocates invalid buckets and key-value pairs.
*/

/*!
  \fn int hash_map_set(hash_map* hash, const void* key, void* value)
  \brief Overrides an existing key-value pair with a new value.
  \param hash Hash map to have key-value pairing altered.
  \param key Key with which to index and search the hash map.
  \param value Value that will replace old value of key-value pair.
  \return 1 if removal was sucessful, 0 if not.
  \note If the key is not found as an existing key in the hash map, setting will fail.
  \note Neither the value nor the key passed into the function will be deallocated a failure is detected.
*/

/*!
  \fn int hash_map_contains_key(hash_map* hash, const void* key)
  \brief Checks hash map for perticular key.
  \param hash Hash map to be cheked for key.
  \param key Key to search for.
  \return 1 if key is in hash map, 0 if not.
*/

/*!
  \fn int hash_map_contains_value(hash_map* hash, const void* value)
  \brief Check hash map for perticular value.
  \param hash Hash map to be checked for value.
  \param value Value to search for.
  \return 1 if value is in hash map, 0 if not.
*/

/*!
  \fn int hash_map_is_empty(hash_map* hash)
  \brief Checks if hash map is empty.
  \param hash Hash map to be cheked for content.
  \return 1 if hash map contains no buckets in unmarked-for-removal use, 0 if not.
*/

/*!
  \fn size_t hash_map_size(hash_map* hash)
  \brief Check hash map size.
  \param hash Hash map to have size checked.
  \return Number representing how many buckets, unmarked for removal, are in hash map.
*/

/*!
  \fn void* hash_map_get(hash_map* hash, const void* key)
  \brief Given key, hash map is searched for corresponding value. That value is then returned.
  \param hash Hash map to be searhed.
  \param key Key to key-value pair to be searched for.
  \return Pointer to value. NULL if no corresponding value was located.
*/

/*!
  \fn void hash_map_empty(hash_map* hash)
  \brief Deallocates all buckets in hash map.
  \param hash Hash map to have buckets deallocated.
  \note The hash map itself is still valid and re-usable after this call.
  \note It \b has \b not been deallocated.
*/

/*!
  \fn void hash_map_kill(hash_map* hash)
  \brief Completely deallocates the hash map.
  \param hash Hash map to be deallocated.
  \note The hash map itself is \b not valid and \b not re-usable after this call.
  \note It \b has been deallocated.
*/

/*!
  \fn void hash_map_dissolve(hash_map* hash)
  \brief Partial deallocation the hash map, key-value pairs remain allocated.
  \param hash Hash map to be dissolved.
  \note This method works like hash_map_kill, except that the key-value pairs are not deallocated.
  \note In other words, the struct is freed, but any inserted data is left alone.
  \note This is useful if the data has a planned lifespan longer than the struct,
  or if the data is shared between multiple structures.
  \note Also much like hash_map_kill, the hash map is \b not valid after this call.
*/

/*!
  \fn void hash_map_dissolve_keys(hash_map* hash)
  \brief Partial deallocation the hash map, values remain allocated.
  \param hash Hash map to be dissolved.
  \note This method works like hash_map_kill, except that the value data is not deallocated.
  \note In other words, the struct is freed, but any inserted value data is left alone.
  \note This is useful if the values have a planned lifespan longer than the struct,
  or if the values are shared between multiple structures.
  \note Also much like hash_map_kill, the hash map is \b not valid after this call.
*/

/*!
  \fn void hash_map_dissolve_values(hash_map* hash)
  \brief Partial deallocation the hash map, keys remain allocated.
  \param hash Hash map to be dissolved.
  \note This method works like hash_map_kill, except that the key data is not deallocated.
  \note In other words, the struct is freed, but any inserted key data is left alone.
  \note This is useful if the keys have a planned lifespan longer than the struct,
  or if the keys are shared between multiple structures.
  \note Also much like hash_map_kill, the hash map is \b not valid after this call.
*/

/*!
  \fn void hash_map_traverse(hash_map* hash, generic_op do_func)
  \brief Traverses all values in the hash map, applies do_func to each value.
  \param hash Hash map to be traversed.
  \param do_func Operation function pointer.
*/
