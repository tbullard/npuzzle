#include "npuzzle.h"

static void first_row_tile_swap(npuzzle* board, int form);
static void first_col_tile_swap(npuzzle* board, int form);
static void first_row_first_col_tile_swap(npuzzle* board, int form);
static void first_row_last_col_tile_swap(npuzzle* board, int form);
static void last_row_tile_swap(npuzzle* board, int form);
static void last_col_tile_swap(npuzzle* board, int form);
static void last_row_first_col_tile_swap(npuzzle* board, int form);
static void last_row_last_col_tile_swap(npuzzle* board, int form);
static void neutral_tile_swap(npuzzle* board, int form);

static void first_row_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + 1);
        board->hole_tile++;
    } else if(form == 1) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - 1);
        --board->hole_tile;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + board->board_dim);
        board->hole_tile += board->board_dim;
    }
    return;
}

static void first_col_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + 1);
        board->hole_tile++;
    } else if(form == 1) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - board->board_dim);
        board->hole_tile -= board->board_dim;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + board->board_dim);
        board->hole_tile += board->board_dim;
    }
    return;
}

static void first_row_first_col_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + 1);
        board->hole_tile++;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + board->board_dim);
        board->hole_tile += board->board_dim;
    }
    return;
}

static void first_row_last_col_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - 1);
        --board->hole_tile;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + board->board_dim);
        board->hole_tile += board->board_dim;
    }
    return;
}

static void last_row_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + 1);
        board->hole_tile++;
    } else if(form == 1) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - 1);
        --board->hole_tile;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - board->board_dim);
        board->hole_tile -= board->board_dim;
    }
    return;
}

static void last_col_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - 1);
        --board->hole_tile;
    } else if(form == 1) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + board->board_dim);
        board->hole_tile += board->board_dim;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - board->board_dim);
        board->hole_tile -= board->board_dim;
    }
    return;
}

static void last_row_first_col_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + 1);
        board->hole_tile++;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - board->board_dim);
        board->hole_tile -= board->board_dim;
    }
    return;
}

static void last_row_last_col_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - 1);
        --board->hole_tile;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - board->board_dim);
        board->hole_tile -= board->board_dim;
    }
    return;
}

static void neutral_tile_swap(npuzzle* board, int form) {
    if(!form) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + 1);
        board->hole_tile++;
    } else if(form == 1) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - 1);
        --board->hole_tile;
    } else if(form == 2) {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile + board->board_dim);
        board->hole_tile += board->board_dim;
    } else {
        array_list_swap(board->board_state, board->hole_tile, board->hole_tile - board->board_dim);
        board->hole_tile -= board->board_dim;
    }
}

npuzzle* npuzzle_make_game(int board_dim) {
    int i, *tile;
    npuzzle* new_board;
    new_board = malloc(sizeof(npuzzle));
    new_board->f_cost = 0;
    new_board->parent_state = NULL;
    new_board->board_dim = board_dim;
    new_board->hole_tile = (board_dim * board_dim) - 1;
    new_board->board_state = array_list_create(NULL,
                                               (generic_cpy)tile_copy,
                                               (generic_op)free);
    for(i = 0; i < new_board->hole_tile + 1; i++) {
        tile = malloc(sizeof(int));
        *tile = i;
        array_list_push(new_board->board_state, tile);
    }
    for(i = 0; i < 100; i++) {
        if(new_board->hole_tile - board_dim < 0 || !(new_board->hole_tile % board_dim)) { /* FR or FC */
            if(new_board->hole_tile % board_dim) { /* FR, !LR, !FC */
                if((new_board->hole_tile + 1) % board_dim) { /* FR, !LR, !FC, !LC */
                    first_row_tile_swap(new_board, (rand() % 3));
                } else { /* FRLC */
                    first_row_last_col_tile_swap(new_board, (rand() % 2));
                }
            } else if (new_board->hole_tile - board_dim >= 0){ /* !FR, FC, !LC */
                if(new_board->hole_tile - ((board_dim - 1) * board_dim) < 0) { /* !FR, !LR, FC, !LC */
                    first_col_tile_swap(new_board, (rand() % 3));
                } else { /* LRFC */
                    last_row_first_col_tile_swap(new_board, (rand() % 2));
                }
            } else { /* FRFC */
                first_row_first_col_tile_swap(new_board, (rand() % 2));
            }
        } else if(new_board->hole_tile - ((board_dim - 1) * board_dim) >= 0 ||
                  !((new_board->hole_tile + 1) % board_dim)) { /* LR or LC, !FR, !FC */
            if((new_board->hole_tile + 1) % board_dim) { /* !FR, LR, !FC, !LC */
                last_row_tile_swap(new_board, (rand() % 3));
            } else if (new_board->hole_tile - ((board_dim - 1) * board_dim) < 0) { /* !LR, !FR, !FC, LC */
                last_col_tile_swap(new_board, (rand() % 3));
            } else { /* LRLC */
                last_row_last_col_tile_swap(new_board, (rand() % 2));
            }
        } else { /* Neutral */
            neutral_tile_swap(new_board, (rand() % 4));
        }
    }
    return new_board;
}

