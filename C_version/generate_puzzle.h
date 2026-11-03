#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "puzzle_globals.h"

#define in_range(a, b, c) ((b) <= (a)) && ((a) <= (c))
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
typedef struct Placement_Node placement_node;

struct Placement_List
{
    struct Placement_Node* head;
    struct Placement_Node* tail;
};
typedef struct Placement_List placement_list;

struct Generation_Result
{
    int crossovers;
    char** board;
};
typedef struct Generation_Result gen_result;

struct Thread_List
{
    struct Thread_List* next;
    pthread_t thread;
};
typedef struct Thread_List thread_list;

struct Result_List
{
    struct Result_List* next;
    placement_list* place;
    gen_result* res;
};
typedef struct Result_List results;

int add_word(int next_word_i, int next_word_place, int attachment_place, placement_node* attachment ,placement_list* place_list);

void remove_tail(placement_list* place_list);
// Try to place a word in the list. Retunrs number of crossovers if successful and -1 otherwise.
int place_word(placement_node* new_word, placement_list* place_list);
// Generate a board with maximal crossovers(single threaded)
gen_result* create_puzzle_singlethread(int word_index, placement_list* place_list);
// Generates puzzles using either single or multithreaded mode
gen_result* puzzle_gen_wrapper(bool mt);
// Wrapper for single threaded puzzle generator. Returns best puzzle for each word order
gen_result* puzzle_gen_wrapper_st();
// Wrapper for multi threaded puzzle generator. Returns best puzzle for each word order
gen_result* puzzle_gen_wrapper_mt();
// Generate a result struct from a word placement list
gen_result* generate_result(placement_list* place_list);
// prints a given board
void print_board(gen_result* res);
// copies a word placement list
placement_list* copy_placement_list(placement_list* placement);
// Creates threads and assigns work
gen_result* create_puzzle_multithread(int word_index, placement_list* place_list);
// Function called by each thread of the multithreaded puzzle generator
void* multi_single_wrapper(void* place_list);
// Destroys a list using a given Dtor. Dtor needs to destroy 1 element and return the next one
void list_dtor(void*(*dtor)(void*), void* list);
// Initializes a word placement list
placement_list* init_placement_list(int word_index ,bool vertical);

// dtors.
void place_list_dtor(placement_list* place);
void gen_res_dtor(gen_result* gen_res);
void* place_node_dtor(void* place_node);
void* res_list_dtor(void* results);
void* thread_list_dtor(void* threads);


