#include "hof.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


/**
 * Load hall of fame from the file
 * @return the number of loaded entries or 0, if there was error in loading
 */

int read_scores(char *filename, Player *list_of_players)
{
    assert(filename != NULL);
    assert(list_of_players != NULL);
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return 0;
    }
    int index_of_player = 0;
    while (fscanf(file, "%s %d", list_of_players[index_of_player].name, &list_of_players[index_of_player].score) != EOF && index_of_player < PLAYERS_LIMIT)
    {

        index_of_player++;
    }

    fclose(file);

    return index_of_player;
}

/**
 * Save the hall of fame array to the file
 */

void save_scores(char *filename, Player *list_of_players, const int size)
{
    assert(filename != NULL);

    assert(list_of_players != NULL);

    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        return;
    }

    int players_to_save = size < PLAYERS_LIMIT ? size : PLAYERS_LIMIT;

    for (int i = 0; i < players_to_save; i++)
    {
        fprintf(file, "%s %d\n", list_of_players[i].name, list_of_players[i].score);
    }

    fclose(file);
}

/**
 * Add Player to the hall of fame array
 * @return true, if Player (entry) was added to the list, false otherwise
 */

bool add_player_to_list(Player list_of_players[], int *size_of_list, const Player player)
{
    assert(list_of_players != NULL && size_of_list != NULL);

    int replace_pos = -1;
    int insert_pos = 0;

    // Searching for a position to insert a new player and a possible replacement player
    while (insert_pos < *size_of_list)
    {
        if (list_of_players[insert_pos].score < player.score)
        {
            break;
        }
        if (list_of_players[insert_pos].score == player.score)
        {
            replace_pos = insert_pos;
        }
        insert_pos++;
    }

    if (replace_pos != -1)
    {
        list_of_players[replace_pos] = player;
        // If we find a player with the same score, we simply replace him
        return true;
    }

    if (*size_of_list < PLAYERS_LIMIT || insert_pos == PLAYERS_LIMIT)
    {
        memmove(&list_of_players[insert_pos + 1], &list_of_players[insert_pos], (*size_of_list - insert_pos) * sizeof(Player));
        *size_of_list = *size_of_list + 1;
        // If the list is not complete, add the player to the found position
    }
    else
    {
        return false;
        // There is no room for a new player and his score is no better than the lowest on the list
    }

    list_of_players[insert_pos] = player;

    return true;
}
