#ifndef SEARCH_H
#define SEARCH_H

#include "heap.h"
#include "hashm.h"
#include "llist.h"
#include "arraylist.h"

typedef int (*search_trans_cost)(const void*, const void*, const void*);
typedef int (*search_is_goal)(const void*, const void*);
typedef int (*search_heuristic)(const void*, const void*);
typedef list* (*search_gen_successors)(const void*, const void*);
typedef list* (*search_goal_backtrace)(const void*);
typedef void (*search_link_parent)(void*, const void*);
typedef void (*search_set_f_cost)(void*, int);

list* search_breadth_first(void* state,
                           void* state_world,
                           search_is_goal state_goal_func,
                           search_gen_successors state_gen_func,
                           search_link_parent state_link_func,
                           search_goal_backtrace state_back_func,
                           hash_func state_hash_alg,
                           generic_comp state_comp_func,
                           generic_cpy state_copy_func,
                           generic_op state_free_func);

list* search_a_star(void* state,
                    void* state_world,
                    search_is_goal state_goal_func,
                    search_gen_successors state_gen_func,
                    search_link_parent state_link_func,
                    search_goal_backtrace state_back_func,
                    search_trans_cost state_trans_func,
                    search_heuristic state_heur_func,
                    search_set_f_cost state_f_cost_set_func,
                    hash_func state_hash_alg,
                    generic_comp state_comp_func,
                    generic_cpy state_copy_func,
                    generic_op state_free_func,
                    heap_comp state_heap_func);

#endif
