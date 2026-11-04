#include "sort_words.h"

void read_words(char* buff, int size, char* path)
{
    FILE* fp;
    if(fopen_s(&fp , path, "r") != 0)
    {
        fprintf(stderr,"unable to open file \"%s\"", path);
        perror(": ");
        exit(1);
    }
    fgets(buff, size, fp);
    fclose(fp);
}

void count_words(char* buff, int size)
{
    word_count = 1;
    for(int i = 0; i < size; i++)
    {
        if(buff[i] == ',')
        {
            word_count++;
        }
    }
    return;
}

void split_words(char* buff, int buff_size)
{
    for(int i = 0, j = 0, k = 0, last = 0; i < buff_size && buff[i-1] != '\0'; i++)
    {
        
        if(buff[i] == ',' || buff[i] == '\0')
        {
            if(k > width && k > height)
            {
                char too_large[k+1];
                memcpy(too_large, buff + last, k);
                too_large[k] = '\0';
                fprintf(stderr, "\nWord:%s of length:%d too large for board size %dx%d\n", too_large, k, width, height);
                exit(1);
            }

            memcpy(word_list[j], buff + last, k);
            word_list[j][k] = '\0'; // add null terminator
            j++;
            k = 0;
            last = i+1;           
        }
        else
        {
            k++;
        }
    }
}

int common_letters(char* word1, char* word2)
{
    int count = 0;
    char letters1[26] = {0};
    char letters2[26] = {0};
    // All word arrays are the same length
    for(int i = 0; i < sizeof(word1); i++)
    {
        if(word1[i] <= 'z' && word1[i] >= 'a')
        {
            letters1[word1[i] - 'a']++;
        }
        if(word2[i] <= 'z' && word2[i] >= 'a')
        {
            letters2[word2[i] - 'a']++;
        }
    }
    for(int i = 0; i < 26; i++)
    {
        if(letters1[i] > 0 && letters2[i] > 0)
            // Add all possible intersections to count
            count += letters1[i] * letters2[i];
    }
    return count;
}

void sort_words()
{
    // Create weighted adjacency matrix for edges
    int** adjacency = (int**)malloc(word_count * sizeof(int*));
    // Initialize adjacency matrix
    for(int i = 0; i < word_count ; i++)
    {
        adjacency[i] = (int*)malloc(sizeof(int) * word_count);
    }
    for(int i = 0; i < word_count; i++)
    {
        adjacency[i][i] = 0;
        for(int j = i + 1; j < word_count; j++)
        {
            adjacency[i][j] = common_letters(word_list[i], word_list[j]);
            adjacency[j][i] = adjacency[i][j];
        }
    }

    // Find heaveist Hamiltonian path in graph given by adjacency matrix
    int*** path_table = (int***)malloc((int)sizeof(int**) * (1 << word_count));
    // Init path table
    for(int i = 0; i < (1 << word_count); i++)
    {
        path_table[i] = (int**)malloc(sizeof(int*) * word_count);
        for(int j = 0; j < word_count; j++)
        {
            path_table[i][j] = (int*)malloc(3 * sizeof(int));
            if(i & (1 << j))
            {
                path_table[i][j][0] = 0;
                path_table[i][j][1] = i; // "parent" subset is the subset itself
                path_table[i][j][2] = j; // parent vertex is itself
            }
            else
            {
                path_table[i][j][0] = -1;
            }
        }
    }
    // Each mask represents a subset. Every set's mask is always smaller than that of any 
    // of its super sets, so all subsets of a set are calculated in advance.
    for(int i = 1; i < (1 << word_count); i++)
    {
        for(int j = 0; j < word_count; j++)
        {
            // Vertex not in set. skip is
            if(!((1 << j) & i))
            {
                continue;
            }
            // Check if continuing the current longest path to the next vertex creates a longer path ending in it
            for(int k = 0; k < word_count; k++)
            {
                // If k isn't in the subset and there is an edge from j to k
                if(!((1 << k) & i) && adjacency[j][k] > 0)
                {
                    // Update weight of heaviest path also containing vertex j
                    int next_mask = i | (1 << k);
                    if(path_table[next_mask][k][0] < (path_table[i][j][0] > 0 ? path_table[i][j][0] * adjacency[j][k] : path_table[i][j][0] + adjacency[j][k]))
                    {
                        path_table[next_mask][k][0] = (path_table[i][j][0] > 0 ? path_table[i][j][0] * adjacency[j][k] : path_table[i][j][0] + adjacency[j][k]);
                        path_table[next_mask][k][1] = i; // "parent" subset is the one we got to it from
                        path_table[next_mask][k][2] = j; // parent vertex is the one we stepped from
                    }
                    
                }
            }
        }
    }
    int path_max = 0, path_end = 0, curr_mask = (1 << (word_count)) - 1, curr_vertex = path_end, last_vertex;
    for(int i = 0; i < word_count; i++)
    {
        if(path_max < path_table[curr_mask][i][0])
        {
            path_max = path_table[curr_mask][i][0];
            curr_vertex = i;
        }
    }
    printf("max path of size: %d\n", path_max);
    if(path_max <= 0)
    {
        // longest path is not hamiltonian
        printf("No hamiltonian path found");
    }
    else
    {
        // Walk the heaviest path and swap each word to its place in the list

        for(int i = 0; i < word_count; i++)
        {
            // Place elements in their new place
            memcpy(order_types[1][i], word_list[curr_vertex], word_length);
            // Walk the path
            last_vertex = curr_vertex;
            curr_vertex = path_table[curr_mask][curr_vertex][2];
            curr_mask = path_table[curr_mask][last_vertex][1];
        }

        int word_max_length = 0;
        curr_mask = (1 << (word_count)) - 1;
        for(int i = 0; i < word_count; i++)
        {
            if(strlen(word_list[i]) > word_max_length)
            {
                word_max_length = (int)strlen(word_list[i]);
                curr_vertex = i;
            }
        }

        for(int i = 0; i < word_count; i++)
        {
            // Place elements in their new place
            memcpy(order_types[2][i], word_list[curr_vertex], word_length);
            // Walk the path
            last_vertex = curr_vertex;
            curr_vertex = path_table[curr_mask][curr_vertex][2];
            curr_mask = path_table[curr_mask][last_vertex][1];
        }
    }

    sort_words_commons(adjacency);

    for(int i = 0; i < word_count; i++)
    {
        free(adjacency[i]);
    }
    free(adjacency);
    for(int i = 0; i < (1 << word_count); i++)
    {
        for(int j = 0; j < word_count; j++)
        {
            free(path_table[i][j]);
        }
        free(path_table[i]);
    }
    free(path_table);
}

void sort_words_commons(int** adjacency)
{
    int* commons = (int*)malloc(word_count * sizeof(int));
    for(int i = 0; i < word_count; i++)
    {
        commons[i] = 0;
        for(int j = 0; j < word_count; j++)
        {
            commons[i] += adjacency[i][j];
        }
    }
    for(int max_ind = 0, max = 0, i = 0; i < word_count; i++)
    {
        max = 0;
        for(int j = 0; j < word_count; j++)
        {
            if(commons[j] > max)
            {
                max = commons[j];
                max_ind = j;
            }
        }
        commons[max_ind] = 0;
        memcpy(order_types[3][i], word_list[max_ind], word_length);
    }
    free(commons);
}