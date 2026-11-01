#include "sort_words.h"

int main()
{
    int len = 15, width = 15;
    
    int size = len * width * 2;

    int wordlen = max(len, width) + 1;

    char* buff = (char*)malloc(size * sizeof(char));

    read_words(buff, size);

    printf("%s\n", buff);

    int word_count = count_words(buff, size);

    printf("word count: %d\n", word_count);

    char** words = (char**)malloc(word_count * sizeof(char**));
    for(int i = 0; i < word_count ; i++)
    {
        words[i] = (char*)malloc(wordlen * sizeof(char));
    }

    split_words(words, buff, size, wordlen);

    for(int i = 0; i< word_count; i++)
    {
        printf("%s\n", words[i]);
    }

    printf("\n");
    
    sort_words(words, wordlen, word_count);

    for(int i = 0; i< word_count; i++)
    {
        printf("%s\n", words[i]);
    }

    free(buff);
    free(words);

    return 0;
}