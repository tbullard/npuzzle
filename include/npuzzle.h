#ifndef NPUZZLE_H
#define NPUZZLE_H

#include "search.h"

typedef struct n_sliding_puzzle {
    struct n_sliding_puzzle* parent_state;
    array_list* board_state;
    int board_dim;
    int hole_tile;
    int f_cost;
} npuzzle;

npuzzle* npuzzle_make_game(int board_dim);
npuzzle* npuzzle_copy_game(npuzzle* board);

list* npuzzle_backtrace(npuzzle* board);
list* npuzzle_gen_successors(npuzzle* board, void* null);

int* tile_copy(int* tile);

int npuzzle_is_goal(npuzzle* board, void* null);
int npuzzle_manhattan_dist(npuzzle* board, void* null);

int npuzzle_trans_cost(npuzzle* src_board, npuzzle* dest_board, void* null);
int npuzzle_compare(npuzzle* a_board, npuzzle* b_board);
int npuzzle_heap_comp(npuzzle* a_board, npuzzle* b_board);

unsigned int npuzzle_hash_FV1a(npuzzle* board);

void npuzzle_link_parent(npuzzle* child_board, npuzzle* parent_board);
void npuzzle_set_f_cost(npuzzle* board, int f_cost);
void npuzzle_kill(npuzzle* board);

#endif
