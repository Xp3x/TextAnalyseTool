#include "analysis_result.h"
#include "bucket.h"
#include "text_file.h"
#include "word_cutting.h"
#include "word_table.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Aufruf: %s <Textdatei>\n", argv[0]);
        return EXIT_FAILURE;
    }

    text_file tf = {0};
    analysis_result result;
    word_table table = {0};
    size_t word_count = 0;
    int status = EXIT_FAILURE;

    analysis_result_init(&result);
    if (text_file_open(&tf, argv[1]) != 0) {
        fprintf(stderr, "Datei konnte nicht geoeffnet werden: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    word_cutting *wc = word_cutting_initialize();
    if (!wc) {
        fprintf(stderr, "Nicht genug Speicher fuer die Wortverarbeitung.\n");
        goto close_file;
    }
    if (initialize_word_table(&table) != 0) {
        fprintf(stderr, "Nicht genug Speicher fuer die Worttabelle.\n");
        goto free_cutting;
    }

    for (;;) {
        if (text_file_read(&tf, &result) != 0) {
            fprintf(stderr, "Fehler beim Lesen der Datei.\n");
            goto free_table;
        }

        word_entry *entry;
        while ((entry = build_word(&tf, wc)) != NULL) {
            if (append_word_table(&table, entry) != 0) {
                free_word_entry(entry);
                fprintf(stderr, "Nicht genug Speicher fuer einen Worteintrag.\n");
                goto free_table;
            }
            word_count++;
        }
        if (tf.buffer_pos < tf.buffer_used) {
            fprintf(stderr, "Fehler beim Erstellen eines Wortes.\n");
            goto free_table;
        }
        if (tf.eof_reached) break;
    }

    /* Der blockweise Zaehler in text_file_read kann Wortteile doppelt zaehlen. */
    result.total_word_count = word_count;
    find_most_used_word(&table, &result);

    printf("Datei: %s\n", tf.filename);
    printf("Dateigroesse: %zu Bytes\n", tf.file_size);
    printf("Woerter: %zu\n", result.total_word_count);
    printf("Zeichen (Bytes): %zu\n", result.char_count);
    printf("Zeichen ohne Whitespace (Bytes): %zu\n", result.char_count_no_space);
    if (result.most_used_word) {
        printf("Haeufigstes Wort: %s (%zu)\n",
               result.most_used_word->word, result.most_used_word_count);
    } else {
        printf("Haeufigstes Wort: keines\n");
    }

    printf("\nWorthaeufigkeiten:\n");
    for (int i = 0; i < table.size; i++) {
        for (bucket *b = table.buckets[i]; b; b = b->next) {
            if (b->value) {
                printf("%-20s %zu\n", b->value->word, b->value->count);
            }
        }
    }
    if (analysis_result_save(&result, "analyse.txt") != 0) {
        fprintf(stderr, "Analyseergebnis konnte nicht gespeichert werden.\n");
        goto free_table;
    }
    printf("\nAnalyseergebnis gespeichert: results/analyse.txt\n");
    status = EXIT_SUCCESS;

free_table:
    free_word_table(&table);
free_cutting:
    word_cutting_free(wc);
close_file:
    fclose(tf.file);
    free(tf.buffer);
    free(tf.filename);
    return status;
}
