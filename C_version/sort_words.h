#include <stdio.h>
#include <math.h>
#include <constants.h>

// Read words from file into buffer
void read_words(char* buff);
// Count words in word list for array size
int count_words(char* buff);
// Returns number of common letters between words. Multiple instances count
int common_letters(char* word1, char* word2);
// Sorts the words in the word list such that the sum of common words between each pair of words is maximal
void sort_words(char** words, int word_length, int word_count);