npuzzle* npuzzle_copy_game(npuzzle* board) {
    npuzzle* new_board;
    new_board = malloc(sizeof(npuzzle));
    new_board->f_cost = board->f_cost;
    new_board->parent_state = board->parent_state;
    new_board->board_dim = board->board_dim;
    new_board->hole_tile = board->hole_tile;
    new_board->board_state = array_list_copy(board->board_state);
    return new_board;
}

list* npuzzle_backtrace(npuzzle* board) {
    list* back_path;
    back_path = list_create(NULL,
                            NULL,
                            (generic_op)npuzzle_kill);
    list_push_front(back_path, board);
    while(board->parent_state) {
        board = board->parent_state;
        list_push_front(back_path, npuzzle_copy_game(board));
    }
    return back_path;
}

list* npuzzle_gen_successors(npuzzle* board, void* null) {
    int i;
    list* successor_list;
    npuzzle* successor_board;
    successor_list = list_create(NULL,
                                 (generic_cpy)npuzzle_copy_game,
                                 (generic_op)npuzzle_kill);
    if(board->hole_tile - board->board_dim < 0 || !(board->hole_tile % board->board_dim)) { /* FR or FC */
        if(board->hole_tile % board->board_dim) { /* FR, !LR, !FC */
            if((board->hole_tile + 1) % board->board_dim) { /* FR, !LR, !FC, !LC */
                for(i = 0; i < 3; i++) {
                    successor_board = npuzzle_copy_game(board);
                    successor_board->f_cost = 0;
                    first_row_tile_swap(successor_board, i);
                    list_push_back(successor_list, successor_board);
                }
            } else { /* FRLC */
                for(i = 0; i < 2; i++) {
                    successor_board = npuzzle_copy_game(board);
                    successor_board->f_cost = 0;
                    first_row_last_col_tile_swap(successor_board, i);
                    list_push_back(successor_list,successor_board);
                }
            }
        } else if (board->hole_tile - board->board_dim >= 0){ /* !FR, FC, !LC */
            if(board->hole_tile - ((board->board_dim - 1) * board->board_dim) < 0) { /* !FR, !LR, FC, !LC */
                for(i = 0; i < 3; i++) {
                    successor_board = npuzzle_copy_game(board);
                    successor_board->f_cost = 0;
                    first_col_tile_swap(successor_board, i);
                    list_push_back(successor_list, successor_board);
                }
            } else { /* LRFC */
                for(i = 0; i < 2; i++) {
                    successor_board = npuzzle_copy_game(board);
                    successor_board->f_cost = 0;
                    last_row_first_col_tile_swap(successor_board, i);
                    list_push_back(successor_list, successor_board);
                }
            }
        } else { /* FRFC */
            for(i = 0; i < 2; i++) {
                successor_board = npuzzle_copy_game(board);
                successor_board->f_cost = 0;
                first_row_first_col_tile_swap(successor_board, i);
                list_push_back(successor_list, successor_board);
            }
        }
    } else if(board->hole_tile - ((board->board_dim - 1) * board->board_dim) >= 0 ||
              !((board->hole_tile + 1) % board->board_dim)) { /* LR or LC, !FR, !FC */
        if((board->hole_tile + 1) % board->board_dim) { /* !FR, LR, !FC, !LC */
            for(i = 0; i < 3; i++) {
                successor_board = npuzzle_copy_game(board);
                successor_board->f_cost = 0;
                last_row_tile_swap(successor_board, i);
                list_push_back(successor_list, successor_board);
            }
        } else if (board->hole_tile - ((board->board_dim - 1) * board->board_dim) < 0) { /* !LR, !FR, !FC, LC */
            for(i = 0; i < 3; i++) {
                successor_board = npuzzle_copy_game(board);
                successor_board->f_cost = 0;
                last_col_tile_swap(successor_board, i);
                list_push_back(successor_list, successor_board);
            }
        } else { /* LRLC */
            for(i = 0; i < 2; i++) {
                successor_board = npuzzle_copy_game(board);
                successor_board->f_cost = 0;
                last_row_last_col_tile_swap(successor_board, i);
                list_push_back(successor_list, successor_board);
            }
        }
    } else { /* Neutral */
        for(i = 0; i < 4; i++) {
            successor_board = npuzzle_copy_game(board);
            successor_board->f_cost = 0;
            neutral_tile_swap(successor_board, i);
            list_push_back(successor_list, successor_board);
        }
    }
    return successor_list;
}

