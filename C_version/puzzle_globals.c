#include "puzzle_globals.h"
#include <stdio.h>

int word_count = DEFAULT_WIDTH, width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT, word_length = (DEFAULT_HEIGHT > DEFAULT_WIDTH ? DEFAULT_HEIGHT : DEFAULT_WIDTH) + 1;
char** words_heaviest_path = NULL, **words = NULL, **words_longest_first = NULL, **words_commons = NULL, **word_list = NULL;