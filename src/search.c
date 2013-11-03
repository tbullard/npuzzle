#include "search.h"

list* search_breadth_first(void* state,
                           void* state_world,
                           search_is_goal state_goal_func,
                           search_gen_successors state_gen_func,
                           search_link_parent state_link_func,
                           search_goal_backtrace state_back_func,
                           hash_func state_hash_alg,
                           generic_comp state_comp_func,
                           generic_cpy state_copy_func,
                           generic_op state_free_func) {
    int found;
    void* current_state, *successor_state;
    list* state_queue, *successor_list, *path;
    hash_table* state_closed_set;
    
    state_queue = list_create(NULL,
                              NULL,
                              state_free_func);
    
    state_closed_set = hash_table_create(89,
                                         .75,
                                         state_hash_alg,
                                         state_comp_func,
                                         state_copy_func,
                                         state_free_func);
    current_state = state;
    list_push_front(state_queue, current_state);
    hash_table_insert(state_closed_set, current_state, 0);
    path = NULL;
    found = 0;
    while(!list_is_empty(state_queue) && !found) {
        current_state = list_back(state_queue);
        list_deque(state_queue);
        if(state_goal_func(current_state, state_world)) {
            current_state = state_copy_func(current_state);
            path = state_back_func(current_state);
            found = 1;
        } else {
            successor_list = state_gen_func(current_state, state_world);
            while(!list_is_empty(successor_list)) {
                successor_state = list_front(successor_list);
                if(!hash_table_contains(state_closed_set, successor_state)) {
                    state_link_func(successor_state, current_state);
                    hash_table_insert(state_closed_set, successor_state, 0);
                    list_push_front(state_queue, successor_state);
                    list_pop(successor_list);
                } else {
                    list_remove_front(successor_list);
                }
            }
            list_kill(successor_list);
        }
    }
    hash_table_kill(state_closed_set);
    list_dissolve(state_queue);
    return path;
}

list* search_a_star(void* state, void* state_world,
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
                    heap_comp state_heap_func) {
    int* g_cost_ptr, *f_cost_ptr, f_cost, tmp_f, g_cost, found;
    void* current_state, *successor_state, *heap_memory_location;
    list* states_overflow, *successor_list, *path;
    hash_table* states_closed_set, *states_open_set;
    hash_map* states_g_cost, *states_f_cost, *states_heap_index;
    heap* states_heap;
    
    states_overflow = list_create(NULL,
                                  NULL,
                                  state_free_func);
    
    states_closed_set = hash_table_create(89,
                                          .75,
                                          state_hash_alg,
                                          state_comp_func,
                                          state_copy_func,
                                          state_free_func);
    
    states_open_set = hash_table_create(89,
                                        .75,
                                        state_hash_alg,
                                        state_comp_func,
                                        state_copy_func,
                                        state_free_func);
    
    states_g_cost = hash_map_create(89,
                                    .75,
                                    state_hash_alg,
                                    state_comp_func,
                                    NULL,
                                    NULL,
                                    NULL,
                                    state_free_func,
                                    (generic_op)free);
    
    states_f_cost = hash_map_create(89,
                                    .75,
                                    state_hash_alg,
                                    state_comp_func,
                                    NULL,
                                    NULL,
                                    NULL,
                                    state_free_func,
                                    (generic_op)free);
    
    states_heap_index = hash_map_create(89,
                                        .75,
                                        state_hash_alg,
                                        state_comp_func,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL);
    
    states_heap = heap_create(89,
                              state_heap_func,
                              state_comp_func,
                              state_copy_func,
                              state_free_func);
    current_state = state;
    f_cost = state_heur_func(current_state, NULL);
    state_f_cost_set_func(current_state, f_cost);
    g_cost = 0;
    g_cost_ptr = malloc(sizeof(int));
    *g_cost_ptr = g_cost;
    f_cost_ptr = malloc(sizeof(int));
    *f_cost_ptr = f_cost;
    hash_map_insert(states_g_cost, current_state, g_cost_ptr, 0);
    heap_memory_location = heap_add(states_heap, state_copy_func(current_state));
    hash_table_insert(states_open_set, state_copy_func(current_state), 0);
    hash_map_insert(states_f_cost, state_copy_func(current_state), f_cost_ptr, 0);
    hash_map_insert(states_heap_index, current_state, heap_memory_location, 1);
    path = NULL;
    found = 0;
    while(!heap_is_empty(states_heap) && !found) {
        current_state = state_copy_func(heap_peek(states_heap));
        heap_remove(states_heap);
        hash_table_remove(states_open_set, current_state);
        hash_map_remove(states_heap_index, current_state);
        if(state_goal_func(current_state, state_world)) {
            path = state_back_func(current_state);
            found = 1;
        } else {
            if(!hash_table_insert(states_closed_set, current_state, 0)) {
                list_push_front(states_overflow, current_state);
            }
            successor_list = state_gen_func(current_state, state_world);
            while(!list_is_empty(successor_list)) {
                successor_state = list_front(successor_list);
                g_cost = *(int*)hash_map_get(states_g_cost, current_state) +
                    state_trans_func(current_state, successor_state, state_world);
                f_cost = g_cost + state_heur_func(successor_state, state_world);
                tmp_f = hash_map_contains_key(states_f_cost, successor_state) ?
                    *(int*)hash_map_get(states_f_cost, successor_state) : UINT_MAX;
                if(hash_table_contains(states_closed_set, successor_state) && f_cost > tmp_f) {
                    list_remove_front(successor_list);
                    continue;
                }
                if(!hash_table_contains(states_open_set, successor_state) || f_cost < tmp_f) {
                    state_f_cost_set_func(successor_state, f_cost);
                    state_link_func(successor_state, current_state);
                    g_cost_ptr = malloc(sizeof(int));
                    f_cost_ptr = malloc(sizeof(int));
                    *g_cost_ptr = g_cost;
                    *f_cost_ptr = f_cost;
                    if(!hash_table_contains(states_open_set, successor_state)) {
                        hash_table_insert(states_open_set, successor_state, 0);
                        heap_memory_location = heap_add(states_heap, state_copy_func(successor_state));
                        hash_map_insert(states_heap_index, successor_state,  heap_memory_location, 1);
                    } else {
                        heap_memory_location = hash_map_get(states_heap_index, successor_state);
                        heap_up_mod_data(states_heap, heap_memory_location,  successor_state);
                    }
                    if(!hash_map_set(states_g_cost, successor_state, g_cost_ptr)) {
                        hash_map_insert(states_g_cost, state_copy_func(successor_state), g_cost_ptr, 0);
                    }
                    if(!hash_map_set(states_f_cost, successor_state, f_cost_ptr)) {
                        hash_map_insert(states_f_cost, state_copy_func(successor_state), f_cost_ptr, 0);
                    }
                    list_pop(successor_list);
                } else {
                    list_remove_front(successor_list);
                }
            }
            list_kill(successor_list);
        }
    }
    heap_kill(states_heap);
    list_kill(states_overflow);
    hash_map_kill(states_g_cost);
    hash_map_kill(states_f_cost);
    hash_table_kill(states_open_set);
    hash_table_kill(states_closed_set);
    hash_map_dissolve(states_heap_index);
    return path;
}
