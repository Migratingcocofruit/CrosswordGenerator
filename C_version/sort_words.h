#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include "puzzle_globals.h"

#define FILE_PATH "E:\\Programs\\Repos\\CrosswordGenerator\\C_version\\words.txt"

// Read words from file at path into buffer
void read_words(char* buff, int size, char* path);
// Count comma separated words in buffer
void count_words(char* buff, int size);
// Splits the buffer to words and writes them to the word list array
void split_words(char* buff, int buff_size);
// Returns number of common letters between words. Multiple instances count
int common_letters(char* word1, char* word2);
// Sorts the words in the word list such that the multiple of common letters between each adjacent pair is maximal
void sort_words();
// Sorts words in descending order of most common letters with other words
void sort_words_commons(int** adjacency);
