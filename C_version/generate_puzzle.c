#include "generate_puzzle.h"

int add_word(int next_word_i, int next_word_place, int tail_place ,struct Placement_List* place_list)
{
    struct Placement_Node* placement = (struct Placement_Node*)malloc(sizeof(struct Placement_Node));

    bool vertical = place_list->tail->coords[1] == place_list->tail->coords[3];

    placement->word_index = next_word_i;

    // Set new word coords
    if(vertical)
    {
        placement->coords[0] = place_list->tail->coords[0] + tail_place;
        placement->coords[1] = place_list->tail->coords[1] - next_word_place;
        placement->coords[2] = placement->coords[0];
        placement->coords[3] = placement->coords[1] + (int)strlen(words[next_word_i]) - 1;
    }
    else
    {
        
        placement->coords[0] = place_list->tail->coords[0] - next_word_place;
        placement->coords[1] = place_list->tail->coords[1] + tail_place;
        placement->coords[2] = placement->coords[0] + (int)strlen(words[next_word_i]) - 1;
        placement->coords[3] = placement->coords[1];
    }

    // Update board edge coords
    for(int i = 0; i < 4; i++)
    {
        if(i < 2)
        {
            placement->edge_coords[i] = min(place_list->tail->edge_coords[i], placement->coords[i]);
        }
        else
        {
            placement->edge_coords[i] = max(place_list->tail->edge_coords[i], placement->coords[i]);
        }
    }

    // Placement excceds boundries. Abort and return -1
    if(abs(placement->edge_coords[3] - placement->edge_coords[1]) > width || abs(placement->edge_coords[2] - placement->edge_coords[0]) > height)
    {
        free(placement);
        return -1;
    }

    int ret_val = place_word(placement, next_word_place, tail_place, place_list);

    // Couldn't place word due to clashing
    if(ret_val < 0)
    {
        free(placement);
        return ret_val;
    }

    // Add new crossovers to total
    placement->crossovers = place_list->tail->crossovers + ret_val;

    // Positioning successful. Append new node to the end of the list
    placement->next = NULL;
    placement->previous = place_list->tail;
    place_list->tail->next = placement;
    place_list->tail = placement;

    return ret_val;
}

