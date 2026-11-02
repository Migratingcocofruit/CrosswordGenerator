#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define DEFAULT_WIDTH 15
#define DEFAULT_HEIGHT 15

// globals
int word_count = 10, width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
int word_length = DEFAULT_WIDTH;
char** words;
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

int add_word(int next_word_i, int next_word_place, int tail_place ,struct Placement_List* place_list);

void remove_tail(struct Placement_List* place_list);

int place_word(struct Placement_Node* new_word, int next_word_place, int tail_place, struct Placement_List* place_list);

struct Generation_Result* create_puzzle_singlethread(int word_index, struct Placement_List* place_list);

struct Generation_Result* generate_result(struct Placement_List* place_list);

