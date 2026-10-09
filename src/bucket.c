#include "bucket.h"
#include "word_entry.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Erstellt einen Bucket mit dem angegebenen Eintrag und ohne Nachfolger.
 * @param value Gespeicherter Eintragszeiger (keine Kopie); NULL ist erlaubt.
 * @return Dynamisch reservierter Bucket oder NULL bei Speichermangel.
 */
bucket *create_bucket(word_entry *value) {
    bucket *new_bucket = malloc(sizeof(bucket));
    if (!new_bucket) return NULL;

    new_bucket->value = value;
    new_bucket->next = NULL;

    return new_bucket;
}

/**
 * @brief Setzt den Eintrag eines Buckets und legt einen leeren Nachfolger an.
 * @param b Vorhandener Bucket; bisherige Inhalte werden nicht freigegeben.
 * @param entry Zu speichernder Eintragszeiger; darf nicht NULL sein.
 * @return 0 bei Erfolg, -1 bei ungueltigen Zeigern oder Speichermangel.
 */
int initialize_bucket(bucket *b, word_entry *entry){
    if(!b || !entry) return -1;

    bucket *next_bucket = create_bucket(NULL);
    if (!next_bucket) return -1;

    b->value = entry;
    b->next = next_bucket;

    return 0;
}

