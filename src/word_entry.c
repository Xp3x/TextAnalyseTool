#include "text_file.h"
#include "word_entry.h"

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
 * Nichtbuchstaben werden uebersprungen. Ein Eintrag wird zurueckgegeben, wenn
 * auf einen Buchstaben Whitespace oder das Pufferende folgt; buffer_pos wird
 * dabei weitergesetzt. Es werden keine weiteren Dateibloecke eingelesen.
 * @param tf Textdatei mit gueltigem Puffer und aktueller Leseposition.
 * @pre Der gesammelte Text umfasst hoechstens 99 Buchstaben.
 * @return Neuer Eintrag mit Zaehler 1 oder NULL bei ungueltigem Puffer,
 *         Speichermangel oder wenn kein Eintrag abgeschlossen wurde.
 *         Der Aufrufer ist fuer die Freigabe des neuen Eintrags verantwortlich.
 */
word_entry *build_word(text_file *tf){
    if(!tf || !tf->buffer) return NULL;
    char temp_word[100];
    temp_word[0] = '\0';

    while(tf->buffer_pos < tf->buffer_used){
        for(size_t i=tf->buffer_pos; i<tf->buffer_used; i++){
            if(isspace((unsigned char)tf->buffer[i]) == 0 && isalpha((unsigned char)tf->buffer[i]) != 0){
                size_t len = strlen(temp_word);
                temp_word[len] = tf->buffer[i];
                temp_word[len+1] = '\0';

                if(i + 1 == tf->buffer_used || isspace((unsigned char)tf->buffer[i + 1]) != 0){
                    word_entry *entry = create_word_entry();
                    if(!entry) return NULL;
                    strncpy(entry->word, temp_word, sizeof(entry->word) - 1);
                    entry->word[sizeof(entry->word) - 1] = '\0';
                    entry->count = 1;

                    temp_word[0] = '\0';

                    tf->buffer_pos = i + 1;

                    return entry;
                }
            }
        }
        tf->buffer_pos = tf->buffer_used;
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
