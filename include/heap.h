#ifndef HEAP_H
#define HEAP_H

#include "ugf.h"

#define DEFAULT_HEAP_CAPACY 89

typedef int (*heap_comp)(const void*, const void*);

struct heap_node;

typedef struct priority_queue {
    struct heap_node** heap_memory;
    size_t size;
    size_t capacity;
    heap_comp heap_func;
    generic_comp comp_func;
    generic_cpy copy_func;
    generic_op free_func;
} heap;

heap* heap_create(unsigned int capacity,
                  heap_comp heap_func,
                  generic_comp comp_func,
                  generic_cpy copy_func,
                  generic_op free_func);

heap* heap_copy(heap* p_queue);

void* heap_peek(heap* p_queue);
void* heap_add(heap* p_queue, void* data);

int heap_remove(heap* p_queue);
int heap_up_mod_data(heap* p_queue, void* change_node, void* data);
int heap_is_empty(heap* p_queue);
int heap_contains(heap* p_queue, const void* data);

size_t heap_size(heap* p_queue);

void heap_empty(heap* p_queue);
void heap_kill(heap* p_queue);
void heap_dissolve(heap* p_queue);
void heap_traverse(heap* p_queue, generic_op do_func);

#endif

/*!
  \file heap.h
  \brief Addressable Heap header file.
  \details Header file for heap, contains all functions and declarations
  a user may call.
  \warning It is assumed that all data inserted into the priority queue is heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
  \def DEFAULT_HEAP_CAPACY
  \brief Default heap capacity, used in heap_create fucntion if invalid capacity is given.
*/

/*!
   \var typedef int (*heap_comp)(const void*, const void*)
   \brief Typedef form of heap comparison functions.
   \param Pointer to \a data-1 being examined.
   \param Pointer to \a data-2 being examined.
   \return 1 if by priority \a data-1 should be behind \a data-2, 0 if otherwise.
   \details An example using ints that establishes a min-heap:
   \code{.c}
int integer_heap_comp(const void* int1, const void* int2) {
    int heap_comp_value;
    heap_comp_value = 0;
    if(*(int*)int1 > *(int*)int2) {
        heap_comp_value = 1;
    return comp_value;
}
   \endcode
*/

/*!
  \var typedef struct priority_queue heap
  \brief Typedef of struct priority_queue to 'heap'.
*/

/*!
  \struct priority_queue heap.h "include/heap.h"
  \brief Fundamental heap structure.
  \var heap::heap_memory
  Member 'heap_memory' represents the basis of the heap in the form of a dynamic memory space.
  \var heap::size
  Member 'size' represents how much of member 'heap_memory' is occupied.
  \var heap::capacity
  Member 'capacity' represents how much total space 'heap_memory' has available.
  \var heap::heap_func
  Member 'heap_func' represents a heap_comp function pointer to be used in establishing priority in the heap.
  \var heap::comp_func
  Member 'comp_func' represents a generic_comp function pointer to be used by the heap's comparision based functions.
  \var heap::copy_func
  Member 'copy_func' represents a generic_copy function pointer to be used by the heap's copy based functions.
  \var heap::free_func
  Member 'free_func' represents a generic_op function pointer to be used by the heap's deallocation based functions.
*/

/*!
  \fn heap* heap_create(unsigned int capacity,
                        heap_comp heap_func,
                        generic_comp comp_func,
                        generic_cpy copy_func,
                        generic_op free_func)
  \brief Creates and returns a newly allocated heap.
  \param capacity Inital allocated size of heap.
  \param heap_func A heap_comp style function pointer, used to establish heap priority.
  \param comp_func A generic_comp style function pointer, used to compare data held within heap for non-priority purposes.
  \param copy_func A generic_cpy style function pointer, used to copy data held within heap.
  \param free_func A generic_op style function pointer, used to deallocate data held within heap.
  \return Newly allocated heap with given function pointer type properties and capacity.
*/

/*!
  \fn void* heap_copy(heap* p_queue)
  \brief Creates and returns an allocated copy of the inputted heap.
  \param p_queue Heap to be copied.
  \return Newly allocated heap that is a copy of the inputted heap.
*/

/*!
  \fn void* heap_peek(heap* p_queue)
  \brief Returns pointer to data at the front of the heap.
  \param p_queue Heap to have front checked.
  \return Pointer to data at front of heap.
  \note This action does not remove the element that is at the front of the heap.
  \return Point to data at front of heap.
*/

/*!
  \fn void* heap_add(heap* p_queue, void* data)
  \brief Adds given data to heap.
  \param p_queue Heap to have data inserted into.
  \param data Data that is to be inserted into the heap.
  \return Pointer to the location of the inserted data.
  \note Regarding the return, this is used for heap_up_mod_data as it represents the 'change_node'.
  \note See heap_up_mod_data for more details.
*/

/*!
  \fn int heap_remove(heap* p_queue)
  \brief Removes data at front of heap.
  \param p_queue Heap to have front removed.
  \return 1 if removal was successful, 0 if not.
  \note The data will be deallocated in this process.
  \note If the data is desired as being used after removal, the following is suggested.
  \note Aquire a pointer to the data by calling heap_peek, and then make a newly allocated copy of that data.
*/

/*!
  \fn int heap_up_mod_data(heap* p_queue, void* change_node, void* data)
  \brief Increases the priority of a node.
  \param p_queue Heap to have front removed.
  \param change_node Pointer to location of node to modify.
  \param data New data to put in node.
  \return 1 if modification was successful, 0 if not.
  \note The old data of the node will be deallocated in this process.
*/

/*!
  \fn int heap_is_empty(heap* p_queue)
  \brief Check to see if heap is empty.
  \param p_queue Heap to have size checked.
  \return 1 if heap is empty, 0 if not.
*/

/*!
  \fn int heap_contains(heap* p_queue, const void* data)
  \brief Check to see if heap contains given data.
  \param p_queue Heap to be searched.
  \param data Data to be searched for.
  \return 1 if heap contains data, 0 if not.
*/

/*!
  \fn size_t heap_size(heap* p_queue)
  \brief Checks total number of dataum in heap.
  \param p_queue Heap to have dataum counted.
  \return Number of datum in heap.
*/

/*!
  \fn void heap_empty(heap* p_queue);
  \brief Deallocates data held within heap.
  \param p_queue Heap to be emptied.
  \note The heap itself is still valid and re-usable after this call.
  \note It \b has \b not been deallocated.
*/

/*!
  \fn void heap_kill(heap* p_queue)
  \brief Completely deallocates the heap.
  \param p_queue Heap to be deallocated.
  \note The heap itself is \b not valid and \b not re-usable after this call.
  \note It \b has been deallocated.
*/

/*!
  \fn void heap_dissolve(heap* p_queue)
  \brief Partial deallocation of the heap, data remains allocated.
  \param p_queue Heap to be dissolved.
  \note This method works like heap_kill, except that the data is not deallocated.
  \note In other words, the struct is freed, but any inserted data is left alone.
  \note This is useful if the data has a planned lifespan longer than the struct,
  or if the data is shared between multiple structures.
  \note Also much like heap_kill, the heap is \b not valid after this call.
*/

/*!
  \fn void heap_traverse(heap* p_queue, generic_op do_func)
  \brief Traverses all datum in the heap, applies do_func to each data.
  \param p_queue Array list to be traversed.
  \param do_func Operation function pointer.
*/


