#include <stdio.h>
#include "generate_puzzle.h"
#include "sort_words.h"

int main()
{
    int size = height * width * 2;

    char* buff = (char*)malloc(size * sizeof(char));

    read_words(buff, size);

    count_words(buff, size);

    words = (char**)malloc(word_count * sizeof(char**));
    for(int i = 0; i < word_count ; i++)
    {
        words[i] = (char*)malloc(word_length * sizeof(char));
    }

    split_words(buff, size);

    sort_words(words);

    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", words[i]);
    }

    struct Generation_Result* res = singlethread_puzzle_wrapper();

    printf("\n");

    print_board(res->board);

    free(buff);
    for(int i = 0; i < word_count; i++)
    {
        free(words[i]);
    }
    free(words);
}
