#include "word_count_array.h"
#include "bucket.h"
#include "word_entry.h"
#include "hash.h"
#include "word_count.h"

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
 * @brief Gibt alle Bucket-Ketten und das Array der Bucket-Zeiger frei. Gibt desweiterem 
 * die Worteinträge und die Array Struktur an sich frei.
 * @param array Erfolgreich initialisierte Tabelle; NULL ist erlaubt.
 *             Nach der Freigabe muss sie vor erneuter Nutzung initialisiert werden.
 */
void free_word_count_array(word_count_array *array) {
    if (!array) return;

    for (int i = 0; i < array->size; i++) {
        bucket *current = array->buckets[i];
        while (current) {
            bucket *next = current->next;
            free_word_entry(current->value);
            free(current);
            current = next;
        }
    }

    free(array->buckets);
    free(array);
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

/** @brief Zählt das am häufigsten vorkommende Wort in der Hashtabelle. Hierbei wird über ein Platzhalter immer das 
 * aktuell am häufigsten vorkommende Wort gespeichert und am Ende in die Ergebnisstruktur geschrieben. WIchtig hierbei
 * ist, dass die Ergebnisstruktur bereits initialisiert sein muss, da hier nur die Werte überschrieben werden.
 * @param array Initialisierte Hashtabelle mit positiver Groesse.
 * @param result Ergebnisstruktur, in die das am häufigsten vorkommende Wort und seine Häufigkeit gespeichert werden.
 * @return 0 bei Erfolg, -1 bei NULL-Argumenten.
 */
int count_most_used_word(word_count_array *array, analysis_result *result){
    if(!array || !result){
        return -1;
    }

    word_entry *current_most = NULL;
    

    for(int i = 0; i < array->size; i++){
        bucket *current_bucket = array->buckets[i];
        while(current_bucket != NULL){
            if(current_bucket->value != NULL){
                if(current_most == NULL || current_bucket->value->count > current_most->count){
                    current_most = current_bucket->value;
                }
            }
            current_bucket = current_bucket->next;
        }
    }   

    if(current_most){
        result->word_counts = current_most;
        result->word_count_total = current_most->count;
    } else {
        result->word_counts = NULL;
        result->word_count_total = 0;
        return -1; // No words found
    }
    return 0;
}




