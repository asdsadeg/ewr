#include <stdlib.h>
#include <assert.h>
#include "game.h"

#include <stdio.h>
#include <string.h>


#include "board.h"

#include "ui.h"
#include "view.h"
#include "str_utils.h"

#define HOF_FILENAME "score.txt"

/**
 * Change players score.
 * Adds value of opened tile to the score.
 * If game is solved/failed then multiples/divide the score by 2.
 */

void update_player_score(Game *game, int input_row, int input_column) {

    if (game->game_state == FAILED) {
        game->player->score /= 2;
    } else {
        game->player->score +=
                game->board->tiles[input_row][input_column]->value;
    }
    if (game->game_state == SOLVED) {
        game->player->score *= 2;
    }
}


/**
 * Open neighbour Tile if value of the clue is 0.
 * Checks main and mixed directions of the opened Tile.
 */
void open_neighbour_tiles(Game *game, int row, int column) {
    assert(game != NULL);
    // iterates every possible direction of opened Tile
    for (int d_row = -1; d_row <= 1; d_row++) {
        for (int d_column = -1; d_column <= 1; d_column++) {
            int a_row = row + d_row;
            int a_column = column + d_column;
            if (a_row >= 0 && a_row < game->board->row_count && a_column >= 0
                && a_column < game->board->column_count) {
                open_tile(game, a_row, a_column);
            }
        }
    }
}


/**
 * Create and allocate pointers of a Board, Player and Game.
 * Sets Game state to PLAYING.
 * @return pointer to the Game
 */
Game *create_game() {
    Game *game = (Game *) calloc(1, sizeof(Game));
    Player *player = (Player *) calloc(1, sizeof(Player));
    game->player = player;
    game->player->score = 1;
    game->game_state = PLAYING;
    return game;
}


/**
 * Open selected Tile(s) according to input coordinates.
 * Checks if input coordinates are in the correct range if Game state is playing.
 * Can change Game state to solved or failed.
 */
void open_tile(Game *game, int input_row, int input_column) {

    if (game->game_state != PLAYING
        || !is_input_data_correct(game->board, input_row, input_column)) {
        return;
    }

    if (game->board->tiles[input_row][input_column]->tile_state == CLOSED) {
        game->board->tiles[input_row][input_column]->tile_state = OPEN;

        if (game->board->tiles[input_row][input_column]->is_mine) {
            game->game_state = FAILED;
            open_all_mines(game->board);
        }

        if (!game->board->tiles[input_row][input_column]->is_mine
            && game->board->tiles[input_row][input_column]->value == 0) {
            open_neighbour_tiles(game, input_row, input_column);
        }
        if (is_game_solved(game->board)) {
            game->game_state = SOLVED;
        }
        update_player_score(game, input_row, input_column);
    }
}


/**
 * Check if the title is mine or not. Return bool
 */
bool check_title_if_mine(Game *game, int input_row, int input_column) {

    if (game->game_state != PLAYING
        || !is_input_data_correct(game->board, input_row, input_column)) {
        return false;
    }

    if (game->board->tiles[input_row][input_column]->tile_state == CLOSED) {
        if (game->board->tiles[input_row][input_column]->is_mine) {
            return true;
        }
    }
    return false;
}

/**
 * Free pointers of the Player and Game.
 */
void destroy_game(Game *game) {
    assert(game != NULL);
    destroy_board(game->board);
    free(game->player);
    free(game);
}


int dialog(){
        char word[5];
        printf("want to play again? (Enter 'yes' to play again or 'no') ");
        scanf("%s", word);
        if(strcmp(word, "y") == 0 || strcmp(word, "ye") == 0 || strcmp(word, "yes") == 0){
                Game *game = create_game();
                Board *board = create_our_board();
                game->board = board;
                read_player_name(game);
                play_game(game);
                destroy_game(game);
                dialog();
             //   exit(EXIT_SUCCESS);
        }
        else if(strcmp(word, "n") == 0 || strcmp(word, "no") == 0){
                printf("Ok, goodbye!\n");
                return 0;
        }else{
              //  printf("Please, choose yes or no \n");
                  printf("Please try again ");
                 // dialog();
        }
        return -1;
}

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

void read_player_name(Game *game) {
    assert(game != NULL);
    printf("Zadajte vaše meno prosím: \n");
    char name_of_player[MAX_PLAYER_LENGTH];
    fgets(name_of_player, MAX_PLAYER_LENGTH, stdin);
    str_remove_spaces(name_of_player);
    strcpy(game->player->name, name_of_player);
}

void play_game(Game *game) {
    assert(game != NULL);

    Player list_of_players[PLAYERS_LIMIT];
    int size_of_list = read_scores(HOF_FILENAME, list_of_players);
    if (size_of_list > 0) {
        print_score(list_of_players, size_of_list);
    }
    game_loop(game);

    if (game->game_state == SOLVED) {
        printf("Gratulujem %s. Riešenie je správne!\n", game->player->name);
    } else {
        printf("Ľutujem %s. Riešenie je nesprávne!\n", game->player->name);
    }
    printf("Vaše skóre je: %d\n", game->player->score);

    bool is_player_added_to_list =
            add_player_to_list(list_of_players, &size_of_list, *game->player);

    if (is_player_added_to_list) {
        save_scores(HOF_FILENAME, list_of_players, size_of_list);
    }
}

void game_loop(Game *game) {
    assert(game != NULL);
    int input, input_row = -1, input_column = -1;
    do {
        print_play_field(game, input_row, input_column);
        print_input_rules();

        while ((input = scanf("%d %d", &input_row, &input_column)) == 0) {
            scanf("%*[^\n] %*[^\n]");
            print_input_rules();
        }
        if (input != EOF) {
            // if input is not empty and is correct then open the tile
            open_tile(game, input_row - 1, input_column - 1);
        }

    } while (game->game_state == PLAYING);
    print_play_field(game, input_row, input_column);
}

void print_score(Player *list_of_players, int number_of_all_players) {
    char *text = view_hof(list_of_players, number_of_all_players);
    printf("%s", text);
    free(text);
}


void print_play_field(Game *game, int input_row, int input_column) {
    char *field = view_play_field(game->board, input_row, input_column);
    printf("\n%s\n", field);
    free(field);
}


void print_input_rules() {
    printf("Zadajte číslo riadka, medzeru a číslo stĺpca. Napr. 2 3, 5 9\n");
}