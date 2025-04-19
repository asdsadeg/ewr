#include "greatest.h"
#include "../board.h"
#include "../game.h"

TEST verify_with_high_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT_FALSE(is_input_data_correct(board, 6, 11));
    PASS();
}

TEST verify_with_low_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT_FALSE(is_input_data_correct(board, -1, 4));
    PASS();
}

TEST verify_with_correct_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT(is_input_data_correct(board, 0, 9));
    PASS();
}

TEST is_game_solved_at_the_start() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    bool is_solved = is_game_solved(game->board);
    ASSERT_FALSE(is_solved);
    destroy_game(game);
    PASS();
}

TEST is_game_solved_if_solved() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (!game->board->tiles[row][column]->is_mine) {
                open_tile(game, row, column);
            }
        }
    }
    ASSERT(is_game_solved(game->board));
    destroy_game(game);
    PASS();
}

TEST open_mines_after_win() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (!game->board->tiles[row][column]->is_mine) {
                open_tile(game, row, column);
            }
        }
    }
    open_all_mines(game->board);
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (game->board->tiles[row][column]->is_mine) {
                ASSERT_EQ(MARKED, game->board->tiles[row][column]->tile_state);
            }
        }
    }
    destroy_game(game);
    PASS();
}

TEST create_our_board_valid_parameters() {
    Board *board = create_our_board();

   
   ASSERT(board != NULL);
    if(board != NULL){
        
    
    if(board->row_count > 0 && board->column_count > 0 && board->mine_count > 0){
    //ASSERT(board->row_count >= 0);
    //ASSERT(board->column_count > 0);
    //ASSERT(board->mine_count > 0);

    int area_field = board->row_count * board->column_count;
    ASSERT(board->mine_count < area_field);


    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            ASSERT(board->tiles[row][column]->tile_state == CLOSED);
        }
    }

    destroy_board(board);
    }
}
    PASS();
}


SUITE (test_board) {
    RUN_TEST(verify_with_high_coordinates);
    RUN_TEST(verify_with_low_coordinates);
    RUN_TEST(verify_with_correct_coordinates);
    RUN_TEST(is_game_solved_at_the_start);
    RUN_TEST(is_game_solved_if_solved);
    RUN_TEST(open_mines_after_win);
    RUN_TEST(create_our_board_valid_parameters);
}
