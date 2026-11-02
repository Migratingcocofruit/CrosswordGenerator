#include "generate_puzzle.h"

int add_word(int next_word_i, int next_word_place, int attachment_place, placement_node* attachment, placement_list* place_list)
{
    placement_node* placement = (placement_node*)malloc(sizeof(placement_node));

    bool vertical = attachment->coords[1] == attachment->coords[3];

    placement->word_index = next_word_i;

    // Set new word coords
    if(vertical)
    {
        placement->coords[0] = attachment->coords[0] + attachment_place;
        placement->coords[1] = attachment->coords[1] - next_word_place;
        placement->coords[2] = placement->coords[0];
        placement->coords[3] = placement->coords[1] + (int)strlen(words[next_word_i]) - 1;
    }
    else
    {
        
        placement->coords[0] = attachment->coords[0] - next_word_place;
        placement->coords[1] = attachment->coords[1] + attachment_place;
        placement->coords[2] = placement->coords[0] + (int)strlen(words[next_word_i]) - 1;
        placement->coords[3] = placement->coords[1];
    }

    // Update board edge coords
    for(int i = 0; i < 4; i++)
    {
        if(i < 2)
        {
            placement->edge_coords[i] = fmin(place_list->tail->edge_coords[i], placement->coords[i]);
        }
        else
        {
            placement->edge_coords[i] = fmax(place_list->tail->edge_coords[i], placement->coords[i]);
        }
    }

    // Placement excceds boundries. Abort and return -1
    if(abs(placement->edge_coords[3] - placement->edge_coords[1]) >= width || abs(placement->edge_coords[2] - placement->edge_coords[0]) >= height)
    {
        free(placement);
        return -1;
    }

    int ret_val = place_word(placement, place_list);

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

int place_word(placement_node* new_word, placement_list* place_list)
{
    int crossovers = 0, line_start = 0, line_end = 2, cross_start = 1, cross_end = 3;
    bool vertical = new_word->coords[0] == new_word->coords[2];

    if(vertical)
    {
        line_start++;
        line_end++;
        cross_start--;
        cross_end--;
    }

    // check if any word interferes with us
    placement_node* current = place_list->tail;
    while(current != NULL)
    {
        // There is a word starting ahead or ending behind where we are placing, or if we overlap or are adjacent to another word in the same orientation Abort.
        // Checking overlap
        if(current->coords[line_end] != current->coords[line_start] 
        && (abs(new_word->coords[cross_start] - current->coords[cross_start]) < 2) 
        && one_in_range(current->coords[line_start], current->coords[line_end], new_word->coords[line_start], new_word->coords[line_end]))
        {
            return -1;
        }
        // Checking end butting
        else if(((new_word->coords[line_start] - 1 == current->coords[line_end] || new_word->coords[line_end] + 1 == current->coords[line_start]))
            && (in_range(new_word->coords[cross_start], current->coords[cross_start], current->coords[cross_end])))
        {
            return -1;
        }
        // Check that all letters only overlap with other words on the same letter
        for(int i = 0; i <= (new_word->coords[line_end] - new_word->coords[line_start]); i++)
        {
            if((new_word->coords[line_start] + i == current->coords[line_start])
            && (new_word->coords[cross_start] >= current->coords[cross_start] && new_word->coords[cross_start] <= current->coords[cross_end]))
            {
                if(words[new_word->word_index][i] == words[current->word_index][new_word->coords[cross_start] - current->coords[cross_start]])
                {
                    crossovers++;
                }
                else
                {
                    return -1;
                }
            }
        }
        current = current->previous;
    }
    return crossovers;
}

void remove_tail(placement_list* place_list)
{
    place_list->tail = place_list->tail->previous;
    free(place_list->tail->next);
    place_list->tail->next = NULL;
}

// Converts a word placement list into a result struct
gen_result* generate_result(placement_list* place_list)
{
    bool vertical;
    placement_node* curr_wnode = place_list->tail;
    int min_x = curr_wnode->edge_coords[0], min_y = curr_wnode->edge_coords[1];
    // Allocate struct
    gen_result* res = (gen_result*)malloc(sizeof(gen_result));
    res->board = (char**)malloc(sizeof(char*) * width);
    for(int i = 0; i < width; i++)
    {
        res->board[i] = (char*)malloc(sizeof(char) * height);
        for(int j = 0; j < height; j++)
        {
            res->board[i][j] = ' ';
        }
    }
    res->crossovers = curr_wnode->crossovers;

    // Place words
    while(curr_wnode != NULL)
    {
        vertical = curr_wnode->coords[0] == curr_wnode->coords[2];
        for(int i = 0; i < strlen(words[curr_wnode->word_index]); i++)
        {
            if(vertical)
            {
                res->board[curr_wnode->coords[0] - min_x][i + curr_wnode->coords[1] - min_y] = words[curr_wnode->word_index][i];
            }
            else
            {
                res->board[curr_wnode->coords[0] + i - min_x][curr_wnode->coords[1] - min_y] = words[curr_wnode->word_index][i];
            }
        }
        curr_wnode = curr_wnode->previous;
    }

    return res;
}

gen_result* create_puzzle_singlethread(int word_index, placement_list* place_list)
{
    if(word_index >= word_count)
    {
        return generate_result(place_list);
    }

    gen_result* max_board = NULL, *curr_board = NULL;

    placement_node* curr_word = place_list->tail;

    while(curr_word)
    {
        for(int i = 0; i < strlen(words[word_index]); i++)
        {
            for(int j = 0; j < strlen(words[curr_word->word_index]); j++)
            {
                if(words[word_index][i] == words[curr_word->word_index][j])
                {
                    if(add_word(word_index, i, j, curr_word, place_list) > 0)
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
        curr_word = curr_word->previous;
    }
    return max_board;
}

gen_result* singlethread_puzzle_wrapper()
{
    gen_result* res_h = NULL;
    gen_result* res_v = NULL;

    placement_list* place_list_h = (placement_list*)malloc(sizeof(placement_list));
    placement_list* place_list_v = (placement_list*)malloc(sizeof(placement_list));

    place_list_h->tail = (placement_node*)malloc(sizeof(placement_node));
    place_list_v->tail = (placement_node*)malloc(sizeof(placement_node));

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

void print_board(gen_result* res)
{
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            printf("|%c", res->board[i][j]);
        }
        printf("|\n");
    }
    printf("score: %d\n", res->crossovers);
}

placement_list* copy_placement_list(placement_list* placement)
{
    placement_list* new_list = (placement_list*)malloc(sizeof(placement_list));
    new_list->head = NULL;
    new_list->tail = NULL;
    if(placement->head == NULL)
    {
        return new_list;
    }
    placement_node* current = placement->head;
    placement_node* new_node = NULL;
    while(current)
    {
        new_node = (placement_node*)malloc(sizeof(placement_node));
        memcpy(new_node->coords, current->coords, 4);
        memcpy(new_node->edge_coords, current->edge_coords, 4);
        new_node->word_index = current->word_index;
        new_node->crossovers = current->crossovers;
        new_node->next = NULL;
        if(new_list->tail)
        {
            new_list->tail->next = new_node;
            new_node->previous = new_list->tail;
        }
        else
        {
            new_list->tail = new_node;
            new_list->head = new_node;
        }
        new_list->tail = new_node;

        current = current->next;
    }
    return new_list;
}