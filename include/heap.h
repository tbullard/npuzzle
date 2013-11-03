#ifndef HEAP_H
#define HEAP_H

#include "ugf.h"

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

heap* heap_create(unsigned int init_capacity,
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