place_word(struct Placement_Node* new_word, int next_word_place, int tail_place, struct Placement_List* place_list)
{
    int crossovers = 0;
    bool vertical = new_word->coords[0] == new_word->coords[2];
    // check if any word interferes with us
    struct Placement_Node* current = place_list->tail;
    while(current != NULL)
    {
        // There is a word directly ahead or behind where we are placing, or if we overlap another word in the same orientation Abort.
        if(vertical)
        {
            if(
            // Overlapping with another vertical word
            (current->coords[3] != current->coords[1] && new_word->coords[0] == current->coords[0] 
            && ((new_word->coords[1] - 1 <= current->coords[3] && new_word->coords[3] + 1 >= current->coords[3])
            || (new_word->coords[3] + 1 >= current->coords[1] && new_word->coords[1] - 1 <= current->coords[1])))
            // A horizontal word passes directly in front or behind the new word
            || (new_word->coords[1] - 1 == current->coords[1] && new_word->coords[0] >= current->coords[0] && new_word->coords[0] <= current->coords[2])
            || (new_word->coords[3] + 1 == current->coords[1] && new_word->coords[0] >= current->coords[0] && new_word->coords[0] <= current->coords[2]))
            {
                return -1;
            }
            // Check that all letters only overlap with other words on the same letter
            for(int i = 0; i <= (new_word->coords[3] - new_word->coords[1]); i++)
            {
                if((new_word->coords[1] + i == current->coords[1])
                && (new_word->coords[0] >= current->coords[0] && new_word->coords[0] <= current->coords[2]))
                {
                    if(words[new_word->word_index][i] == words[current->word_index][new_word->coords[0] - current->coords[0]])
                    {
                        crossovers++;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
        }
        // Same but for a horizontal word
        else
        {
            if(
            // Overlapping with another horizontal word
            (current->coords[0] != current->coords[2] && new_word->coords[1] == current->coords[1] 
            && ((new_word->coords[0] - 1 <= current->coords[2] && new_word->coords[2] + 1 >= current->coords[2])
            || (new_word->coords[2] + 1 >= current->coords[0] && new_word->coords[0] - 1 <= current->coords[0])))
            // A vertical word passes directly infront or behind.
            || (new_word->coords[0] - 1 == current->coords[0] && new_word->coords[1] >= current->coords[1] && new_word->coords[1] <= current->coords[3])
            || (new_word->coords[2] + 1 == current->coords[0] && new_word->coords[1] >= current->coords[1] && new_word->coords[1] <= current->coords[3]))
            {
                return -1;
            }
            // Check that all letters only overlap with other words on the same letter
            for(int i = 0; i <= (new_word->coords[2] - new_word->coords[0]); i++)
            {
                if((new_word->coords[0] + i == current->coords[0])
                && (new_word->coords[1] >= current->coords[1] && new_word->coords[1] <= current->coords[3]))
                {
                    if(words[new_word->word_index][i] == words[current->word_index][new_word->coords[1] - current->coords[1]])
                    {
                        crossovers++;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
        }
        current = current->previous;
    }
    return crossovers;
}

void remove_tail(struct Placement_List* place_list)
{
    place_list->tail = place_list->tail->previous;
    free(place_list->tail->next);
    place_list->tail->next = NULL;
}

// Converts a word placement list into a result struct
struct Generation_Result* generate_result(struct Placement_List* place_list)
{
    bool vertical;
    struct Placement_Node* curr_wnode = place_list->tail;
    int min_x = curr_wnode->edge_coords[0], min_y = curr_wnode->edge_coords[1];
    // Allocate struct
    struct Generation_Result* res = (struct Generation_Result*)malloc(sizeof(struct Generation_Result));
    res->board = (char**)malloc(sizeof(char*) * width);
    for(int i = 0; i < width; i++)
    {
        res->board[i] = (char*)malloc(sizeof(char) * height);
    }
    res->crossovers = curr_wnode->crossovers;

    // Place words
    while(curr_wnode != NULL)
    {
        vertical = curr_wnode->coords[1] == curr_wnode->coords[3];
        // normalize coords of each word so our lowest are at [0,0]
        curr_wnode->coords[0] -= min_x;
        curr_wnode->coords[2] -= min_x;
        curr_wnode->coords[1] -= min_y;
        curr_wnode->coords[3] -= min_y;
        for(int i = 0; i < strlen(words[curr_wnode->word_index]); i++)
        {
            if(vertical)
            {
                res->board[curr_wnode->coords[0]][i + curr_wnode->coords[1]];
            }
            else
            {
                res->board[curr_wnode->coords[0] + i][curr_wnode->coords[1]];
            }
        }
        curr_wnode = curr_wnode->previous;
    }

    return res;
}

struct Generation_Result* create_puzzle_singlethread(int word_index, struct Placement_List* place_list)
{
    if(word_index >= word_count)
    {
        return generate_result(place_list);
    }

    struct Generation_Result* max_board = NULL, *curr_board = NULL;

    for(int i = 0; i < strlen(words[word_index]); i++)
    {
        for(int j = 0; j < strlen(words[place_list->tail->word_index]); j++)
        {
            if(words[word_index][i] == words[place_list->tail->word_index][j])
            {
                if(add_word(word_index, i, j, place_list) > 0)
                {
                    curr_board = create_puzzle_singlethread(word_index + 1, place_list);
                    if(curr_board && (max_board == NULL || curr_board->crossovers > max_board->crossovers))
                    {
                        free(max_board);
                        max_board = curr_board;
                    }
                    remove_tail(place_list);
                }
            }
        }
    }

    return max_board;
}

struct Generation_Result* singlethread_puzzle_wrapper()
{
    struct Generation_Result* res_h = NULL;
    struct Generation_Result* res_v = NULL;

    struct Placement_List* place_list_h = (struct Placement_List*)malloc(sizeof(struct Placement_List));
    struct Placement_List* place_list_v = (struct Placement_List*)malloc(sizeof(struct Placement_List));

    place_list_h->tail = (struct Placement_Node*)malloc(sizeof(struct Placement_Node));
    place_list_v->tail = (struct Placement_Node*)malloc(sizeof(struct Placement_Node));

    place_list_h->head = place_list_h->tail;
    place_list_v->head = place_list_v->tail;

    place_list_v->head->next = NULL;
    place_list_v->head->previous = NULL;
    place_list_h->head->next = NULL;
    place_list_h->head->previous = NULL;
    
    place_list_h->head->crossovers = 0;
    place_list_v->head->crossovers = 0;

    place_list_h->tail->word_index = 0;
    place_list_h->tail->coords[0] = 0;
    place_list_h->tail->coords[1] = 0;
    place_list_h->tail->coords[2] = (int)strlen(words[place_list_h->tail->word_index]) - 1;
    place_list_h->tail->coords[3] = 0;

    place_list_v->tail->word_index = 0;
    place_list_v->tail->coords[0] = 0;
    place_list_v->tail->coords[1] = 0;
    place_list_v->tail->coords[2] = 0;
    place_list_v->tail->coords[3] = (int)strlen(words[place_list_h->tail->word_index]) - 1;

    for(int i = 0; i < 4; i++)
    {
        place_list_v->tail->edge_coords[i] = place_list_v->tail->coords[i];
        place_list_h->tail->edge_coords[i] = place_list_h->tail->coords[i];
    }

    res_h = create_puzzle_singlethread(1, place_list_h);
    res_v = create_puzzle_singlethread(1, place_list_v);

    // TODO: destroy lists

    if(res_v == NULL || res_h->crossovers > res_v->crossovers)
    {
        return res_h;
    }
    return res_v;
}

void print_board(char** board)
{
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            printf("|%c", board[i][j]);
        }
        printf("|\n");
    }
}