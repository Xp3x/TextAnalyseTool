#ifndef WORD_COUNT_ARRAY_H
#define WORD_COUNT_ARRAY_H

#include "bucket.h"
#include "word_entry.h"
#include "analisys_result.h"

typedef struct {
    bucket **buckets;
    int size;
} word_count_array;

int initialize_word_count_array(word_count_array *array);
void free_word_count_array(word_count_array *array);
int append_array(word_count_array *array, word_entry *entry);
int count_most_used_word(word_count_array *array, analysis_result *result);



#endif // WORD_COUNT_ARRAY_H
