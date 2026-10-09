#include <stddef.h>
#ifndef WORD_PAIR_COUNT_H
#define WORD_PAIR_COUNT_H

typedef struct {
    char *word1;
    char *word2;
    size_t count;

} word_pair_count;

#endif // WORD_PAIR_COUNT_H