#include "word_count_array.h"
#include "bucket.h"
#include "word_entry.h"
#include "hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

size_t size_array = 10000;

/**
 * @brief Reserviert eine Hashtabelle mit size_array leeren Buckets.
 * @param array Zu initialisierende Struktur; bestehende Inhalte werden nicht freigegeben.
 * @return 0 bei Erfolg, -1 bei NULL oder Speichermangel.
 */
int initialize_word_count_array(word_count_array *array) {
    if (!array) return -1;

    array->size = size_array;
    array->buckets = malloc(array->size * sizeof(bucket *));
    if (!array->buckets) return -1;

    for (int i = 0; i < array->size; i++) {
        array->buckets[i] = create_bucket(NULL);
        if (!array->buckets[i]) {
            for (int j = 0; j < i; j++) {
                free(array->buckets[j]);
            }
            free(array->buckets);
            return -1;
        }
    }

    return 0;
}

/**
 * @brief Gibt alle Bucket-Ketten und das Array der Bucket-Zeiger frei.
 * Die Worteintraege und die uebergebene Struktur selbst werden nicht freigegeben.
 * @param array Erfolgreich initialisierte Tabelle; NULL ist erlaubt.
 *             Nach der Freigabe muss sie vor erneuter Nutzung initialisiert werden.
 */
void free_word_count_array(word_count_array *array) {
    if (!array) return;

    for (int i = 0; i < array->size; i++) {
        bucket *current = array->buckets[i];
        while (current) {
            bucket *next = current->next;
            free(current);
            current = next;
        }
    }

    free(array->buckets);
}

/**
 * @brief Fuegt ein Wort ein oder erhoeht den Zaehler eines vorhandenen Wortes um 1.
 * Hashkollisionen werden durch verkettete Buckets behandelt. Neue Eintraege
 * werden als Zeiger gespeichert. Bei einem vorhandenen Wort wird der uebergebene
 * Eintrag weder gespeichert noch freigegeben; sein Zaehler wird nicht addiert.
 * @param array Initialisierte Hashtabelle mit positiver Groesse.
 * @param entry Einzufuegender Worteintrag mit nullterminiertem Wort.
 * @return 0 bei Erfolg, -1 bei NULL-Argumenten oder Speichermangel.
 */
int append_array(word_count_array *array, word_entry *entry) {
    if (!array || !entry) return -1;

    unsigned long index = hash(entry->word) % array->size;
    bucket *current = array->buckets[index];

    if (current->value == NULL) {
        current->value = entry;
        return 0;
    }

    while (current != NULL) {
        if (strcmp(current->value->word, entry->word) == 0) {
            current->value->count++;
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