int* tile_copy(int* tile) {
    int* new_tile;
    new_tile = malloc(sizeof(int));
    *new_tile = *(int*)tile;
    return new_tile;
}

int npuzzle_manhattan_dist(npuzzle* board, void* null) {
    int i, x1, x2, y1, y2, cost;
    cost = 0;
    for(i = 0; i < array_list_length(board->board_state); i++) {
        x1 = i % board->board_dim;
        y1 = i / board->board_dim;
        x2 = *(int*)array_list_get(board->board_state, i) % board->board_dim;
        y2 = *(int*)array_list_get(board->board_state, i) / board->board_dim;
        cost += (abs(x1 - x2) + abs(y1 - y2));
    }
    return cost;
}

int npuzzle_is_goal(npuzzle* board, void* null) {
    int i, goal;
    goal = 1;
    for(i = 1; i < array_list_length(board->board_state) && goal; i++) {
        if(*(int*)array_list_get(board->board_state, i) < *(int*)array_list_get(board->board_state, i - 1)) {
            goal = 0;
        }
    }
    return goal;
}

int npuzzle_trans_cost(npuzzle* src_board, npuzzle* dest_board, void* null) {
    return 1;
}

int npuzzle_compare(npuzzle* a_board, npuzzle* b_board) {
    int i, a_var, b_var, comp_value;
    comp_value = 0;
    for(i = 0; i < array_list_length(a_board->board_state) && !comp_value; i++) {
        a_var = *(int*)array_list_get(a_board->board_state, i);
        b_var = *(int*)array_list_get(b_board->board_state, i);
        if(a_var != b_var) {
            if(a_var > b_var) {
                comp_value = 1;
            } else {
                comp_value = -1;
            }
        }
    }
    return comp_value;
}

int npuzzle_heap_comp(npuzzle* a_board, npuzzle* b_board) {
    int comp_value;
    comp_value = 0;
    if(a_board->f_cost > b_board->f_cost) {
        comp_value = 1;
    }
    return comp_value;
}

unsigned int npuzzle_hash_FV1a(npuzzle* board) {
    int i, j, *tile_num;
    unsigned int hash;
    hash = 2166136261;
    for(i = 0; i < array_list_length(board->board_state); i++) {
        tile_num = (int*)array_list_get(board->board_state, i);
        for(j = 0; j < 4; j++) {
            hash ^= *(((char*)tile_num) + j);
            hash *= 16777619;
        }
    }
    return hash;
}

void npuzzle_link_parent(npuzzle* child_board, npuzzle* parent_board) {
    child_board->parent_state = parent_board;
    return;
}

void npuzzle_set_f_cost(npuzzle* board, int f_cost) {
    board->f_cost = f_cost;
    return;
}

void npuzzle_kill(npuzzle* board) {
    array_list_kill(board->board_state);
    free(board);
    return;
}
