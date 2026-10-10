#ifndef WORD_CUTTING_H
#define WORD_CUTTING_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "word_entry.h"

typedef struct word_cutting{
    word_entry *firsthalf; // Laufender Eintrag; bleibt zwischen Aufrufen erhalten.
    word_entry *secondhalf; // Von build_word nicht verwendet.
    size_t cutting_flag; //0 for no cutting, 1 for cutting
} word_cutting;

word_cutting *word_cutting_initialize();
void word_cutting_free(word_cutting *wc);

#endif // WORD_CUTTING_H
