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
