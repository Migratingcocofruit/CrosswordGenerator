#include <stdio.h>
#include "generate_puzzle.h"
#include "sort_words.h"

int main()
{
    int size = height * width * 2;

    char* buff = (char*)malloc(size * sizeof(char));

    int order = 0;

    read_words(buff, size);

    count_words(buff, size);

    order_types = (char***)malloc(sizeof(char**) * 3);

    word_list = (char**)malloc(word_count * sizeof(char**));
    order_types[2] = (char**)malloc(word_count * sizeof(char**));
    order_types[0] = (char**)malloc(word_count * sizeof(char**));
    order_types[1] = (char**)malloc(word_count * sizeof(char**));

    for(int i = 0; i < word_count ; i++)
    {
        word_list[i] = (char*)malloc(word_length * sizeof(char));
        order_types[0][i] = (char*)malloc(word_length * sizeof(char));
        order_types[1][i] = (char*)malloc(word_length * sizeof(char));
        order_types[2][i] = (char*)malloc(word_length * sizeof(char));
    }

    words = order_types[order];

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
        printf("%s\n", order_types[0][i]);
    }

    printf("\nlongest first order:\n===============\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", order_types[1][i]);
    }

    printf("\nmost common characters order:\n===============\n");
    for(int i = 0; i < word_count; i++)
    {
        printf("%s\n", order_types[2][i]);
    }

    //words = order_types[1];

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
        free(order_types[1][i]);
        free(order_types[0][i]);
        free(order_types[2][i]);
    }
    free(word_list);
    free(order_types[1]);
    free(order_types[0]);
    free(order_types[2]);
}
