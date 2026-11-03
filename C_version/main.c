#include <stdio.h>
#include "generate_puzzle.h"
#include "sort_words.h"

int main()
{
    int size = height * width * 2;

    char* buff = (char*)malloc(size * sizeof(char));

    int order = 0;

    gen_result* res;

    read_words(buff, size);

    count_words(buff, size);

    order_types = (char***)malloc(sizeof(char**) * 4);
    for(int i = 0; i < ORDER_TYPE_COUNT; i++)
    {
        order_types[i] = (char**)malloc(word_count * sizeof(char**));
        for(int j = 0; j < word_count ; j++)
        {
            order_types[i][j] = (char*)malloc(word_length * sizeof(char));
        }
    }

    word_list = order_types[0];
    
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

        puzzle_gen_wrapper(&res ,false);

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
    for(int i = 0; i < ORDER_TYPE_COUNT; i++)
    {
        for(int j = 0; j < word_count; j++)
        {
            free(order_types[i][j]);
        }
        free(order_types[i]);
    }
}
