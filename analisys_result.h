#include <stddef.h>
#ifndef ANALISYS_RESULT_H
#define ANALISYS_RESULT_H
#include "word_count.h"
#include "word_pair_count.h"

typedef struct {
    size_t total_word_count;
    size_t char_count;
    size_t char_count_no_space;

    word_count *word_counts;
    size_t word_count_total;

    word_pair_count *word_pair_counts;
    size_t word_pair_count_total;

} analysis_result;

int analysis_result_init(analysis_result *result);
void free_analysis_result(analysis_result *result);
analysis_result *create_analysis_result(void);

#endif // ANALISYS_RESULT_H
