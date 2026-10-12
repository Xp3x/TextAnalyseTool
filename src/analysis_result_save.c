#include <stdio.h>

#include "analysis_result.h"

/**
 * @brief Speichert die Analyseergebnisse in einer Datei im Verzeichnis "results".
 *
 * @param result Zeiger auf die Analyseergebnisse.
 * @param filename Name der Ausgabedatei.
 * @return 0 bei Erfolg, -1 bei Fehler.
 */
int analysis_result_save(const analysis_result *result, const char *filename) {
    char filepath[512];

    if (!result || !filename || !*filename ||
        (result->word_pair_count_total > 0 && !result->word_pair_counts)) {
        return -1;
    }

    for (size_t i = 0; i < result->word_pair_count_total; i++) {
        if (!result->word_pair_counts[i].word1 ||
            !result->word_pair_counts[i].word2) {
            return -1;
        }
    }

    int length = snprintf(filepath, sizeof(filepath), "results/%s", filename);
    if (length < 0 || (size_t)length >= sizeof(filepath)) {
        return -1;
    }

    FILE *file = fopen(filepath, "w");

    if (!file) {
        perror("Datei konnte nicht geoeffnet werden");
        return -1;
    }

    fprintf(file, "Woerter: %zu\n", result->total_word_count);
    fprintf(file, "Zeichen (Bytes): %zu\n", result->char_count);
    fprintf(file, "Zeichen ohne Whitespace (Bytes): %zu\n",
            result->char_count_no_space);

    if (result->most_used_word) {
        fprintf(file, "Haeufigstes Wort: %s (%zu)\n",
                result->most_used_word->word,
                result->most_used_word_count);
    } else {
        fprintf(file, "Haeufigstes Wort: keines\n");
    }

    fprintf(file, "Wortpaare: %zu\n", result->word_pair_count_total);
    for (size_t i = 0; i < result->word_pair_count_total; i++) {
        const word_pair_count *pair = &result->word_pair_counts[i];
        fprintf(file, "%s %s (%zu)\n", pair->word1, pair->word2, pair->count);
    }

    int write_failed = ferror(file);
    if (fclose(file) == EOF) {
        perror("Fehler beim Schliessen der Datei");
        return -1;
    }

    return write_failed ? -1 : 0;
}
