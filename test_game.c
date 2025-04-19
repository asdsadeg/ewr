#include "greatest.h"
#include "../board.h"
#include "../game.h"
//#include "../ui.h"
//#include "../ui.c"
#include <string.h>
#include "../str_utils.h"

#define HOF_FILENAME "score.txt"

TEST open_mine() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if(game->board->tiles[row][column]->is_mine){
                open_tile(game, row, column);
            }
        }
    }
    ASSERT_EQ(FAILED, game->game_state);
    destroy_game(game);
    PASS();
}

TEST open_first_mine() {
    Game *game = create_game();
    int hieght = 10;
    int width = 10;
    int mines_count = 5;
    Board *board = create_board(hieght, width, mines_count);
    game->board = board;
    while (true) {
        int input_row = 1;
        int input_column = 1;
        if (check_title_if_mine(game, input_row - 1, input_column - 1)) {
            bool clean_step = false;
            while (clean_step == false) {
                destroy_board(game->board);
                game->board = create_board(hieght, width, mines_count);
                if (!check_title_if_mine(game, input_row - 1, input_column - 1)){
                    open_tile(game, input_row - 1, input_column - 1);
                    clean_step = true;
                }
            }
            break;
        } else {
            // printf("First move was passed. It does not a mine\n");
            destroy_board(game->board);
            game->board = create_board(hieght, width, mines_count);
        }
    }
    ASSERT_EQ(PLAYING, game->game_state);
    destroy_game(game);
    PASS();
}

TEST open_tile_with_bad_coordinates() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;
    open_tile(game, -1, -1);
    open_tile(game, game->board->row_count, game->board->column_count);

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
                    ASSERT_EQ(CLOSED, game->board->tiles[row][column]->tile_state);
        }
    }
    destroy_game(game);
    PASS();
}

TEST count_score_after_move() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 70);
    game->board = board;
    bool is_closed_tile_open = false;
    int score = 1;
    while (!is_closed_tile_open) {
        int row = rand() % game->board->row_count;
        int column = rand() % game->board->column_count;
        if (!game->board->tiles[row][column]->is_mine) {
            open_tile(game, row, column);
            score += game->board->tiles[row][column]->value;
            ASSERT_EQ(score, game->player->score);
            is_closed_tile_open = true;
        }
    }

    destroy_game(game);
    PASS();
}

TEST count_score_when_game_failed() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 30);
    game->board = board;
    game->player->score = 30;
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (game->board->tiles[row][column]->is_mine) {
                open_tile(game, row, column);
                ASSERT_EQ(15, game->player->score);
                break;
            }
        }
    }

    destroy_game(game);
    PASS();
}




TEST dialog_no(){
        int rez = dialog();
        ASSERT_EQm("return", -1, rez);
        
        PASS();
}
/*
TEST dialog_yes(){
        int rez = dialog();
        ASSERT_EQm("return", 0, rez);
        PASS();
}
*/



TEST dialog_yes(){
        char word[5] = "yes";
        ASSERT_EQ(test_dialog(word), 0);
        PASS();
}




TEST print_in_rules() {

    print_input_rules();

    PASS();

}




/*
int test_dialog(char* word){
        if(strcmp(word, "yes") == 0 || strncmp(word, "ye", 2) == 0 || strncmp(word, "y", 1) == 0){
                return 0; 
        }
        else if(strcmp(word, "no") == 0 || strncmp(word, "n", 1) == 0){
                return -1;
        }else{
                return -1; 
        }
}

*/


TEST print_sc() {
    Player list_of_players[2];
    strcpy(list_of_players[0].name, "Player1");
    list_of_players[0].score = 150;
    strcpy(list_of_players[1].name, "Player2");
    list_of_players[1].score = 200;
    char buf[256];
    sprintf(buf, "Player1 - 150\nPlayer2 - 200\n");
  //  play_game(Game *game);
    print_score(list_of_players, 2);
  //  char *output = play_game(Game *game);
  //  ASSERT_STR_EQ(expected_text, output);
  //  free(output);
    PASS();
}




TEST read_p_name(){
        Game game;
        Player player;
        game.player = &player;
        FILE* failik = fopen("test.txt", "w");
        fprintf(failik, "Rex Bor\n");
        fclose(failik);
        freopen("test.txt", "r", stdin);
        read_player_name(&game);
        char expected_name[MAX_PLAYER_LENGTH];
        strcpy(expected_name, "Rex Bor");
        str_remove_spaces(expected_name);
        ASSERT_STR_EQ(game.player->name, expected_name);
        remove("test.txt");
        PASS();
}
/*
TEST print_play_fi() {
    Game game;
    print_play_field(&game, 0, 0);
    char expected_output[] = "qwe";
     ASSERT_STR_EQ(expected_output, "qwe");
    ASSERT_STR_EQ("qwe", "qwe");
    PASS();
}
*/
/*
TEST p_game(){
        Game game;
        game.player = malloc(sizeof(Player));
        strcpy(game.player->name, "TestPlayer");
        game.player->score = 100;
        game.game_state = SOLVED;
        play_game(&game);
        ASSERT_EQ_FMT(SOLVED, game.game_state, "%d");
        ASSERT_EQ_FMT(100, game.player->score, "%d");
        free(game.player);
        PASS();
}
*/

TEST test_yes() {
    ASSERT_EQ(test_dialog("yes"), 0);
    PASS();
}

TEST test_no() {
    ASSERT_EQ(test_dialog("no"), -1);
    PASS();
}

TEST test_oth() {
    ASSERT_EQ(test_dialog("zxc"), -1);
    PASS();
}


TEST print_play_fi() {
    Game game;
    game.board = create_board(5, 5, 2);
    int input_row = 2;
    int input_column = 3;
    print_play_field(&game, input_row, input_column);
    free(game.board);
    PASS();
}

TEST p_game(){
    Game game;
    Player player = {"John", 100};
    game.player = &player;
    game.game_state = SOLVED;
    FILE *failik = fopen(HOF_FILENAME, "w");
    play_game(&game);
    ASSERT_EQ_FMT(game.player->score, 100, "%d");
    ASSERT(game.game_state == SOLVED);
    Player list_of_players[PLAYERS_LIMIT];
    int size_of_list = read_scores(HOF_FILENAME, list_of_players);
    ASSERT(size_of_list == 1);
    ASSERT_STR_EQ(list_of_players[0].name, "John");
    ASSERT_EQ(list_of_players[0].score, 100);
    fclose(failik);
    PASS();
}




TEST full(){
     printf("Ok, goodbye!\n");
    PASS();

}



SUITE (test_game) {
    RUN_TEST(open_mine);
    RUN_TEST(open_first_mine);
    RUN_TEST(open_tile_with_bad_coordinates);
    RUN_TEST(count_score_after_move);
    RUN_TEST(count_score_when_game_failed);
    RUN_TEST(dialog_yes);
    RUN_TEST(dialog_no);
  //  RUN_TEST(dialog_other);
    RUN_TEST(print_sc);
    RUN_TEST(print_in_rules);
    RUN_TEST(print_play_fi);
    RUN_TEST(read_p_name);
    RUN_TEST(p_game);

    RUN_TEST(test_yes);
    RUN_TEST(test_no);
    RUN_TEST(test_oth);
    RUN_TEST(full);
}