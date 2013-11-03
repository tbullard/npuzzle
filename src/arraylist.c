#include "arraylist.h"

array_list* array_list_create(generic_comp comp_func,
                              generic_cpy copy_func,
                              generic_op free_func) {
    array_list* new_array;
    new_array = malloc(sizeof(array_list));
    new_array->comp_func = comp_func;
    new_array->copy_func = copy_func;
    new_array->free_func = free_func;
    new_array->memory = NULL;
    new_array->allocated = 0;
    new_array->used = 0;
    new_array->index = -1;
    return new_array;
}

array_list* array_list_copy(array_list* array) {
    int i;
    void* data;
    array_list* new_array;
    new_array = NULL;
    if(array) {
        new_array = array_list_create(array->comp_func,
                                      array->copy_func,
                                      array->free_func);
        for(i = 0; i < array_list_length(array); i++) {
            data = array->memory[i] ? array->copy_func(array->memory[i]) : NULL;
            array_list_push(new_array, data);
        }
    }
    return new_array;
}

void* array_list_get(array_list* array, int index) {
    void* data;
    data = NULL;
    if(array) {
        if (index >= 0 && index <= array->index) {
            data = array->memory[index];
        }
    }
    return data;
}

int array_list_push(array_list* array, void* data) {
    int pushed;
    size_t toallocate;
    size_t size;
    size = sizeof(void*);
    pushed = 0;
    if(array) {
        pushed = 1;
        if ((array->allocated - array->used) < size) {
            toallocate = (array->allocated == 0) ? size :
                         (array->allocated * 2);
            array->memory = realloc(array->memory, toallocate);
            array->allocated = toallocate;
        }
        array->memory[++array->index] = data;
        array->used += size;
    }
    return pushed;
}

int array_list_insert(array_list* array, int index, void* data) {
    int inserted;
    inserted = 0;
    if(array) {
        if (index >= 0 && index <= array->index) {
            inserted = 1;
            array->memory[index] = data;
        }
    }
    return inserted;
}

int array_list_remove_last(array_list* array) {
    int removed;
    size_t size;
    removed = 0;
    size = sizeof(void*);
    if(array) {
        if(array->memory[array->index]) {
            removed = 1;
            array->free_func(array->memory[array->index]);
            array->memory[array->index] = NULL;
            --array->index;
            array->used -= size;
        }
    }
    return removed;
}

int array_list_remove(array_list* array, int index) {
    int removed;
    if(array) {
        if((index >= 0 && index <= array->index) && array->memory[index]) {
            removed = 1;
            if(index == array->index) {
                array_list_remove_last(array);
            } else {
                array->free_func(array->memory[index]);
                array->memory[index] = NULL;
            }
        }
    }
    return removed;
}

int array_list_remove_if(array_list* array, generic_pred pred_func) {
    int i, count;
    count = 0;
    if(array) {
        for(i = 0; i < array_list_length(array); i++) {
            if(array->memory[i]) {
                if(pred_func(array->memory[i])) {
                    array_list_remove(array, i);
                    count++;
                }
            }
        }
    }
    return count;
}

int array_list_contains(array_list* array, const void* data) {
    int i, contains;
    contains = 0;
    if(array) {
        for(i = 0; i < array_list_length(array) && !contains; i++) {
            if(array->memory[i]) {
                if(!array->comp_func(data, array->memory[i])) {
                    contains = 1;
                }
            }
        }
    }
    return contains;
}

int array_list_swap(array_list* array, int a_index, int b_index) {
    int swapped;
    void* a_data;
    void* b_data;
    swapped = 0;
    if(array) {
        if((a_index >= 0 && b_index >= 0) &&
           (a_index <= array->index && b_index <= array->index)) {
            swapped = 1;
            a_data = array->memory[a_index] ? array->memory[a_index] : NULL;
            b_data = array->memory[b_index] ? array->memory[b_index] : NULL;
            array_list_insert(array, a_index, b_data);
            array_list_insert(array, b_index, a_data);
        }
    }
    return swapped;
}

int array_list_is_empty(array_list* array) {
    int empty;
    empty = 1;
    if(array) {
        if(array->index >= 0) {
            empty = 0;
        }
    }
    return empty;
}

size_t array_list_length(array_list* array) {
    size_t length;
    length = 0;
    if(array) {
        length = array->index + 1;
    }
    return length;
}

void array_list_empty(array_list* array) {
    int i;
    if(array) {
        for(i = 0; i < array_list_length(array); i++) {
            array->free_func(array->memory[i]);
            array->memory[i] = NULL;
        }
        array->used = 0;
        array->index = -1;
    }
    return;
}

void array_list_kill(array_list* array) {
    if(array) {
        array_list_empty(array);
        free(array->memory);
        free(array);
    }
    return;
}

void array_list_dissolve(array_list* array) {
    int i;
    if(array) {
        for(i = 0; i < array_list_length(array); i++) {
            array->memory[i] = NULL;
        }
        free(array->memory);
        free(array);
    }
    return;
}

void array_list_traverse(array_list* array, generic_op do_func) {
    int i;
    if(array) {
        for(i = 0; i < array_list_length(array); i++) {
            if(array->memory[i]) {
                do_func(array->memory[i]);
            }
        }
    }
    return;
}
