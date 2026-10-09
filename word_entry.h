#include <stddef.h>
#ifndef WORD_ENTRY_H
#define WORD_ENTRY_H

typedef struct {
    char word[100];
    size_t count;
} word_entry;

#endif // WORD_ENTRY_H