#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

/**
 * Check if mine is on the current Tile.
 * @return true if Tile has mine on or false otherwise
 */
bool is_mine_on(Board *board, int row, int column) {
    assert(board != NULL);
    return row >= 0 && row < board->row_count && column >= 0
           && column < board->column_count
           && board->tiles[row][column]->is_mine;
}


/**
 * Count number of mines interacted with Tile.
 * @return count of mines
 */
int count_neighbour_mines(Board *board, int row, int column) {
    assert(board != NULL);

    // iterates with interacting tiles and counts clue value
    int count = 0;
    for (int drow = -1; drow <= 1; drow++) {
        for (int dcolumn = -1; dcolumn <= 1; dcolumn++) {
            if (is_mine_on(board, row + drow, column + dcolumn)) {
                count++;
            }
        }
    }
    return count;
}


/**
 * Set values to tiles according to neighbour mines count.
 * If Tile is a mine then value is set to -1.
 */
void set_tile_values(Board *board) {
    assert(board != NULL);

    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {

            if (board->tiles[row][column]->is_mine) {
                board->tiles[row][column]->value = -1;
            } else {
                board->tiles[row][column]->value =
                        count_neighbour_mines(board, row, column);
            }
        }
    }
}


/**
 * When Game is won all mines are shown as marked.
 */
void mark_all_mines(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED) {
                board->tiles[row][column]->tile_state = MARKED;
            }
        }
    }
}


/**
 * Generate random value from 0 to upper range of the border.
 * @return random number in range 0 to upper_range param
 */
int generate_random_coordinates(int upper_range) {
    return rand() % upper_range;
}


/**
 * Generate random coordinates to row and column according to mine count value.
 * Randomly sets mines to the Board pointer
 */
void set_mines_randomly(Board *board) {
    assert(board != NULL);

    int board_mine_count = 0;
    srand(time(NULL));
    while (board_mine_count != board->mine_count) {
        int random_row = generate_random_coordinates(board->row_count);
        int random_column = generate_random_coordinates(board->column_count);

        if (board->tiles[random_row][random_column]->is_mine == false) {

            board->tiles[random_row][random_column]->is_mine = true;
            board_mine_count++;
        }
    }
}

bool read_board_parameters(int* row_count, int* col_count, int* mine_count)
{
    printf("Write pls row count: ");
    scanf("%d", row_count);
    //   assert((*row_count) > 0);
    if((*row_count)<0)
    {
    perror("row count less that zero");
    return false;
    }


    printf("Write pls col count: ");
    scanf("%d", col_count);
    //  assert((*col_count) > 0);
    if((*col_count)<0)
    {
    perror("col count less that zero");
    return false;
    }

    printf("Write pls mine count: ");
    scanf("%d", mine_count);
    //   assert((*mine_count) > 0);
    if((*mine_count)<0)
    {
    perror("mine count less that zero");
    return false;
    }

    // int area_field = (*col_count) * (*row_count);
    // assert((*mine_count) < area_field);
    
    return true;
}


Board *create_our_board() {
    Board *board = (Board *) calloc(1, sizeof(Board));
    board->row_count = 0;
    board->column_count = 0;
    board->mine_count = 0;
    if(read_board_parameters(&(board->row_count), &(board->column_count), &(board->mine_count)) == false)
    {
        free(board);
        return NULL;
        //return board;
    }

    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            board->tiles[row][column] = (Tile *) calloc(1, sizeof(Tile));
            board->tiles[row][column]->tile_state = CLOSED;
            board->tiles[row][column]->is_mine = false;
        }
    }
    set_mines_randomly(board);
    set_tile_values(board);
    return board;
}

/**
 * Create and allocate pointer of the Board.
 * @return pointer of the Board
 */
Board *create_board(int row_count, int column_count, int mine_count) {
    Board *board = (Board *) calloc(1, sizeof(Board));
    board->row_count = row_count;
    board->column_count = column_count;
    board->mine_count = mine_count;

    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            board->tiles[row][column] = (Tile *) calloc(1, sizeof(Tile));
            board->tiles[row][column]->tile_state = CLOSED;
            board->tiles[row][column]->is_mine = false;
        }
    }
    set_mines_randomly(board);
    set_tile_values(board);
    return board;
}


/**
 * Free the Board and each Tile.
 */
void destroy_board(Board *board) {
    assert(board != NULL);

    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->row_count; column++) {
            free(board->tiles[row][column]);
        }
    }
    free(board);
}


/**
 * Check if Game is solved.
 * @return false if Board consists of any Tile which is closed and has clue value, else true
 */
bool is_game_solved(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED
                && !board->tiles[row][column]->is_mine) {
                return false;
            }
        }
    }
    mark_all_mines(board);
    return true;
}


/**
 * Check if input row and column are within correct range.
 * @return true if input coordinates are within the range, false otherwise
 */
bool is_input_data_correct(Board *board, int input_row, int input_column) {
    assert(board != NULL);
    return input_row >= 0 && board->row_count > input_row
           && input_column >= 0 && board->column_count > input_column;
}


/**
 * If Game is lost all mines are shown.
 */
void open_all_mines(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED
                && board->tiles[row][column]->is_mine) {
                board->tiles[row][column]->tile_state = OPEN;
            }
        }
    }
}
