#include "word_table.h"
#include "bucket.h"
#include "hash.h"

#include <stdlib.h>
#include <string.h>

static const int WORD_TABLE_SIZE = 10000;

/**
 * @brief Reserviert eine Hashtabelle mit WORD_TABLE_SIZE leeren Buckets.
 * @param table Zu initialisierende Struktur; bestehende Inhalte werden nicht freigegeben.
 * @return 0 bei Erfolg, -1 bei NULL oder Speichermangel.
 */
int initialize_word_table(word_table *table) {
    if (!table) return -1;

    table->size = WORD_TABLE_SIZE;
    table->buckets = malloc(table->size * sizeof(bucket *));
    if (!table->buckets) return -1;

    for (int i = 0; i < table->size; i++) {
        table->buckets[i] = create_bucket(NULL);
        if (!table->buckets[i]) {
            for (int j = 0; j < i; j++) {
                free(table->buckets[j]);
            }
            free(table->buckets);
            return -1;
        }
    }

    return 0;
}

/**
 * @brief Gibt alle Bucket-Ketten, Worteintraege und das Bucket-Array frei.
 * @param table Erfolgreich initialisierte Tabelle; NULL ist erlaubt.
 *              Nach der Freigabe muss sie vor erneuter Nutzung initialisiert werden.
 */
void free_word_table(word_table *table) {
    if (!table || !table->buckets) return;

    for (int i = 0; i < table->size; i++) {
        bucket *current = table->buckets[i];
        while (current) {
            bucket *next = current->next;
            free_word_entry(current->value);
            free(current);
            current = next;
        }
    }

    free(table->buckets);
    table->buckets = NULL;
    table->size = 0;
}

/**
 * @brief Fuegt ein Wort ein oder erhoeht den Zaehler eines vorhandenen Wortes um 1.
 * Hashkollisionen werden durch verkettete Buckets behandelt. Neue Eintraege
 * werden als Zeiger gespeichert. Bei einem vorhandenen Wort wird der vorhandene
 * Zaehler erhoeht und der uebergebene Eintrag freigegeben.
 * @param table Initialisierte Hashtabelle mit positiver Groesse.
 * @param entry Einzufuegender Worteintrag mit nullterminiertem Wort.
 * @return 0 bei Erfolg, -1 bei NULL-Argumenten oder Speichermangel.
 */
int append_word_table(word_table *table, word_entry *entry) {
    if (!table || !table->buckets || table->size <= 0 || !entry) return -1;

    unsigned long index = hash(entry->word) % table->size;
    bucket *current = table->buckets[index];

    if (current->value == NULL) {
        current->value = entry;
        return 0;
    }

    while (current != NULL) {
        if (strcmp(current->value->word, entry->word) == 0) {
            current->value->count++;
            free_word_entry(entry);
            return 0;
        }
        if (current->next == NULL) {
            break;
        }
        current = current->next;
    }

    bucket *new_bucket = create_bucket(entry);
    if (!new_bucket) return -1;
    current->next = new_bucket;

    return 0;
}

/** @brief Sucht das am haeufigsten vorkommende Wort in der Hashtabelle.
 * Die Ergebnisstruktur muss bereits initialisiert sein, da hier nur die Werte
 * fuer das haeufigste Wort ueberschrieben werden.
 * @param table Initialisierte Hashtabelle mit positiver Groesse.
 * @param result Ergebnisstruktur fuer das haeufigste Wort und seine Haeufigkeit.
 * @return 0 bei Erfolg, -1 bei NULL-Argumenten.
 */
int find_most_used_word(word_table *table, analysis_result *result) {
    if (!table || !table->buckets || !result) {
        return -1;
    }

    word_entry *current_most = NULL;

    for (int i = 0; i < table->size; i++) {
        bucket *current_bucket = table->buckets[i];
        while (current_bucket != NULL) {
            if (current_bucket->value != NULL) {
                if (current_most == NULL || current_bucket->value->count > current_most->count) {
                    current_most = current_bucket->value;
                }
            }
            current_bucket = current_bucket->next;
        }
    }

    if (current_most) {
        result->most_used_word = current_most;
        result->most_used_word_count = current_most->count;
    } else {
        result->most_used_word = NULL;
        result->most_used_word_count = 0;
        return -1;
    }

    return 0;
}
