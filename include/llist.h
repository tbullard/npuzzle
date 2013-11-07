#ifndef LLIST_H
#define LLIST_H

#include "ugf.h"

struct list_node;

typedef struct linked_list {
    struct list_node* head;
    struct list_node* tail;
    size_t size;
    generic_comp comp_func;
    generic_cpy copy_func;
    generic_op free_func;
} list;

list* list_create(generic_comp comp_func,
                  generic_cpy copy_func,
                  generic_op free_func);

list* list_copy(list* llist);

void list_push_front(list* llist, void* data);
void list_push_back(list* llist, void* data);
int list_insert_at(list* llist, int index, void* data);

int list_remove_front(list* llist);
int list_remove_back(list* llist);
int list_remove_if(list* llist, generic_pred pred_func);
int list_remove_at_index(list* llist, int index);

void* list_front(list* llist);
void* list_back(list* llist);
void* list_get_at(list* llist, int index);
int list_contains(list* llist, const void* data);
int list_is_empty(list* llist);
int list_pop(list* llist);
int list_deque(list* llist);

size_t list_size(list* llist);

void list_empty(list* llist);
void list_kill(list* llist);
void list_dissolve(list* llist);
void list_traverse(list* llist, generic_op do_func);

#endif

/*!
  \file llist.h
  \brief Linked List header file.
  \details Header file for linked list, contains all functions and declarations
  a user may call.
  \note Traditional zero-style indexing is used for this linked list implementation. The first index is '0', second '1', etc.
  \warning It is assumed that all data inserted into the list is heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
  \var typedef struct linked_list list
  \brief Typedef of struct linked_list to 'list'.
*/

/*!
  \struct linked_list llist.h "include/llist.h"
  \brief Fundamental linked list structure.
  \var linked_list::head
  Member 'head' represents a pointer to the lnode head of the list.
  \var linked_list::tail
  Member 'tail' represents a pointer to the lnode tail of the list.
  \var linked_list::size
  Member 'size' represents the size of the list.
  \var linked_list::comp_func
  Member 'comp_func' represents a generic_comp function pointer to be used by the list's comparision based functions.
  \var linked_list::copy_func
  Member 'copy_func' represents a generic_cpy function pointer to be used by the list's copy based functions.
  \var linked_list::free_func
  Member 'free_func' represents a generic_op function pointer to be used by the list's deconstruction based functions.
*/

/*!
  \fn list* list_create(generic_comp comp_func,
                        generic_cpy copy_func,
                        generic_op free_func)
  \brief Creates and return an allocated list.
  \param comp_func A generic_comp style function pointer, used to compare data held within list.
  \param copy_func A generic_cpy style function pointer, used to copy data held within list.
  \param free_func A generic_op style function pointer, used to deallocate data held within list.
  \return Newly allocated list with given function pointer type properties.
*/

/*!
  \fn list* list_copy(list* llist)
  \brief Creates and return an allocated copy of the inputted list.
  \param llist Linked list to be copied.
  \return Newly allocated list that is a copy of the inputted list.
*/

/*!
  \fn void list_push_front(list* llist, void* data)
  \brief Inserts a new piece of data at the front, or head of the given list.
  \param llist Linked list to have data added to the front/head of.
  \param data Pointer to data being inserted into the linked list.
*/

/*!
  \fn void list_push_back(list* llist, void* data)
  \brief Inserts a new piece of data at the back, or tail of the given list.
  \param llist Linked list to have data added to the back/tail of.
  \param data Pointer to data being inserted into the linked list.
*/

/*!
  \fn int list_insert_at(list* llist, int index, void* data)
  \brief Inserts a new piece of data at the given index of the list.
  \param llist Linked list to have data added to it.
  \param index Index of insertion for the data.
  \param data Pointer to data being inserted into the linked list.
  \return 1 if insertion was successful, 0 if not.
  \note Passing in index '0' is the same as calling the list_push_front function.
  \note Passing in an index of llist->size - 1  is the same as calling the list_push_back function. 
*/

/*!
  \fn int list_remove_front(list* llist)
  \brief Removes the front, or head, of the linked list.
  \param llist Linked list to have front/head removed.
  \return 1 if removal was successful, 0 if not.
*/

