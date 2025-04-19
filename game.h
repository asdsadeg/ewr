#ifndef MINES_GAME_H
#define MINES_GAME_H
#include "hof.h"
#include "board.h"

typedef enum  {
    FAILED,
    PLAYING,
    SOLVED,
} GameState;

typedef struct {
    Board *board;          /* Struct of the play field */
    Player *player;        /* Struct of user who is playing the Game */
    GameState game_state;  /* Enum for status of the Game */
} Game;

Game *create_game();
void open_tile(Game *game, int input_row, int input_column);
bool check_title_if_mine(Game *game, int input_row, int input_column);
void destroy_game(Game *game);

void game_loop(Game *game);
void print_score(Player *list_of_players, int number_of_all_players);

void print_play_field(Game *game, int input_row, int input_column);
void print_input_rules();

int dialog();
int test_dialog(char* word);

void read_player_name(Game *game);
void play_game(Game *game);
#endif //MINES_GAME_H