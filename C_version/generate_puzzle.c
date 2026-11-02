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
        placement->coords[3] = placement->coords[1] + strlen(words[next_word_i]) - 1;
    }
    else
    {
        
        placement->coords[0] = place_list->tail->coords[0] - next_word_place;
        placement->coords[1] = place_list->tail->coords[1] + tail_place;
        placement->coords[2] = placement->coords[0] + strlen(words[next_word_i]) - 1;
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
    struct Placement_Node* current = place_list->head;
    while(current != NULL)
    {
        // There is a word directly ahead or behind where we are placing, or if we overlap another word in the same orientation Abort.
        if(vertical)
        {
            if((new_word->coords[1] - 1 >= current->coords[0] && new_word->coords[1] - 1 <= current->coords[2])
            || (new_word->coords[3] + 1 >= current->coords[0] && new_word->coords[3] + 1 <= current->coords[2])
            || (new_word->coords[1] - 1 >= current->coords[1] && new_word->coords[1] - 1 <= current->coords[3])
            || (new_word->coords[3] + 1 >= current->coords[1] && new_word->coords[3] + 1 <= current->coords[3])
            || (new_word->coords[1] <= current->coords[1] && new_word->coords[3] >= current->coords[3] && current->coords[3] != current->coords[1] && new_word->coords[1] == current->coords[0]))
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
            if((new_word->coords[0] - 1 >= current->coords[1] && new_word->coords[0] - 1 <= current->coords[3])
            || (new_word->coords[2] + 1 >= current->coords[1] && new_word->coords[2] + 1 <= current->coords[3])
            || (new_word->coords[0] - 1 >= current->coords[0] && new_word->coords[0] - 1 <= current->coords[2])
            || (new_word->coords[2] + 1 >= current->coords[0] && new_word->coords[2] + 1 <= current->coords[2])
            || (new_word->coords[0] <= current->coords[0] && new_word->coords[2] >= current->coords[2] && current->coords[2] != current->coords[0]  && new_word->coords[1] == current->coords[0]))
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
    }
}