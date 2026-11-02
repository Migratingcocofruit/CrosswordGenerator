#include <stdio.h>
#include "generate_puzzle.h"
#include "sort_words.h"

int main()
{
    int size = height * width * 2;

    char* buff = (char*)malloc(size * sizeof(char));

    read_words(buff, size);

    count_words(buff, size);

    words_heaviest_path = (char**)malloc(word_count * sizeof(char**));
    for(int i = 0; i < word_count ; i++)
    {
        words_heaviest_path[i] = (char*)malloc(word_length * sizeof(char));
    }

    words_longest_first = (char**)malloc(word_count * sizeof(char**));
    for(int i = 0; i < word_count ; i++)
    {
        words_longest_first[i] = (char*)malloc(word_length * sizeof(char));
    }

    words = words_heaviest_path;

    split_words(buff, size);

    sort_words();

    printf("normal order:\n==========\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", words[i]);
    }

    printf("\nlongest first order:\n===============\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", words_longest_first[i]);
    }

    words = words_longest_first;

    struct Generation_Result* res = singlethread_puzzle_wrapper();

    printf("\n");

    if(res)
    {
        print_board(res->board);
    }
    else
    {
        printf("no board found\n");
    }

    free(buff);
    for(int i = 0; i < word_count; i++)
    {
        free(words[i]);
    }
    free(words);
}
