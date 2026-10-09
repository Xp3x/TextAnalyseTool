#include "analisys_result.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Setzt alle Zaehler auf 0 und die Ergebniszeiger auf NULL.
 * @param result Zu initialisierende Struktur; vorhandene Daten werden nicht freigegeben.
 * @return 0 bei Erfolg, -1 bei einem NULL-Zeiger.
 */
int analysis_result_init(analysis_result *result) {
    if (!result) return -1;

    result->total_word_count = 0;
    result->char_count = 0;
    result->char_count_no_space = 0;

    result->word_counts = NULL;
    result->word_count_total = 0;

    result->word_pair_counts = NULL;
    result->word_pair_count_total = 0;

    return 0;
}

/**
 * @brief Gibt die Ergebnisstruktur frei, jedoch nicht ihre referenzierten Arrays.
 * @param result Dynamisch reservierte Struktur; NULL ist erlaubt.
 */
void free_analysis_result(analysis_result *result) {
    if (!result) return;
    free(result);
}

/**
 * @brief Reserviert und initialisiert eine leere Ergebnisstruktur.
 * @return Neue Struktur oder NULL bei einem Fehler; vom Aufrufer freizugeben.
 */
analysis_result *create_analysis_result(void) {
    analysis_result *result = malloc(sizeof *result);
    if (!result) return NULL;

    if (analysis_result_init(result) != 0) {
        free(result);
        return NULL;
    }

    return result;
}
