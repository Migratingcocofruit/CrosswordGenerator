#include "sort_words.h"

int main()
{
    width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
    
    int size = height * width * 2;

    word_length = fmax(height, width) + 1;

    char* buff = (char*)malloc(size * sizeof(char));

    read_words(buff, size);

    printf("%s\n", buff);

    count_words(buff, size);

    printf("word count: %d\n", word_count);

    word_list = (char**)malloc(word_count * sizeof(char**));
    for(int i = 0; i < word_count ; i++)
    {
        word_list[i] = (char*)malloc(word_length * sizeof(char));
    }

    split_words(buff, size);

    for(int i = 0; i< word_count; i++)
    {
        printf("%s\n", word_list[i]);
    }

    printf("\n");
    
    sort_words();

    for(int i = 0; i< word_count; i++)
    {
        printf("%s\n", word_list[i]);
    }

    free(buff);
    free(word_list);

    return 0;
}