#include "hash.h"

/**
 * @brief Berechnet den djb2-Hashwert einer Zeichenkette.
 * @param str Nullterminierte Zeichenkette; darf nicht NULL sein.
 * @return Hashwert, der zur Bestimmung eines Tabellenindex verwendet werden kann.
 */
unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}
