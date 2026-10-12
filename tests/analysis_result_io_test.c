#include "analysis_result.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void clear_loaded(analysis_result *result) {
    free(result->most_used_word);
    for (size_t i = 0; i < result->word_pair_count_total; i++) {
        free(result->word_pair_counts[i].word1);
        free(result->word_pair_counts[i].word2);
    }
    free(result->word_pair_counts);
    assert(analysis_result_init(result) == 0);
}

int main(void) {
    const char *filename = "analysis_result_io_test.tmp";
    const char *path = "results/analysis_result_io_test.tmp";
    word_entry word = {"hallo", 3};
    word_pair_count pairs[] = {{"hallo", "welt", 2}, {"welt", "hallo", 1}};
    analysis_result original = {7, 42, 35, &word, 3, pairs, 2};
    analysis_result loaded = {0};
    assert(analysis_result_save(&original, filename) == 0);
    assert(analysis_result_load(&loaded, filename) == 0);
    assert(loaded.total_word_count == 7 && loaded.char_count == 42);
    assert(loaded.char_count_no_space == 35);
    assert(strcmp(loaded.most_used_word->word, "hallo") == 0);
    assert(loaded.most_used_word_count == 3 && loaded.most_used_word->count == 3);
    assert(loaded.word_pair_count_total == 2);
    for (size_t i = 0; i < 2; i++) {
        assert(strcmp(loaded.word_pair_counts[i].word1, pairs[i].word1) == 0);
        assert(strcmp(loaded.word_pair_counts[i].word2, pairs[i].word2) == 0);
        assert(loaded.word_pair_counts[i].count == pairs[i].count);
    }
    assert(analysis_result_load(&loaded, filename) == -1);
    clear_loaded(&loaded);

    analysis_result empty = {0};
    assert(analysis_result_save(&empty, filename) == 0);
    assert(analysis_result_load(&loaded, filename) == 0);
    assert(!loaded.most_used_word && !loaded.word_pair_counts);
    assert(loaded.word_pair_count_total == 0 && loaded.total_word_count == 0);

    const char *invalid[] = {
        "Woerter: -1\n",
        "Woerter: 999999999999999999999999999999\n",
        "Woerter: 7\nZeichen (Bytes): 42\nZeichen ohne Whitespace (Bytes): 35\n"
        "Haeufigstes Wort: hallo (3)\nWortpaare: 2\nhallo welt (2)\n",
        "Woerter: 0\nZeichen (Bytes): 0\nZeichen ohne Whitespace (Bytes): 0\n"
        "Haeufigstes Wort: keines\nWortpaare: 0\nUnerwartete Daten\n"
    };
    for (size_t i = 0; i < sizeof invalid / sizeof *invalid; i++) {
        FILE *file = fopen(path, "w");
        assert(file);
        assert(fputs(invalid[i], file) >= 0);
        assert(fclose(file) == 0);
        loaded.total_word_count = 123;
        assert(analysis_result_load(&loaded, filename) == -1);
        assert(loaded.total_word_count == 123);
        assert(!loaded.most_used_word && !loaded.word_pair_counts);
    }
    assert(analysis_result_load(NULL, filename) == -1);
    assert(analysis_result_load(&loaded, "") == -1);
    assert(remove(path) == 0);
    puts("analysis_result IO tests passed");
    return 0;
}