/*!
  \fn int list_remove_back(list* llist)
  \brief Removes the back, or tail, of the linked list.
  \param llist Linked list to have back/tail removed.
  \return 1 if removal was successful, 0 if not.
*/

/*!
  \fn int list_remove_if(list* llist, generic_pred pred_func)
  \brief Removes from linked list values that return "true" for the given predicate funtion.
  \param llist Linked list to have entries checked.
  \param pred_func Predicate function pointer.
  \return Integer representing number of entries deleted.
*/

/*!
  \fn int list_remove_at_index(list* llist, int index)
  \brief Removes from linked list value at given index.
  \param llist Linked list to have indexed value removed.
  \param index Index of removal.
  \return 1 if removal was successful, 0 if not.
  \note Passing in index '0' is the same as calling the list_remove_front function.
  \note Passing in an index of llist->size - 1  is the same as calling the list_remove_back function. 
*/

/*!
  \fn void* list_front(list* llist)
  \brief Return data within the front, or head, of the linked list.
  \param llist Linked list to have head/front data returned.
  \return Head/front data pointer. NULL if failure occured.
*/

/*!
  \fn void* list_back(list* llist)
  \brief Return data within the back, or tail, of the linked lst.
  \param llist Linked list to have back/tail data returned.
  \return Back/tail data pointer. NULL if failure occured.
*/

/*!
  \fn void* list_get_at(list* llist, int index)
  \brief Return data from linked list at given index.
  \param llist Linked list to have index value returned.
  \param index Index of retrieval. 
  \return Data pointer at specified index. NULL if index is not valid or failure occured.
  \note Passing in index '0' is the same as calling the list_front function.
  \note Passing in an index of llist->size - 1  is the same as calling the list_back function. 
*/

/*!
  \fn int list_contains(list* llist, const void* data)
  \brief Checks linked list for inputted data.
  \param llist Linked list to have its entries checked.
  \param data Data to check linked list entries against.
  \return 1 if linked list contains data, 0 if not.
*/

/*!
  \fn int list_is_empty(list* llist)
  \brief Checks linked list for emptiness.
  \param llist Linked list to be checked for emptiness.
  \return 1 if empty, 0 if not.
*/

/*!
  \fn size_t list_size(list* llist)
  \brief Checks linked list size.
  \param llist Linked list to have size checked.
  \return Number of datum stored in linked list.
*/

/*!
  \fn int list_pop(list* llist)
  \brief Removes the front, or head, of the linked list.
  \param llist Linked list to have front/head removed.
  \return 1 if removal was successful, 0 if not.
  \note This differs from the list_remove_front function in that the data in the head is not deallocated.
*/

/*!
  \fn int list_deque(list* llist)
  \brief Removes the back, or tail, of the linked list.
  \param llist Linked list to have back/tail removed.
  \return 1 if removal was successful, 0 if not.
  \note This differs from the list_remove_back function in that the data in the tail is not deallocated.
*/

/*!
  \fn void list_empty(list* llist)
  \brief Removes from the linked list all datum held inside of it.
  \param llist Linked list to be emptied.
  \note The Linked list itself is still valid and re-usable after this call.
  \note It \b has \b not been deallocated.
*/

/*!
  \fn void list_kill(list* llist)
  \brief Completely deallocates the linked list.
  \param llist Linked list to be deallocated.
  \note The Linked list itself is \b not valid and \b not re-usable after this call.
  \note It \b has been deallocated.
*/

/*!
  \fn void list_dissolve(list* llist)
  \brief Partial deallocation of the list, data remains allocated.
  \param llist List to be dissolved.
  \note This method works like list_kill, except that the data is not deallocated.
  \note In other words, the struct is freed, but any inserted data is left alone.
  \note This is useful if the data has a planned lifespan longer than the struct,
  or if the data is shared between multiple structures.
  \note Also much like list_kill, the list is \b not valid after this call.
*/

/*!
  \fn void list_traverse(list* llist, generic_op do_func)
  \brief Traverses all datum in the linked list, applies do_func to each data.
  \param llist Linked list to be traversed.
  \param do_func Operation function pointer.
*/
