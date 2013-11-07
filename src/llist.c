#include "llist.h"

typedef struct list_node {
    struct list_node* prev;
    struct list_node* next;
    void* data;
} node;

static node* list_node_create(void* data);

static node* list_node_create(void* data) {
    node* new_node;
    new_node = malloc(sizeof(node));
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->data = data;
    return new_node;
}

list* list_create(generic_comp comp_func,
                  generic_cpy copy_func,
                  generic_op free_func) {
    list* new_list;
    new_list = malloc(sizeof(list));
    new_list->comp_func = comp_func;
    new_list->copy_func = copy_func;
    new_list->free_func = free_func;
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
}

list* list_copy(list* llist) {
    list* new_list;
    node* cur_node;
    new_list = NULL;
    if(llist) {
        new_list = list_create(llist->comp_func,
                               llist->copy_func,
                               llist->free_func);
        cur_node = llist->head;
        while(cur_node) {
            list_push_back(new_list, llist->copy_func(cur_node->data));
            cur_node = cur_node->next;
        }
    }
    return new_list;
}

void list_push_front(list* llist, void* data) {
    node* new_node;
    if(llist) {
        new_node = list_node_create(data);
        if(llist->head) {
            llist->head->prev = new_node;
        } else {
            llist->tail = new_node;
        }
        new_node->next = llist->head;
        llist->head = new_node;
        llist->size++;
    }
    return;
}

void list_push_back(list* llist, void* data) {
    node* new_node;
    if(llist) {
        new_node = list_node_create(data);
        if(llist->tail) {
            llist->tail->next = new_node;
        } else {
            llist->head = new_node;
        }
        new_node->prev = llist->tail;
        llist->tail = new_node;
        llist->size++;
    }
    return;
}

int list_insert_at(list* llist, int index, void* data) {
    int insert, cur_Index;
    node* cur_node;
    node* new_node;
    insert = 0;
    if(llist) {
        insert = (index > -1 && index <= llist->size) ? 1 : 0;
        if(insert) {
            if(index) {
                if(index != llist->size) {
                    new_node = list_node_create(data);
                    cur_Index = 0;
                    cur_node = llist->head;
                    while(cur_Index != index) {
                        cur_node = cur_node->next;
                        cur_Index++;
                    }
                    cur_node->prev->next = new_node;
                    new_node->prev = cur_node->prev;
                    new_node->next = cur_node;
                    cur_node->prev = new_node;
                    llist->size++;
                } else {
                    list_push_back(llist, data);
                }
            } else {
                list_push_front(llist, data);
            }
        }
    }
    return insert;
}

int list_remove_front(list* llist) {
    int removeal;
    removeal = 0;
    if(llist) {
        removeal = !list_is_empty(llist) ? 1 : 0;
        if(removeal) {
            --llist->size;
            llist->free_func(llist->head->data);
            if(llist->size) {
                llist->head = llist->head->next;
                llist->head->prev->next = NULL;
                free(llist->head->prev);
                llist->head->prev = NULL;
            } else {
                llist->tail = NULL;
                free(llist->head);
                llist->head = NULL;
            }
        }
    }
    return removeal;
}

int list_remove_back(list* llist) {
    int removeal;
    removeal = 0;
    if(llist) {
        removeal = !list_is_empty(llist) ? 1 : 0;
        if(removeal) {
            --llist->size;
            llist->free_func(llist->tail->data);
            if(llist->size) {
                llist->tail = llist->tail->prev;
                llist->tail->next->prev = NULL;
                free(llist->tail->next);
                llist->tail->next = NULL;
            } else {
                llist->tail = NULL;
                free(llist->head);
                llist->head = NULL;
            }
        }
    }
    return removeal;
}

int list_pop(list* llist) {
    int popped;
    node* cur_node;
    popped = 0;
    if(llist) {
        if(!list_is_empty(llist)) {
            popped = 1;
            --llist->size;
            cur_node = llist->head;
            llist->head = llist->head->next;
            if(llist->head) {
                llist->head->prev = NULL;
            } else {
                llist->tail = NULL;
            }
            cur_node->next = NULL;
            free(cur_node);
        }
    }
    return popped;
}

int list_deque(list* llist) {
    int dequed;
    node* cur_node;
    dequed = 0;
    if(llist) {
        if(!list_is_empty(llist)) {
            dequed = 1;
            --llist->size;
            cur_node = llist->tail;
            llist->tail = llist->tail->prev;
            if(llist->tail) {
                llist->tail->next = NULL;
            } else {
                llist->head = NULL;
            }
            cur_node->prev = NULL;
            free(cur_node);
        }
    }
    return dequed;
}

int list_remove_at_index(list* llist, int index) {
    int removeal, found, cur_Index;
    node* cur_node;
    cur_node = NULL;
    removeal = 0;
    if(llist) {
        found = cur_Index = 0;
        removeal = (index > -1 && !list_is_empty(llist) &&
                    index < llist->size) ? 1 : 0;
        if(removeal) {
            if(!index) {
                list_remove_front(llist);
            } else if(index == (llist->size - 1)) {
                list_remove_back(llist);
            } else {
                cur_node = llist->head;
                while(!found) {
                    if(cur_Index == index) {
                        found = 1;
                        --llist->size;
                        llist->free_func(cur_node->data);
                        cur_node->next->prev = cur_node->prev;
                        cur_node->prev->next = cur_node->next;
                        cur_node->next = NULL;
                        cur_node->prev = NULL;
                        free(cur_node);
                    } else {
                        cur_node = cur_node->next;
                        cur_Index++;
                    }
                }
            }
        }
    }
    return removeal;
}

