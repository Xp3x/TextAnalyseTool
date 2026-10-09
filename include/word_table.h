#ifndef WORD_TABLE_H
#define WORD_TABLE_H

#include "word_entry.h"
#include "analysis_result.h"

typedef struct bucket bucket;

typedef struct {
    bucket **buckets;
    int size;
} word_table;

int initialize_word_table(word_table *table);
void free_word_table(word_table *table);
int append_word_table(word_table *table, word_entry *entry);
int find_most_used_word(word_table *table, analysis_result *result);

#endif // WORD_TABLE_H
