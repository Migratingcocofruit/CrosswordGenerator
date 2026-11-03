#include <stdio.h>
#include "generate_puzzle.h"
#include "sort_words.h"

int main(int argc, char* argv[])
{
    char path[256];
    memcpy(path, argc > 1 ? argv[1] : FILE_PATH, 256);

    if(argc > 2)
    {
        if(atoi(argv[2]) < 1)
        {
            fprintf(stderr, "invalid width set(%d), defaulting to %d", atoi(argv[2]), DEFAULT_WIDTH);
        }
        else
        {
            width = atoi(argv[2]);
        }
    }

    if(argc > 3)
    {   
        if(atoi(argv[3]) < 1)
        {
            fprintf(stderr, "invalid height set(%d), defaulting to %d", atoi(argv[3]), DEFAULT_HEIGHT);
        }
        else
        {
            height = atoi(argv[3]);
        }
    }

    int size = height * width * 2;

    char* buff = (char*)malloc(size * sizeof(char));

    int order = 0;

    gen_result* res[ORDER_TYPE_COUNT];

    read_words(buff, size, path);

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

    puzzle_gen_wrapper(res, true);

    for(int i = 0; i < 4; i++)
    {
        printf("\n");

        if(res[i])
        {
            print_board(res[i]);
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
