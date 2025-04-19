#include <stdlib.h>
#include "game.h"
#include "ui.h"
#include "board.h"
#include <stdio.h>

#include <string.h>

int main() {
    Game *game = create_game();
    read_player_name(game);
    Board *board = create_our_board();
    if(board == NULL)
    {
        destroy_game(game);
        exit(EXIT_FAILURE);
    }
    game->board = board;
    play_game(game);
    destroy_game(game);
    dialog();
    exit(EXIT_SUCCESS);
}
