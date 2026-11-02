#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include "puzzle_globals.h"

#define FILE_PATH "E:\\Programs\\Repos\\CrosswordGenerator\\C_version\\words.txt"

// Read words from file into buffer
void read_words(char* buff, int size);
// Count words in word list for array size
void count_words(char* buff, int size);
// Splits and allocates the word array
void split_words(char* buff, int buff_size);
// Returns number of common letters between words. Multiple instances count
int common_letters(char* word1, char* word2);
// Sorts the words in the word list such that the sum of common words between each pair of words is maximal
void sort_words();
