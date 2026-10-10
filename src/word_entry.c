#include "text_file.h"
#include "word_entry.h"
#include "word_cutting.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Erstellt einen Worteintrag mit leerem Wort und Zaehler 0.
 * @return Neuer Eintrag oder NULL bei Speichermangel; mit free_word_entry freigeben.
 */
word_entry *create_word_entry(void) {
    word_entry *entry = malloc(sizeof *entry);
    if (!entry) return NULL;

    entry->count = 0;
    entry->word[0] = '\0';

    return entry;
}

/**
 * @brief Sammelt ab der aktuellen Pufferposition Buchstaben zu einem Worteintrag.
 * Satzzeichen werden uebersprungen; Whitespace schliesst ein Wort ab.
 * Am Pufferende bleibt ein Wortteil in wc erhalten, bis Whitespace oder
 * eof_reached das Wort abschliesst. Es werden keine Dateibloecke eingelesen.
 * Woerter mit mehr als 99 Buchstaben werden auf 99 Buchstaben gekuerzt;
 * die restlichen Zeichen werden bis zum Wortende weiterhin konsumiert.
 * @param tf Textdatei mit gueltigem Puffer und aktueller Leseposition.
 * @param wc Einmal pro Textdatei initialisierter Zustand fuer alle Aufrufe.
 * @return Neuer Eintrag mit Zaehler 1 oder NULL bei ungueltigem Puffer,
 *         Speichermangel oder wenn kein Wort abgeschlossen wurde. Bei
 *         verbrauchtem Puffer ohne EOF muss der Aufrufer den naechsten Block
 *         einlesen und erneut aufrufen, auch wenn dieser Block leer ist.
 *         cutting_flag zeigt einen wartenden Wortteil an. Zurueckgegebene
 *         Eintraege gehoeren dem Aufrufer und sind nicht mehr in wc gespeichert.
 */
word_entry *build_word(text_file *tf, word_cutting *wc){
    if(!tf || !tf->buffer || !wc) return NULL;

    while(tf->buffer_pos < tf->buffer_used){
        unsigned char current = (unsigned char)tf->buffer[tf->buffer_pos];
        if(isalpha(current)){
            if(!wc->firsthalf){
                wc->firsthalf = create_word_entry();
                if(!wc->firsthalf) return NULL;
                wc->cutting_flag = 1;
            }
            size_t len = strlen(wc->firsthalf->word);
            if(len < sizeof(wc->firsthalf->word) - 1){
                wc->firsthalf->word[len] = (char)current;
                wc->firsthalf->word[len + 1] = '\0';
            }
        }
        tf->buffer_pos++;

        if(isspace(current) && wc->firsthalf){
            word_entry *entry = wc->firsthalf;
            wc->firsthalf = NULL;
            wc->cutting_flag = 0;
            entry->count = 1;
            return entry;
        }
    }

    if(tf->eof_reached && wc->firsthalf){
        word_entry *entry = wc->firsthalf;
        wc->firsthalf = NULL;
        wc->cutting_flag = 0;
        entry->count = 1;
        return entry;
    }
    return NULL;
}

/**
 * @brief Gibt den Speicher eines dynamisch reservierten Worteintrags frei.
 * @param entry Freizugebender Eintrag; NULL ist erlaubt.
 */
void free_word_entry(word_entry *entry) {
    if (!entry) return;
    free(entry);
}
