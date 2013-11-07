#ifndef ARR_LIST_H
#define ARR_LIST_H

#include "ugf.h"

typedef struct dynamic_array {
    void** memory;
    size_t allocated;
    size_t used;
    int index;
    generic_comp comp_func;
    generic_cpy copy_func;
    generic_op free_func;
} array_list;

array_list* array_list_create(generic_comp comp_func,
                              generic_cpy copy_func,
                              generic_op free_func);

array_list* array_list_copy(array_list* array);

void* array_list_get(array_list* array, int index);

int array_list_push(array_list* array, void* data);
int array_list_insert(array_list* array, int index, void* data);
int array_list_remove(array_list* array, int index);
int array_list_remove_last(array_list* array);
int array_list_remove_if(array_list* array, generic_pred pred_func);
int array_list_contains(array_list* array, const void* data);
int array_list_swap(array_list* array, int a_index, int b_index);
int array_list_is_empty(array_list* array);

size_t array_list_length(array_list* array);

void array_list_empty(array_list* array);
void array_list_kill(array_list* array);
void array_list_dissolve(array_list* array);
void array_list_traverse(array_list* array, generic_op do_func);

#endif

/*!
  \file arraylist.h
  \brief Arraylist header file.
  \details Header file for arraylist, contains all functions and declarations
  a user may call.
  \note Traditional zero-style indexing is used for this dynamic array implementation. The first index is '0', second '1', etc.
  \warning It is assumed that all data inserted into the array is heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
  \var typedef struct dynamic_array array_list
  \brief Typedef of struct dynamic_array to 'array_list'.
*/

/*!
  \struct dynamic_array arraylist.h "include/arraylist.h"
  \brief Fundamental dynamic array structure.
  \var dynamic_array::memory
  Member 'memory' represents the basis of the memory space allocated to the array.
  \var dynamic_array::allocated
  Member 'allocated' represents the total memory space allocated to the array.
  In other words, it is the capacity of the array.
  \var dynamic_array::used
  Member 'used' represents how much of the total memory space has been occupied.
  In other words, it is the size of the array.
  \var dynamic_array::index
  Member 'index' represents the index that is the back, or tail, of the occupied portion of the array.
  \var dynamic_array::comp_func
  Member 'comp_func' represents a generic_comp function pointer to be used by the array's comparision based functions.
  \var dynamic_array::copy_func
  Member 'copy_func' represents a generic_cpy function pointer to be used by the array's copy based functions.
  \var dynamic_array::free_func
  Member 'free_func' represents a generic_op function pointer to be used by the array's deconstruction based functions.
*/

/*!
  \fn array_list* array_list_create(generic_comp comp_func,
                                    generic_cpy copy_func,
                                    generic_op free_func)
  \brief Creates and returns an allocated arraylist.
  \param comp_func A generic_comp style function pointer, used to compare data held within arraylist.
  \param copy_func A generic_cpy style function pointer, used to copy data held within arraylist.
  \param free_func A generic_op style function pointer, used to deallocate data held within arraylist.
  \return Newly allocated arraylist with given function pointer type properties.
*/

/*!
  \fn array_list* array_list_copy(array_list* array)
  \brief Creates and returns an allocated copy of the inputted arraylist.
  \param array arraylist to be copied.
  \return Newly allocated arraylist that is a copy of the inputted arraylist.
*/

/*!
  \fn void* array_list_get(array_list* array, int index)
  \brief Returns data from arraylist at given index.
  \param array arraylist to have index value returned.
  \param index Index of retrevial.
  \return Data pointer at specified index. NULL if index is not valid or failure occured.
*/

/*!
  \fn int array_list_push(array_list* array, void* data)
  \brief Appends data at end, or tail, of arraylist.
  \param array arraylist to have data appended to.
  \param data Pointer to data.
  \return 1 if appending to the arraylist was successful, 0 if not. 
*/

