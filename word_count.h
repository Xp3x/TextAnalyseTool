#include <stddef.h>
#ifndef WORD_COUNT_H
#define WORD_COUNT_H

#include "word_entry.h"

struct text_file;


typedef struct {
    char *word;
    size_t count;

} word_count;

word_entry *create_word_entry(void);
word_entry *build_word(struct text_file *tf);
void free_word_entry(word_entry *entry);

#endif // WORD_COUNT_H

