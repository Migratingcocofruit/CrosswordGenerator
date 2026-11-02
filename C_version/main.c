#include <stdio.h>
#include "generate_puzzle.h"
#include "sort_words.h"

int main()
{
    int size = height * width * 2;

    char* buff = (char*)malloc(size * sizeof(char));

    read_words(buff, size);

    count_words(buff, size);

    word_list = (char**)malloc(word_count * sizeof(char**));
    words_commons = (char**)malloc(word_count * sizeof(char**));
    words_heaviest_path = (char**)malloc(word_count * sizeof(char**));
    words_longest_first = (char**)malloc(word_count * sizeof(char**));

    for(int i = 0; i < word_count ; i++)
    {
        word_list[i] = (char*)malloc(word_length * sizeof(char));
        words_heaviest_path[i] = (char*)malloc(word_length * sizeof(char));
        words_longest_first[i] = (char*)malloc(word_length * sizeof(char));
        words_commons[i] = (char*)malloc(word_length * sizeof(char));
    }

    words = words_longest_first;

    split_words(buff, size);

    sort_words();

    printf("normal order:\n==========\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", word_list[i]);
    }

    printf("\nheaviest path order:\n===============\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", words_heaviest_path[i]);
    }

    printf("\nlongest first order:\n===============\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", words_longest_first[i]);
    }

    printf("\nmost common characters order:\n===============\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", words_commons[i]);
    }

    //words = words_longest_first;

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
        free(word_list[i]);
        free(words_longest_first[i]);
        free(words_heaviest_path[i]);
        free(words_commons[i]);
    }
    free(word_list);
    free(words_longest_first);
    free(words_heaviest_path);
    free(words_commons);
}