/*!
  \fn int array_list_insert(array_list* array, int index, void* data)
  \brief Inserts data into arraylist at given index.
  \param array arraylist to have data inserted into.
  \param index Index of insertion.
  \param data Pointer to data.
  \return 1 if insertion was successful, 0 if not or if given index was not valid.
  \note If the given index is already occupied by data,
  then that data will be deallocated before the new data is inserted.
*/

/*!
  \fn int array_list_remove(array_list* array, int index)
  \brief Removes data from arraylist at given index.
  \param array arraylist to have indexed data removed.
  \param index Index of removal.
  \return 1 if removal was successful, 0 if not or if given index was not valid.
  \note Passing in an index of array->index functions the same as calling array_list_remove_last.
  \note This does not alter the 'used' value held within the arraylist, unless array->index.
*/

/*!
  \fn int array_list_remove_last(array_list* array)
  \brief Removes data from end, or tail, of arraylist.
  \param array arraylist to have end/tail data removed.
  \return 1 if removal was successful, 0 if not.
  \note This action decriments the space cosidered valid for indexing.
  \note For example, calling this function on an arraylist with 3 datum inserted will result in only
  indexes 0 and 1 remaining valid.
  \note Because of this, the 'used' value held within the arraylist is also decremented accordingly.
*/

/*!
  \fn int array_list_remove_if(array_list* array, generic_pred pred_func)
  \brief Removes values from arraylist that return "true" for the given predicate function.
  \param array arraylist to have entries checked.
  \param pred_func Predicate function pointer.
  \return Integer representing number of entries deleted.
*/

/*!
  \fn int array_list_contains(array_list* array, const void* data)
  \brief Checks arraylist for inputted data.
  \param array arraylist to have its entries checked.
  \param data Data to check arraylist entries against.
  \return 1 if arraylist contains data, 0 if not.
*/

/*!
  \fn int array_list_swap(array_list* array, int a_index, int b_index)
  \brief Swaps two entries in arraylist as given by two indexes.
  \param array arraylist to have two of its entires swapped.
  \param a_index Index of one entry.
  \param b_index Index of one entry.
  \return 1 if swap was successful, 0 if not or if given index(es) proved invalid. 
*/

/*!
  \fn int array_list_is_empty(array_list* array)
  \brief Checks if arraylist is empty.
  \param array arraylist to be checked for content.
  \return 1 if arraylist is empty, 0 if not.
*/
   
/*!
  \fn size_t array_list_length(array_list* array)
  \brief Checks length of arraylist.
  \param array Given arraylist.
  \return Length of arraylist.
  \note Length is defined as the maximum valid index + 1.
*/

/*!
  \fn void array_list_empty(array_list* array)
  \brief Deallocates all entries in arraylist.
  \param array arraylist to be emptied.
  \note The arraylist itself is still valid and re-usable after this call.
  \note It \b has \b not been deallocated.
*/

/*!
  \fn void void array_list_kill(array_list* array)
  \brief Completely deallocates the arraylist.
  \param array arraylist to be deallocated.
  \note The arraylist itself is \b not valid and \b not re-usable after this call.
  \note It \b has been deallocated.
*/

/*!
  \fn void void array_list_dissolve(array_list* array)
  \brief Partial deallocation the arraylist, data will remain allocated.
  \param array arraylist to be dissolved.
  \note This method works like array_list_kill, except that the data is not deallocated.
  \note In other words, the struct is freed, but any inserted data is left alone.
  \note This is useful if the data has a planned lifespan longer than the struct,
  or if the data is shared between multiple structures.
  \note Also much like array_list_kill, the arraylist is \b not valid after this call.
*/

/*!
  \fn void array_list_traverse(array_list* array, generic_op do_func)
  \brief Traverses all datum in the arraylist, applies do_func to each datum.
  \param array arraylist to be traversed.
  \param do_func Operation function pointer.
*/
