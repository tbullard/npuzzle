#include "heap.h"

typedef struct heap_node {
    void* data;
    int index;
} node;

static node* heap_node_ceate(int index, void* data);

static node* heap_node_ceate(int index, void* data) {
    node* new_node;
    new_node = malloc(sizeof(node));
    new_node->data = data;
    new_node->index = index;
    return new_node;
}

heap* heap_create(unsigned int capacity,
                  heap_comp heap_func,
                  generic_comp comp_func,
                  generic_cpy copy_func,
                  generic_op free_func) {
    int i;
    heap* new_heap;
    new_heap = malloc(sizeof(heap));
    new_heap->heap_func = heap_func;
    new_heap->comp_func = comp_func;
    new_heap->copy_func = copy_func;
    new_heap->free_func = free_func;
    new_heap->size = 0;
    new_heap->capacity = capacity ? capacity : DEFAULT_HEAP_CAPACY;
    new_heap->heap_memory = malloc(new_heap->capacity * sizeof(node*));
    for(i = 0; i < new_heap->capacity; i++) {
        new_heap->heap_memory[i] = NULL;
    }
    return new_heap;
}

heap* heap_copy(heap* p_queue) {
    int i;
    heap* new_heap;
    node* cur_node;
    new_heap = NULL;
    if(p_queue) {
        new_heap = heap_create(p_queue->capacity,
                               p_queue->heap_func,
                               p_queue->comp_func,
                               p_queue->copy_func,
                               p_queue->free_func);
        for(i = 0; i < p_queue->size; i++) {
            cur_node = p_queue->heap_memory[i];
            new_heap->heap_memory[i] =
                heap_node_ceate(i, new_heap->copy_func(cur_node->data));
        }
        new_heap->size = p_queue->size;
    }
    return new_heap;
}

void* heap_peek(heap* p_queue) {
    void* data;
    data = NULL;
    if(p_queue) {
        data = p_queue->heap_memory[0]->data;
    }
    return data;
}

void* heap_add(heap* p_queue, void* data) {
    int heapd, i;
    node* new_node, *tmp_node;
    new_node = NULL;
    heapd = 0;
    if(p_queue) {
        new_node = heap_node_ceate(0, data);
        if(p_queue->size == p_queue->capacity) {
            p_queue->heap_memory = realloc(p_queue->heap_memory, 2 *
                                           p_queue->capacity * sizeof(node*));
            p_queue->capacity *= 2;
        }
        heapd = 0;
        p_queue->heap_memory[p_queue->size] = new_node;
        i = p_queue->size;
        p_queue->size++;
        while(!heapd && i) {
            tmp_node = p_queue->heap_memory[(i - 1) >> 1];
            if(p_queue->heap_func(tmp_node->data, new_node->data)) {
                p_queue->heap_memory[(i - 1) >> 1] = new_node;
                p_queue->heap_memory[i] = tmp_node;
                tmp_node->index = i;
                new_node->index = (i - 1) >> 1;
                i = (i - 1) >> 1;
            } else {
                new_node->index = i;
                heapd = 1;
            }
        }
    }
    return new_node;
}

int heap_remove(heap* p_queue) {
    int removed, heapd, i;
    node* cur_node, *tmp_node, *l_child, *r_child;
    removed = 0;
    if(p_queue) {
        if(!heap_is_empty(p_queue)) {
            removed = 1;
            heapd = i = 0;
            tmp_node = p_queue->heap_memory[0];
            cur_node = p_queue->heap_memory[p_queue->size - 1];
            p_queue->heap_memory[0] = cur_node;
            cur_node->index = 0;
            p_queue->heap_memory[p_queue->size - 1] = NULL;
            p_queue->free_func(tmp_node->data);
            free(tmp_node);
            --p_queue->size;
            if(!heap_is_empty(p_queue)) {
                while(!heapd) {
                    l_child = r_child = NULL;
                    if((i << 1) + 1 < p_queue->size) {
                        l_child = p_queue->heap_memory[(i << 1) + 1];
                    } else {
                        heapd = 1;
                    }
                    if((i + 1) << 1 < p_queue->size) {
                        r_child = p_queue->heap_memory[(i + 1) << 1];
                    }
                    if(l_child && r_child) {
                        if(p_queue->heap_func(r_child->data, l_child->data)) {
                            if(p_queue->heap_func(cur_node->data,
                                                  l_child->data)) {
                                p_queue->heap_memory[i] = l_child;
                                p_queue->heap_memory[(i << 1) + 1] = cur_node;
                                l_child->index = i;
                                cur_node->index = (i << 1) + 1;
                                i = (i << 1) + 1;
                            } else {
                                heapd = 1;
                            }
                        } else if(p_queue->heap_func(cur_node->data,
                                                     r_child->data)) {
                            p_queue->heap_memory[i] = r_child;
                            p_queue->heap_memory[(i + 1) << 1] = cur_node;
                            r_child->index = i;
                            cur_node->index = (i + 1) << 1;
                            i = (i + 1) << 1;
                        } else {
                            heapd = 1;
                        }
                    } else if(l_child){
                        if(p_queue->heap_func(cur_node->data, l_child->data)) {
                            p_queue->heap_memory[i] = l_child;
                            p_queue->heap_memory[(i << 1) + 1] = cur_node;
                            l_child->index = i;
                            cur_node->index = (i << 1) + 1;
                            i = (i + 1) << 1;
                        } else {
                            heapd = 1;
                        }
                    }
                }
            }
        }
    }
    return removed;
}

