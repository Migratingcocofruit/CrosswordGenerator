#include "sort_words.h"

void read_words(char* buff, int size)
{
    FILE* fp;
    if(fopen_s(&fp , FILE_PATH, "r") != 0)
    {
        perror("failed");
        printf("unable to open file: %s", FILE_PATH);
        return;
    }
    fgets(buff, size, fp);
    fclose(fp);
}

int count_words(char* buff, int size)
{
    int word_count = 1;
    for(int i = 0; i < size; i++)
    {
        if(buff[i] == ',')
        {
            word_count++;
        }
    }
    return word_count;
}

void split_words(char** words, char* buff, int buff_size, int wordlen)
{
    for(int i = 0, j = 0, k = 0, last = 0; i < buff_size && buff[i-1] != '\0'; i++)
    {
        if(buff[i] == ',' || buff[i] == '\0')
        {
            memcpy(words[j], buff + last, k);
            words[j][k] = '\0'; // add null terminator
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

void sort_words(char** words, int word_length, int word_count)
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
            adjacency[i][j] = common_letters(words[i], words[j]);
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
                    if(path_table[next_mask][k][0] < path_table[i][j][0] + adjacency[j][k])
                    {
                        path_table[next_mask][k][0] = path_table[i][j][0] + adjacency[j][k];
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

        // Back up the original list
        char** temp = (char**)malloc(word_count * sizeof(char*));
        for(int i = 0; i < word_count; i++)
        {
            temp[i] = malloc(sizeof(char) * word_length);
            memcpy(temp[i], words[i], word_length);
        }

        for(int i = 0; i < word_count; i++)
        {
            // Place elements in their place from the backup
            memcpy(words[i], temp[curr_vertex], word_length);
            // Walk the path
            last_vertex = curr_vertex;
            curr_vertex = path_table[curr_mask][curr_vertex][2];
            curr_mask = path_table[curr_mask][last_vertex][1];
        }
        free(temp);
    }
    free(adjacency);
    free(path_table);
}
