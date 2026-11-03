#include "generate_puzzle.h"

int add_word(int next_word_i, int next_word_place, int attachment_place, placement_node* attachment, placement_list* place_list)
{
    placement_node* placement = (placement_node*)malloc(sizeof(placement_node));
    placement->next == NULL;
    placement->previous == NULL;

    bool vertical = attachment->coords[1] == attachment->coords[3];

    placement->word_index = next_word_i;

    // Set new word coords
    if(vertical)
    {
        placement->coords[0] = attachment->coords[0] + attachment_place;
        placement->coords[1] = attachment->coords[1] - next_word_place;
        placement->coords[2] = placement->coords[0];
        placement->coords[3] = placement->coords[1] + (int)strlen(order_types[place_list->order_index][next_word_i]) - 1;
    }
    else
    {
        
        placement->coords[0] = attachment->coords[0] - next_word_place;
        placement->coords[1] = attachment->coords[1] + attachment_place;
        placement->coords[2] = placement->coords[0] + (int)strlen(order_types[place_list->order_index][next_word_i]) - 1;
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
        else if(in_range(current->coords[line_start], new_word->coords[line_start],new_word->coords[line_end])
            && (current->coords[cross_start] - 1 == new_word->coords[cross_start] || current->coords[cross_end] + 1 == new_word->coords[cross_start]))
        {
            return -1;
        }
        // Check that all letters only overlap with other words on the same letter
        for(int i = 0; i <= (new_word->coords[line_end] - new_word->coords[line_start]); i++)
        {
            if((new_word->coords[line_start] + i == current->coords[line_start])
            && (new_word->coords[cross_start] >= current->coords[cross_start] && new_word->coords[cross_start] <= current->coords[cross_end]))
            {
                if(order_types[place_list->order_index][new_word->word_index][i] == order_types[place_list->order_index][current->word_index][new_word->coords[cross_start] - current->coords[cross_start]])
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
    if(!place_list)
    {
        return NULL;
    }
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
        for(int i = 0; i < strlen(order_types[place_list->order_index][curr_wnode->word_index]); i++)
        {
            if(vertical)
            {
                res->board[curr_wnode->coords[0] - min_x][i + curr_wnode->coords[1] - min_y] = order_types[place_list->order_index][curr_wnode->word_index][i];
            }
            else
            {
                res->board[curr_wnode->coords[0] + i - min_x][curr_wnode->coords[1] - min_y] = order_types[place_list->order_index][curr_wnode->word_index][i];
            }
        }
        curr_wnode = curr_wnode->previous;
    }

    return res;
}

placement_list* create_puzzle_singlethread(int word_index, placement_list* place_list)
{
    if(word_index >= word_count)
    {
        return place_list;
    }

    placement_list* max_board = NULL, *curr_board = NULL;

    placement_node* curr_word = place_list->tail;

    while(curr_word)
    {
        for(int i = 0; i < strlen(order_types[place_list->order_index][word_index]); i++)
        {
            for(int j = 0; j < strlen(order_types[place_list->order_index][curr_word->word_index]); j++)
            {
                if(order_types[place_list->order_index][word_index][i] == order_types[place_list->order_index][curr_word->word_index][j])
                {
                    if(add_word(word_index, i, j, curr_word, place_list) > 0)
                    {                        
                        curr_board = create_puzzle_singlethread(word_index + 1, place_list);
                        if(curr_board && (max_board == NULL || curr_board->tail->crossovers > max_board->tail->crossovers))
                        {
                            place_list_dtor(max_board);
                            max_board = copy_placement_list(curr_board);
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

placement_list* init_placement_list(int word_index, int word_order ,bool vertical)
{
    placement_list* place_list = (placement_list*)malloc(sizeof(placement_list));

    place_list->tail = (placement_node*)malloc(sizeof(placement_node));

    place_list->order_index = word_order;

    place_list->head = place_list->tail;

    place_list->head->next = NULL;
    place_list->head->previous = NULL;
    
    place_list->head->crossovers = 0;

    place_list->tail->word_index = word_index;
    place_list->tail->coords[0] = 0;
    place_list->tail->coords[1] = 0;
    place_list->tail->coords[2] = 0;
    place_list->tail->coords[3] = (int)strlen(order_types[word_order][place_list->tail->word_index]) - 1;

    memcpy(place_list->tail->edge_coords, place_list->tail->coords,sizeof(int) * 4);

    return place_list;
}

void puzzle_gen_wrapper(gen_result** res ,bool mt)
{
    mt ? puzzle_gen_wrapper_mt(res) : puzzle_gen_wrapper_st(res);
}

void puzzle_gen_wrapper_mt(gen_result** res)
{
    thread_list* thread_lists[ORDER_TYPE_COUNT][2];
    thread_list *curr_thread;
    placement_list* max_board[ORDER_TYPE_COUNT];
    gen_result* res_h = NULL;
    gen_result* res_v = NULL;

    // Create threads for each sorting
    for(int i = 0 ; i < ORDER_TYPE_COUNT; i++)
    {
        placement_list* place_list_h = init_placement_list(0, i, false);
        placement_list* place_list_v = init_placement_list(0, i, true);

        create_puzzle_multithread(1, place_list_h, thread_lists[i]);
        create_puzzle_multithread(1, place_list_v, thread_lists[i] + 1);

        place_list_dtor(place_list_h);
        place_list_dtor(place_list_v);
    }
    
    // Join and find maximum for each sorting
    for(int i = 0 ; i < ORDER_TYPE_COUNT; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            curr_thread = thread_lists[i][j];
            while(curr_thread)
            {
                pthread_join(curr_thread->thread, NULL);
                curr_thread = curr_thread->next;
            }
        }
    }

    for(int i = 0 ; i < ORDER_TYPE_COUNT; i++)
    {
        max_board[i] = NULL;
        for(int j = 0; j < 2; j++)
        {
            curr_thread = thread_lists[i][j];
            // join threads and pick the maximum result
            while(curr_thread)
            {
                if(curr_thread && curr_thread->place && (!max_board[i] || max_board[i]->tail->crossovers < curr_thread->place->tail->crossovers))
                {
                    place_list_dtor(max_board[i]);
                    max_board[i] = curr_thread->place;
                    curr_thread->place = NULL;
                }
                curr_thread = thread_list_dtor(curr_thread);
            }
        }
    }

    for(int i = 0; i < ORDER_TYPE_COUNT; i++)
    {
        words = order_types[i];
        res[i] = generate_result(max_board[i]);
    }
}

void puzzle_gen_wrapper_st(gen_result** res)
{
    for(int i=0; i < ORDER_TYPE_COUNT; i++)
    {
        gen_result* res_h = NULL;
        gen_result* res_v = NULL;

        placement_list* place_list_h = init_placement_list(0, i, false);
        placement_list* place_list_v = init_placement_list(0, i, true);

        
        res_h = generate_result(create_puzzle_singlethread(1, place_list_h));
        res_v = generate_result(create_puzzle_singlethread(1, place_list_v));
        

        place_list_dtor(place_list_h);
        place_list_dtor(place_list_v);

        if(res_v == NULL || res_h->crossovers > res_v->crossovers)
        {
            gen_res_dtor(res_v);
            res[i] = res_h;
        }
        gen_res_dtor(res_h);
        res[i] = res_v;

    }
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
    if(!placement)
    {
        return NULL;
    }
    
    placement_list* new_list = (placement_list*)malloc(sizeof(placement_list));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->order_index = placement->order_index;
    if(placement->head == NULL)
    {
        return new_list;
    }
    placement_node* current = placement->head;
    placement_node* new_node = NULL;
    while(current)
    {
        new_node = (placement_node*)malloc(sizeof(placement_node));
        memcpy(new_node->coords, current->coords, 4 * sizeof(int));
        memcpy(new_node->edge_coords, current->edge_coords, 4 * sizeof(int));
        new_node->word_index = current->word_index;
        new_node->crossovers = current->crossovers;
        new_node->next = NULL;
        new_node->previous = NULL;

        if(new_list->tail)
        {
            new_list->tail->next = new_node;
            new_node->previous = new_list->tail;
        }
        else
        {  
            new_list->head = new_node;
        }

        new_list->tail = new_node;
        current = current->next;
    }
    return new_list;
}

void create_puzzle_multithread(int word_index, placement_list* place_list, thread_list **thread_head)
{
    thread_list* curr_thread = NULL, *prev_thread = NULL;

    *thread_head = NULL;

    placement_list* max_board = NULL, *curr_board = NULL;

    placement_node* curr_word = place_list->tail;

    while(curr_word)
    {
        for(int i = 0; i < strlen(order_types[place_list->order_index][word_index]); i++)
        {
            for(int j = 0; j < strlen(order_types[place_list->order_index][curr_word->word_index]); j++)
            {
                if(order_types[place_list->order_index][word_index][i] == order_types[place_list->order_index][curr_word->word_index][j])
                {
                    if(add_word(word_index, i, j, curr_word, place_list) > 0)
                    {
                        if(!curr_thread)
                        {
                            curr_thread = (thread_list*)malloc(sizeof(thread_list));
                            *thread_head = curr_thread;
                        }
                        else
                        {
                            curr_thread->next = (thread_list*)malloc(sizeof(thread_list));
                            curr_thread = curr_thread->next;
                        }
                        curr_thread->place = copy_placement_list(place_list);
                        curr_thread->next = NULL;
                        pthread_create(&(curr_thread->thread), NULL, multi_single_wrapper, (void*)(curr_thread));          
                    }
                    remove_tail(place_list);
                }
            }
        }
        curr_word = curr_word->previous;
    }
}

void* multi_single_wrapper(void* thread)
{
    ((thread_list*)thread)->place = create_puzzle_singlethread(2, ((thread_list*)thread)->place);
    return thread;
}

void list_dtor(void*(*dtor)(void*), void* list)
{
    void* curr = list;
    while(curr)
    {
        curr = dtor(curr);
    }
}

void* place_node_dtor(void* place_node)
{
    if(!place_node)
    {
        return NULL;
    }
    placement_node* next = ((placement_node*)place_node)->next;
    if(next)
    {
        next->previous = NULL;
    }
    free(place_node);
    return (void*)next;
}

void* thread_list_dtor(void* threads)
{
    if(!threads)
    {
        return NULL;
    }
    place_list_dtor(((thread_list*)threads)->place);

    thread_list* next = ((thread_list*)threads)->next;

    free(threads);
    return (void*)next;

}

void place_list_dtor(placement_list* place)
{
    if(!place)
    {
        return;
    }
    list_dtor(place_node_dtor, (void*)place->head);
    free(place);
}

void gen_res_dtor(gen_result* gen_res)
{
    if(!gen_res)
    {
        return;
    }
    if(gen_res->board)
    {
        for(int i = 0; i < width; i++)
        {
            free(gen_res->board[i]);
        }
        free(gen_res->board);
    }

    free(gen_res);
}