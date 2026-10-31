#include <sort_words.h>

void read_words(char* buff)
{
    FILE* fp = fopen(FILE_PATH, 'r');
    fgets(buff, sizeof(buff), fp);
}

int count_words(char* buff)
{
    int word_count = 1;
    for(int i = 0; i < sizeof(buff); i++)
    {
        if(buff[i] == ',')
        {
            word_count++;
        }
    }
    return word_count;
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
            count += letters1[i] + letters2[i];
    }
}

void sort_words(char** words, int word_length, int word_count)
{
    // Create weighted adjacency matrix for edges
    int adjacency[word_count][word_count];
    // Initialize adjacency matrix
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
    int path_table[1 << word_count][word_count][3];
    // Init path table to -inf. no paths yet.
    for(int i = 0; i < ((1 << word_count) * word_count); i++)
        {
            **(path_table + i)[0] = -INFINITY;
        }

    // Longest path to each vertex that includes only itself is always 0.
    for(int i = 0; i < word_count; i++)
    {
        path_table[1 << i][i][0] = 0;
        path_table[1 << i][i][1] = 1 << i; // "parent" subset is the subset itself
        path_table[1 << i][i][2] = i; // parent vertex is itself
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
                    int next_mask = i | 1 << k;
                    if(path_table[next_mask][k][0] < path_table[i][j] + adjacency[j][k])
                    {
                        path_table[next_mask][k][0] = path_table[i][j] + adjacency[j][k];
                        path_table[next_mask][k][1] = i; // "parent" subset is the one we got to it from
                        path_table[next_mask][k][2] = j; // parent vertex is the one we stepped from
                    }
                    
                }
            }
        }
    }
    int path_max = 0, path_end = 0, curr_mask = (1 << (word_count + 1)) - 1, curr_vertex = path_end, last_vertex;
    for(int i = 0; i < word_count; i++)
    {
        if(path_max < path_table[curr_mask][i][0])
        {
            path_max = path_table[curr_mask][i][0];
            curr_vertex = i;
        }
        
    }
    if(path_max <= 0)
    {
        // longest path is not hamiltonian
        printf("No hamiltonian path found");
        return;
    }
    else
    {
        // Walk the heaviest path and swap each word to its place in the list
        char temp[word_length];        
        for(int i = 0; i < word_count; i++)
        {
            // Swap element to its place
            memcpy(temp, words[word_count], word_length);
            memcpy(words[word_count], words[curr_vertex], word_length);
            memcpy(words[curr_vertex], temp, word_length);
            // Walk the path
            last_vertex = curr_vertex;
            curr_vertex = path_table[curr_mask][curr_vertex][2];
            curr_mask = path_table[curr_mask][last_vertex][1];
        }
    }
}
