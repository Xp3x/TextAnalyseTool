#include <stddef.h>
#ifndef WORD_ENTRY_H
#define WORD_ENTRY_H

struct text_file;

typedef struct {
    char word[100];
    size_t count;
} word_entry;

word_entry *create_word_entry(void);
word_entry *build_word(struct text_file *tf);
void free_word_entry(word_entry *entry);

#endif // WORD_ENTRY_H