int heap_up_mod_data(heap* p_queue, void* change_node, void* data) {
    int moded, heapd, i;
    node* cur_node, *tmp_node;
    moded = heapd = 0;
    if(p_queue && change_node) {
        if(!heap_is_empty(p_queue)) {
            moded = 1;
            cur_node = (node*)change_node;
            p_queue->free_func(cur_node->data);
            cur_node->data = data;
            i = cur_node->index;
            cur_node->index = 0;
            while(!heapd && i) {
                tmp_node = p_queue->heap_memory[(i - 1) >> 1];
                if(p_queue->heap_func(tmp_node->data, cur_node->data)) {
                    p_queue->heap_memory[(i - 1) >> 1] = cur_node;
                    p_queue->heap_memory[i] = tmp_node;
                    tmp_node->index = i;
                    i = (i - 1) >> 1;
                } else {
                    cur_node->index = i;
                    heapd = 1;
                }
            }
        }
    }
    return moded;
}

int heap_is_empty(heap* p_queue) {
    int empty;
    empty = 1;
    if(p_queue) {
        if(p_queue->size) {
            empty = 0;
        }
    }
    return empty;
}

int heap_contains(heap* p_queue, const void* data) {
    int contains, i;
    node* cur_node;
    contains = 0;
    if(p_queue) {
        if(!heap_is_empty(p_queue)) {
            for(i = 0; i < p_queue->size && !contains; i++) {
                cur_node = p_queue->heap_memory[i];
                if(!p_queue->comp_func(cur_node->data, data)) {
                    contains = 1;
                }
            }
        }
    }
    return contains;
}

size_t heap_size(heap* p_queue) {
    size_t size;
    if(p_queue) {
        size = p_queue->size;
    }
    return size;
}

void heap_empty(heap* p_queue) {
    int i;
    node* cur_node;
    if(p_queue) {
        if(!heap_is_empty(p_queue)) {
            for(i = 0; i < p_queue->size; i++) {
                cur_node = p_queue->heap_memory[i];
                p_queue->heap_memory[i] = NULL;
                p_queue->free_func(cur_node->data);
                free(cur_node);
            }
        }
    }
    return;
}

void heap_kill(heap* p_queue) {
    if(p_queue) {
        heap_empty(p_queue);
        free(p_queue->heap_memory);
        free(p_queue);
    }
    return;
}

void heap_dissolve(heap* p_queue) {
    int i;
    node* cur_node;
    if(p_queue) {
        for(i = 0; i < p_queue->size; i++) {
            cur_node = p_queue->heap_memory[i];
            p_queue->heap_memory[i] = NULL;
            free(cur_node);
        }
        free(p_queue->heap_memory);
        free(p_queue);
    }
    return;
}

void heap_traverse(heap* p_queue, generic_op do_func) {
    int i;
    node* cur_node;
    if(p_queue) {
        if(!heap_is_empty(p_queue)) {
            for(i = 0; i < p_queue->size; i++) {
                cur_node = p_queue->heap_memory[i];
                do_func(cur_node->data);
            }
        }
    }
    return;
}

/*!
  \file heap.c
  \brief Heap source file.
  \details Header file for heap, contains all functions and declarations.
  \warning It is assumed that all data inserted into the heap is heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
  \var typedef struct heap_node node
  \brief Typedef of struct heap_node to 'node'.
*/

/*!
  \struct heap_node
  \brief Fundamental heap node structure.
  \var heap_node::data
  Member 'data' represents the allocated data in the node.
  \var heap_node::index
  Member 'hash_key' represents the location of the node in the heap.
*/

/*!
  \static
  \fn heap_node_ceate(int index, void* data)
  \brief Creates and returns a newly allocated node.
  \param index Location of node in heap.
  \param data Nodes's data.
  \return Newly allocated node.
*/



