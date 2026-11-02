#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "puzzle_globals.h"

#define one_in_range(a, b, c, d) ((((c) <= (a)) && ((a) <= (d))) || (((c) <= (b)) && ((b) <= (d))) || (((a) <= (c)) && ((c) <= (b))) || (((a) <= (d)) && ((d) <= (b))))
#define one_equals_across(a, b, c, d) (((a) == (c)) || ((a) == (d)) || ((b) == (c)) || ((b) == (d)))
struct Placement_Node
{
    struct Placement_Node* next;
    struct Placement_Node* previous;
    // Array of actuall coordinates relative to first letter of first word
    // [start_x, start_y, end_x, end_y]
    int coords[4];
    // Coord array containing the extremes of the entire placement with this word last
    int edge_coords[4];
    // Index of word in the list
    int word_index;
    // amount of crosovers in placements up to here
    int crossovers;
};
struct Placement_List
{
    struct Placement_Node* head;
    struct Placement_Node* tail;
};

struct Generation_Result
{
    int crossovers;
    char** board;
};

int add_word(int next_word_i, int next_word_place, int attachment_place, struct Placement_Node* attachment ,struct Placement_List* place_list);

void remove_tail(struct Placement_List* place_list);
// Try to place a word in the list. Retunrs number of crossovers if successful and -1 otherwise.
int place_word(struct Placement_Node* new_word, struct Placement_List* place_list);
// Generate a board with maximal crossovers(single threaded)
struct Generation_Result* create_puzzle_singlethread(int word_index, struct Placement_List* place_list);
// Wrapper for create_puzzle_singlethread
struct Generation_Result* singlethread_puzzle_wrapper();
// Generate a result struct from a word placement list
struct Generation_Result* generate_result(struct Placement_List* place_list);
// prints a given board
void print_board(char** board);