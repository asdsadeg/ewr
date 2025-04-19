// tests/test_hof.c
#include "greatest.h"
#include "../hof.h"

TEST load_file_number_of_players()
{
    char hof_file[] = "tests/score/optimal_size";
    Player list_of_players[PLAYERS_LIMIT];

    ASSERT_EQ(5, read_scores(hof_file, list_of_players));
    PASS();
}

TEST load_file_over_limit()
{
    char hof_file[] = "tests/score/over_size";
    int high_size_of_list = PLAYERS_LIMIT + 5;
    Player list_of_players[high_size_of_list];

    ASSERT_EQ(PLAYERS_LIMIT, read_scores(hof_file, list_of_players));
    PASS();
}

TEST load_nonexistent_file()
{
    char hof_file[] = "tests/score/nonexistent";
    Player list_of_players[PLAYERS_LIMIT];
    ASSERT_EQ(0, read_scores(hof_file, list_of_players));
    PASS();
}

TEST compare_loaded_and_saved_players()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 99},
        {"jozo", 55},
        {"alena", 43},
    };
    int size_of_input_list = 3;
    char hof_file[] = "tests/score/for_saving";

    save_scores(hof_file, list_of_players, size_of_input_list);
    Player test_list_of_players[size_of_input_list];
    int size_of_test_list =
        read_scores(hof_file, test_list_of_players);

    ASSERT_EQ(size_of_input_list, size_of_test_list);
    for (int i = 0; i < size_of_test_list; i++)
    {
        ASSERT_EQ(list_of_players[i].score, test_list_of_players[i].score);
        ASSERT_STR_EQ(list_of_players[i].name, test_list_of_players[i].name);
    }
    remove(hof_file);
    PASS();
}

TEST save_player_over_limit()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 99},
        {"jozo", 55},
        {"alena", 43},
        {"roman", 32},
        {"lubo", 21},
        {"milan", 20},
        {"alex", 18},
        {"dano", 9},
        {"kiko", 4},
        {"lubo", 2}};
    int size = PLAYERS_LIMIT + 1;
    char hof_file[] = "tests/score/for_saving";
    Player *player = (Player *)calloc(1, sizeof(Player));
    strcpy(player->name, "lucia");
    player->score = 26;

    ASSERT_FALSE(add_player_to_list(list_of_players, &size, *player));
    free(player);
    remove(hof_file);
    PASS();
}

TEST add_player_to_optimal_list()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 99},
        {"alex", 18},
        {"lubo", 2}};
    int size = 3;
    Player *player = (Player *)calloc(1, sizeof(Player));
    strcpy(player->name, "lucia");
    player->score = 26;

    int size_before_adding = size;
    add_player_to_list(list_of_players, &size, *player);
    ASSERT_EQ(size_before_adding + 1, size);
    free(player);
    PASS();
}

TEST add_player_sorted_correct()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"peter", 99},
        {"jozo", 55},
        {"alena", 43}};
    int size = 3;
    Player *player = (Player *)calloc(1, sizeof(Player));
    strcpy(player->name, "maria");
    player->score = 60;

    add_player_to_list(list_of_players, &size, *player);
    ASSERT_EQ(size, 4);
    ASSERT_STR_EQ(list_of_players[1].name, "maria");
    free(player);
    PASS();
}

TEST add_player_replace_same_score()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"Anna", 100},
        {"Bob", 90},

        {"Charlie", 80},
        {"Diana", 70},

    };
    int size = 4;
    Player new_player = {"Eva", 80};

    bool added = add_player_to_list(list_of_players, &size, new_player);
    ASSERT(added);
    ASSERT_EQ(size, 4);

    ASSERT_STR_EQ(list_of_players[2].name, "Eva");
    PASS();
}

TEST add_player_to_non_full_list()
{
    Player list_of_players[PLAYERS_LIMIT] = {
        {"Anna", 100},
        {"Charlie", 80},

    };
    int size = 2;

    Player new_player = {"Bob", 90};

    bool added = add_player_to_list(list_of_players, &size, new_player);
    ASSERT(added);
    ASSERT_EQ(size, 3);
    ASSERT_STR_EQ(list_of_players[1].name, "Bob");
    ASSERT_EQ(list_of_players[1].score, 90);
    PASS();
}


SUITE(test_hall_of_fame)
{
    RUN_TEST(load_file_number_of_players);
    RUN_TEST(load_file_over_limit);
    RUN_TEST(load_nonexistent_file);
    RUN_TEST(compare_loaded_and_saved_players);
    RUN_TEST(save_player_over_limit);
    RUN_TEST(add_player_to_optimal_list);
    RUN_TEST(add_player_sorted_correct);
    RUN_TEST(add_player_replace_same_score);
    RUN_TEST(add_player_to_non_full_list);
}

