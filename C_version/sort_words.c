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

void sort_words(char** words, int len, int width, int word_count)
{
    // Create weighted adjacency matrix for edges
    int adjacency[word_count][word_count];
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
    int path_table[1 << word_count][word_count];
    // Init path table to -inf. no paths yet
    for(int i = 0; i < ((1 << word_count) * word_count); i++)
        {
            **(path_table + i) = -INFINITY;
        }
    // Longest path to each vertex that includes only itself is always 0.
    for(int i = 0; i < word_count; i++)
    {
        path_table[1 << i][i] = 0;
    }
    for(int i = 0; i < (1 << word_count); i++)
    {
        for(int j = 0; j < word_count; j++)
        {
            
        }
    }
}
