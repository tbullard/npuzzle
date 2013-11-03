#include "npuzzle.h"

int main(void) {
    return 0;
}

/* Testing stuff */

/* int main(void) { */
/*     int j; */
/*     j = 1; */
/*     while(j) { */
/*         int i, length1, length2; */
/*         npuzzle* puz, *puz2; */
/*         list* puz_list, *puz_list2; */
/*         srand(time(0)); */
/*         puz = npuzzle_make_game(3); */
/*         puz2 = npuzzle_copy_game(puz); */
/*         for(i = 0; i < array_list_length(puz->board_state); i++) { */
/*             printf("%d ", *(int*)array_list_get(puz->board_state, i)); */
/*         } */
/*         printf("\n"); */
/*         puz_list = search_a_star(puz, (search_is_goal)npuzzle_is_goal, (search_gen_successors)npuzzle_gen_successors, */
/*                                  (search_link_parent)npuzzle_link_parent, (search_goal_backtrace)npuzzle_backtrace, */
/*                                  (search_trans_cost)npuzzle_trans_cost, (search_heuristic)npuzzle_manhattan_dist, */
/*                                  (search_set_f_cost)npuzzle_set_f_cost, (hash_func)npuzzle_hash_FV1a, */
/*                                  (generic_comp)npuzzle_compare, (generic_cpy)npuzzle_copy_game, */
/*                                  (generic_op)npuzzle_kill, (heap_comp)npuzzle_heap_comp); */
/*         printf("%d\n", (unsigned int)list_size(puz_list)); */
/*         length1 = list_size(puz_list); */
/*         /\* while(!list_is_empty(puz_list)) { *\/ */
/*         /\*     puz = (npuzzle*)list_front(puz_list); *\/ */
/*         /\*     for(i = 0; i < array_list_length(puz->board_state); i++) { *\/ */
/*         /\*         printf("%d ", *(int*)array_list_get(puz->board_state, i)); *\/ */
/*         /\*     } *\/ */
/*         /\*     printf("\n"); *\/ */
/*         /\*     list_remove_front(puz_list); *\/ */
/*         /\* } *\/ */
/*         list_kill(puz_list); */
/*         puz_list2 = search_breadth_first(puz2, (search_is_goal)npuzzle_is_goal, (search_gen_successors)npuzzle_gen_successors, */
/*                                          (search_link_parent)npuzzle_link_parent, (search_goal_backtrace)npuzzle_backtrace, */
/*                                          (hash_func)npuzzle_hash_FV1a, */
/*                                          (generic_comp)npuzzle_compare, (generic_cpy)npuzzle_copy_game, */
/*                                          (generic_op)npuzzle_kill); */
/*         printf("%d\n", (unsigned int)list_size(puz_list2)); */
/*         length2 = list_size(puz_list2); */
/*         /\* while(!list_is_empty(puz_list2)) { *\/ */
/*         /\*     puz2 = (npuzzle*)list_front(puz_list2); *\/ */
/*         /\*     for(i = 0; i < array_list_length(puz2->board_state); i++) { *\/ */
/*         /\*         printf("%d ", *(int*)array_list_get(puz2->board_state, i)); *\/ */
/*         /\*     } *\/ */
/*         /\*     printf("\n"); *\/ */
/*         /\*     list_remove_front(puz_list2); *\/ */
/*         /\* } *\/ */
/*         list_kill(puz_list2); */
/*         if(length1 != length2) { */
/*             j = 0; */
/*         } */
/*         printf("\n"); */
/*     } */
/*     return 0; */
/* } */

/* int main(void) { */
/*     int i; */
/*     npuzzle* puz; */
/*     list* puz_list; */
/*     srand(time(0)); */
/*     puz = npuzzle_make_game(3); */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 3; *\/ */
/*     /\* shit = array_list_create(NULL, (generic_cpy)tile_copy, (generic_op)free); *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 7; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 5; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 2; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 0; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 6; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 8; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 4; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* k = malloc(sizeof(int)); *\/ */
/*     /\* *k = 1; *\/ */
/*     /\* array_list_push(shit, k); *\/ */
/*     /\* puz->board_state = shit; *\/ */
/*     /\* puz->hole_tile = 6; *\/ */
/*     for(i = 0; i < array_list_length(puz->board_state); i++) { */
/*         printf("%d ", *(int*)array_list_get(puz->board_state, i)); */
/*     } */
/*     printf("\n"); */
/*     puz_list = search_a_star(puz, NULL, (search_is_goal)npuzzle_is_goal, (search_gen_successors)npuzzle_gen_successors, */
/*                              (search_link_parent)npuzzle_link_parent, (search_goal_backtrace)npuzzle_backtrace, */
/*                              (search_trans_cost)npuzzle_trans_cost, (search_heuristic)npuzzle_manhattan_dist, */
/*                              (search_set_f_cost)npuzzle_set_f_cost, (hash_func)npuzzle_hash_FV1a, */
/*                              (generic_comp)npuzzle_compare, (generic_cpy)npuzzle_copy_game, */
/*                              (generic_op)npuzzle_kill, (heap_comp)npuzzle_heap_comp); */
/*     printf("%d\n", (unsigned int)list_size(puz_list)); */
/*     while(!list_is_empty(puz_list)) { */
/*         puz = (npuzzle*)list_front(puz_list); */
/*         for(i = 0; i < array_list_length(puz->board_state); i++) { */
/*             printf("%d ", *(int*)array_list_get(puz->board_state, i)); */
/*         } */
/*         printf("\n"); */
/*         list_remove_front(puz_list); */
/*     } */
/*     list_kill(puz_list); */
/*     return 0; */
/* } */
