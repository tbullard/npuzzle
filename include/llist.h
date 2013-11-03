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