int list_remove_if(list* llist, generic_pred pred_func) {
    int count;
    node* past_node;
    node* cur_node;
    cur_node = NULL;
    past_node = NULL;
    count = 0;
    if(llist) {
        if(!list_is_empty(llist)) {
            past_node = llist->head;
            cur_node = llist->head->next;
            while(past_node && pred_func(past_node->data)) {
                past_node = past_node->next;
                if(past_node) {
                    cur_node = past_node->next;
                }
                list_remove_front(llist);
                count++;
            }
            while(cur_node) {
                if(pred_func(cur_node->data)) {
                    if(cur_node != llist->tail) {
                        --llist->size;
                        llist->free_func(cur_node->data);
                        cur_node = cur_node->next;
                        cur_node->prev = cur_node->prev->prev;
                        past_node = past_node->next;
                        past_node->prev->next = past_node->next;
                        past_node->prev = NULL;
                        past_node->next = NULL;
                        free(past_node);
                        past_node = cur_node->prev;
                    } else {
                        list_remove_back(llist);
                        past_node = NULL;
                        cur_node = NULL;
                    }
                    count++;
                } else {
                    past_node = past_node->next;
                    cur_node = past_node->next;
                }
            }
        }
    }
    return count;
}

void* list_get_at(list* llist, int index) {
    int cur_Index, found, in_range;
    node* cur_node;
    cur_node = NULL;
    if(llist) {
        cur_node = llist->head;
        cur_Index = found = 0;
        in_range = (index > -1 && !list_is_empty(llist) &&
                    index < llist->size) ? 1 : 0;
        if(in_range) {
            while(!found) {
                if(cur_Index == index) {
                    found = 1;
                } else {
                    cur_node = cur_node->next;
                    cur_Index++;
                }
            }
        }
    }
    return cur_node ? cur_node->data : NULL;
}

void* list_front(list* llist) {
    void* data;
    data = NULL;
    if(llist) {
        data = !list_is_empty(llist) ? llist->head->data : NULL;
    }
    return data;
}

void* list_back(list* llist) {
    void* data;
    data = NULL;
    if(llist) {
        data = !list_is_empty(llist) ? llist->tail->data : NULL;
    }
    return data;
}

int list_contains(list* llist, const void* data) {
    int found;
    node* cur_node;
    found = 0;
    if(llist) {
        cur_node = llist->head;
        if(!list_is_empty(llist)) {
            while(!found && cur_node) {
                if(!llist->comp_func(cur_node->data, data)) {
                    found = 1;
                } else {
                    cur_node = cur_node->next;
                }
            }
        }
    }
    return found;
}

int list_is_empty(list* llist) {
    int empty;
    empty = 1;
    if(llist) {
        if(llist->size) {
            empty = 0;
        }
    }
    return empty;
}

size_t list_size(list* llist) {
    int size;
    size = 0;
    if(llist) {
        size = llist->size;
    }
    return size;
}

void list_empty(list* llist) {
    if(llist) {
        while(llist->size) {
            list_remove_front(llist);
        }
    }
    return;
}

void list_kill(list* llist) {
    if(llist) {
        list_empty(llist);
        free(llist);
    }
    return;
}

void list_dissolve(list* llist) {
    if(llist) {
        while(!list_is_empty(llist)) {
            list_pop(llist);
        }
        free(llist);
    }
    return;
}

void list_traverse(list* llist, generic_op do_func) {
    node* cur_node;
    if(llist) {
        cur_node = llist->head;
        while(cur_node) {
            do_func(cur_node->data);
            cur_node = cur_node->next;
        }
    }
    return;
}

/*!
  \file llist.c
  \brief Linked List source file.
  \details Source file for linked list, contains all functions and declarations, static and otherwise.
  \note Traditional zero-style indexing is used for this linked list implementation. The first index is '0', second '1', etc.
  \warning It is assumed that all data inserted into the list is heap allocated.
  Failure durring deallocation will occur if that is not the case.
  \author Timothy Bullard
  \version 1.0
*/

/*!
  \var typedef struct list_node node
  \brief Typedef of struct list_node to 'node'.
*/

/*!
  \struct list_node
  \brief Fundamental node structure for linked list.
  \var list_node::prev
  Member 'prev' represents a pointer to the node that is previous, or behind, a given node.
  \var list_node::next
  Member 'next' represents a pointer to the node that is next, or in front of, a given node.
  \var list_node::data
  Member 'data' represents the allocated data to be stored in the node.
*/

/*!
  \static
  \fn node* list_node_create(void* data)
  \brief Creates note for given data.
  \param data Allocated data to be wrapped in a lnode.
  \return Newly allocated list node containing data.
*/

