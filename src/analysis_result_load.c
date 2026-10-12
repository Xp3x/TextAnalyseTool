#include "analysis_result.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Laedt die von analysis_result_save gespeicherte Datei aus "results".
 * @param result Initialisierte, leere Zielstruktur; bleibt bei Fehlern unveraendert.
 * @return 0 bei Erfolg, -1 bei Datei-, Format- oder Speicherfehlern.
 * @note Geladene Woerter und das Wortpaararray muss der Aufrufer mit free freigeben.
 */
int analysis_result_load(analysis_result *result, const char *filename) {
    if (!result || !filename || !*filename || result->most_used_word ||
        result->word_pair_counts || result->word_pair_count_total) return -1;

    char filepath[512];
    int length = snprintf(filepath, sizeof(filepath), "results/%s", filename);
    if (length < 0 || (size_t)length >= sizeof(filepath)) return -1;

    FILE *file = fopen(filepath, "r");
    if (!file) return -1;

    analysis_result loaded = {0};
    if (fscanf(file, "Woerter: %zu\n", &loaded.total_word_count) != 1 ||
        fscanf(file, "Zeichen (Bytes): %zu\n", &loaded.char_count) != 1 ||
        fscanf(file, "Zeichen ohne Whitespace (Bytes): %zu\n",
               &loaded.char_count_no_space) != 1) goto fail;

    char line[160], word[100];
    int end = 0;
    if (!fgets(line, sizeof(line), file)) goto fail;
    if (strcmp(line, "Haeufigstes Wort: keines\n") != 0) {
        if (sscanf(line, "Haeufigstes Wort: %99s (%zu)%n", word,
                   &loaded.most_used_word_count, &end) != 2 ||
            end == 0 || strcmp(line + end, "\n") != 0) goto fail;
        loaded.most_used_word = malloc(sizeof *loaded.most_used_word);
        if (!loaded.most_used_word) goto fail;
        strcpy(loaded.most_used_word->word, word);
        loaded.most_used_word->count = loaded.most_used_word_count;
    }

    size_t pair_total;
    if (fscanf(file, "Wortpaare: %zu\n", &pair_total) != 1) goto fail;
    if (pair_total) {
        loaded.word_pair_counts = calloc(pair_total, sizeof *loaded.word_pair_counts);
        if (!loaded.word_pair_counts) goto fail;
    }
    for (size_t i = 0; i < pair_total; i++) {
        word_pair_count *pair = &loaded.word_pair_counts[i];
        char word1[100], word2[100], closing[2];
        if (fscanf(file, "%99s %99s (%zu%1[)]\n", word1, word2,
                   &pair->count, closing) != 4) goto fail;

        /* Wortlaenge entspricht word_entry.word (maximal 99 Zeichen). */
        loaded.word_pair_count_total = i + 1;
        pair->word1 = malloc(strlen(word1) + 1);
        pair->word2 = malloc(strlen(word2) + 1);
        if (!pair->word1 || !pair->word2) goto fail;
        strcpy(pair->word1, word1);
        strcpy(pair->word2, word2);
    }
    if (fgetc(file) != EOF || ferror(file)) goto fail;

    int close_failed = fclose(file) == EOF;
    file = NULL;
    if (close_failed) goto fail;
    *result = loaded;
    return 0;

fail:
    if (file) fclose(file);
    free(loaded.most_used_word);
    for (size_t i = 0; i < loaded.word_pair_count_total; i++) {
        free(loaded.word_pair_counts[i].word1);
        free(loaded.word_pair_counts[i].word2);
    }
    free(loaded.word_pair_counts);
    return -1;
}
