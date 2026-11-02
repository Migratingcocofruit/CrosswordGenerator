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

    order_types = (char***)malloc(sizeof(char**) * 4);

    word_list = (char**)malloc(word_count * sizeof(char**));
    order_types[3] = (char**)malloc(word_count * sizeof(char**));
    order_types[1] = (char**)malloc(word_count * sizeof(char**));
    order_types[2] = (char**)malloc(word_count * sizeof(char**));

    for(int i = 0; i < word_count ; i++)
    {
        word_list[i] = (char*)malloc(word_length * sizeof(char));
        order_types[1][i] = (char*)malloc(word_length * sizeof(char));
        order_types[2][i] = (char*)malloc(word_length * sizeof(char));
        order_types[3][i] = (char*)malloc(word_length * sizeof(char));
    }

    order_types[0] = word_list;
    
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
            printf("%s\n", order_types[1][i]);
        }

        printf("\nlongest first order:\n===============\n");
        for(int i = 0; i < word_count; i++)
        {
            printf("%s\n", order_types[2][i]);
        }

        printf("\nmost common characters order:\n===============\n");
        for(int i = 0; i < word_count; i++)
        {
            printf("%s\n", order_types[3][i]);
        }

    for(int i = 0; i < 4; i++)
    {
        words = order_types[i];

        gen_result* res = puzzle_gen_wrapper(1);

        printf("\n");

        if(res)
        {
            print_board(res);
        }
        else
        {
            printf("no board found\n");
        }
    }
    
    free(buff);
    for(int i = 0; i < word_count; i++)
    {
        free(word_list[i]);
        free(order_types[2][i]);
        free(order_types[1][i]);
        free(order_types[3][i]);
    }
    free(word_list);
    free(order_types[2]);
    free(order_types[1]);
    free(order_types[3]);
}
