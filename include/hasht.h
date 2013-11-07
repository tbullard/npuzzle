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

/*!
  \file hasht.h
  \brief Hash Table header file.
  \details Header file for hash table, contains all functions and declarations
  a user may call.
  \warning It is assumed that all keys and values inserted into the hash map are heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
   \def DEFAULT_LDFACT
   \brief Default load factor, used in hash_table/map_create fucntion if invalid load factor is given.
*/

/*!
   \def DEFAULT_CAPACY
   \brief Default capacity, used in hash_table/map_create fucntion if invalid capacity is given.
*/

/*!
  \var typedef struct linear_hash_table hash_table
  \brief Typedef of struct linear_hash_table to 'hash_table'.
*/

/*!
  \var typedef unsigned int (*hash_func)(const void*)
  \brief Typedef form of hash function.
  \param Pointer to data needing to be hashed.
  \return Unsigned integer representing data's hashed value.
*/

/*!
  \struct linear_hash_table hasht.h "include/hasht.h"
  \brief Fundamental hash table structure.
  \var linear_hash_table::bucket_list
  Member 'bucket_list' represents a pointer to a series of bucket pointers. This is the basis of the hash table's
  storage.
  \var linear_hash_table::size
  Member 'size' represents how many of the buckets in the hash table are in use, and not marked as removed.
  \var linear_hash_table::capacity
  Member 'capacity' represents how many total buckets there are in the hash table.
  \var linear_hash_table:: cur_load_factor
  Member 'cur_load_factor' represents a ratio which in turn represents number of occupied buckets in respects to total capacity.
  \var linear_hash_table::load_factor_limit
  Member 'load_factor_limit' represents a fixed ratio regarding the allotted limit of occupied buckets to total capacity.
  \var linear_hash_table::hash_alg
  Member 'hash_alg' represents the hash_func to be used by the hash table for indexing operations.
  \var linear_hash_table::comp_func
  Member 'comp_func' represents a generic_comp function pointer to be used by the hash table's comparision based functions.
  \var linear_hash_table::copy_func
  Member 'copy_func' represents a generic_cpy function pointer to be used by the hash table's copy based functions.
  \var linear_hash_table::free_func
  Member 'free_func' represents a generic_op function pointer to be used by the hash table's deconstruction based functions.
*/

/*!
  \fn hash_table* hash_table_create(unsigned int capacity,
                                    float load_factor_limit,
                                    hash_func hash_alg,
                                    generic_comp comp_func,
                                    generic_cpy copy_func,
                                    generic_op free_func)
  \brief Creates and returns an allocated hash table.
  \param capacity Total number of buckets hash table could be initiated to contain.
  \param load_factor_limit Desired maximum ratio between occupied buckets and total buckets before regrowing is triggered.
  \param hash_alg Hashing algorithm function pointer for data.
  \param comp_func A generic_comp style function pointer, used to compare data held within hash table.
  \param copy_func A generic_cpy style function pointer, used to copy data held within hash table.
  \param free_func A generic_op style function pointer, used to deallocate data held within hash table.
  \return Newly allocated hash table with given function pointer, capacity, and load factor limit properties.
*/

/*!
  \fn hash_table* hash_table_copy(hash_table* hash)
  \brief Creates and returns an allocated copy of the inputted hash table.
  \param hash Hash table to be copied.
  \return Newly allocated hash table that is a copy of the inputted hash table.
*/

/*!
  \fn int hash_table_insert(hash_table* hash, void* data, int soft_insert)
  \brief Inserts new data into given hash table.
  \param hash Hash table to have key and value inserted into.
  \param data Data to be stored.
  \param soft_insert If 0, then data that has been marked for deletion will be freed upon regowth/collision,
  if 0 then the data not be deallocated.
  \return 1 if insertion was successful, 0 if not.
  \note If the data is a duplicate of an already existing key in a valid bucket, insertion will fail.
  \note If the bucket is marked as removed, then the data passed into the function overrides the previous value,
  and the old bucket is deallocated.
  \note The data passed into the function will not be deallocated if a failure is detected.
*/

/*!
  \fn int hash_table_remove(hash_table* hash, void* data)
  \brief Removes a data-value pair from the hash table, given the data.
  \param hash Hash table to have data removed.
  \param data Data with will be removed from hash table.
  \return 1 if removal was sucessful, 0 if not.
  \note Genuine removal in the form of deallocation will not occur within this fucntion call.
  This merely marks a bucket as not valid.
  \note Selective deallocation is reserved for the regrowing phase, which is handled in the insertion function,
  and therein deallocates invalid buckets and data.
*/

/*!
  \fn int hash_table_contains(hash_table* hash, const void* data)
  \brief Checks hash table for perticular data.
  \param hash Hash table to be cheked for data.
  \param data Data to search for.
  \return 1 if data is in hash map, 0 if not.
*/

/*!
  \fn int hash_table_is_empty(hash_table* hash)
  \brief Checks if hash table is empty.
  \param hash Hash table to be cheked for content.
  \return 1 if hash table contains no buckets in unmarked-for-removal use, 0 if not.
*/

/*!
  \fn size_t hash_table_size(hash_table* hash)
  \brief Check hash table size.
  \param hash Hash table to have size checked.
  \return Number representing how many buckets, unmarked for removal, are in hash table.
*/

/*!
  \fn void hash_table_empty(hash_table* hash)
  \brief Deallocates all buckets in hash table.
  \param hash Hash table to have buckets deallocated.
  \note The hash table itself is still valid and re-usable after this call.
  \note It \b has \b not been deallocated.
*/

/*!
  \fn void hash_table_kill(hash_table* hash)
  \brief Completely deallocates the hash table.
  \param hash Hash table to be deallocated.
  \note The hash table itself is \b not valid and \b not re-usable after this call.
  \note It \b has been deallocated.
*/

/*!
  \fn void hash_table_dissolve(hash_table* hash)
  \brief Partial deallocation the hash table, data remains valid.
  \param hash Hash table to be dissolved.
  \note This method works like hash_table_kill, except that the data is not deallocated.
  \note In other words, the struct is freed, but any inserted data is left alone.
  \note This is useful if the data has a planned lifespan longer than the struct,
  or if the data is shared between multiple structures.
  \note Also much like hash_table_kill, the hash table is \b not valid after this call.
*/

/*!
  \fn void hash_table_traverse(hash_table* hash, generic_op do_func)
  \brief Traverses all values in the hash table, applies do_func to each datum.
  \param hash Hash table to be traversed.
  \param do_func Operation function pointer.
*/
