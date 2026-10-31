#include<sort_words.h>

int main()
{
    int len = 8, width = 8;
    char buff[len * width * 2];

    read_words(buff);

    printf(buff);

    return 0;
}