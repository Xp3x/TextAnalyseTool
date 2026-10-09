#include "text_file.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BLOCK_SIZE 4096;

/**
 * @brief Oeffnet eine Datei binaer zum Lesen und initialisiert ihren Lesepuffer.
 * Kopiert den Dateinamen und ermittelt die Dateigroesse. Bei Erfolg muss der
 * Aufrufer spaeter die Datei schliessen sowie Dateinamen und Puffer freigeben.
 * @param tf Zu initialisierende Struktur ohne noch belegte Ressourcen.
 * @param filename Nullterminierter Pfad zur Datei.
 * @return 0 bei Erfolg, -1 bei NULL-Argumenten, Oeffnungsfehler oder Speichermangel.
 */
int text_file_open(text_file *tf, const char *filename) {
    if (!tf || !filename) return -1;

    tf->filename = strdup(filename);
    if (!tf->filename) return -1;

    tf->file = fopen(filename, "rb");
    if (!tf->file) {
        free(tf->filename);
        return -1;
    }

    fseek(tf->file, 0, SEEK_END);
    tf->file_size = ftell(tf->file);
    fseek(tf->file, 0, SEEK_SET);

    tf->buffer_capacity = BLOCK_SIZE;
    tf->buffer = malloc(tf->buffer_capacity);
    if (!tf->buffer) {
        fclose(tf->file);
        free(tf->filename);
        return -1;
    }

    tf->buffer_used = 0;
    tf->buffer_pos = 0;
    tf->eof_reached = false;

    return 0;
}
/**
 * @brief Liest bei verbrauchtem Puffer hoechstens einen weiteren Dateiblock ein.
 * Wandelt den neuen Block in Kleinbuchstaben um und addiert seine Wort- und
 * Zeichenzahlen zum Ergebnis. Ein noch nicht verbrauchter Puffer bleibt unveraendert.
 * @param tf Geoeffnete und initialisierte Textdatei; eof_reached zeigt das Dateiende an.
 * @param result Initialisierte Ergebnisstruktur fuer die aufsummierten Zaehler.
 * @return 0 bei Erfolg, auch am Dateiende; -1 bei ungueltigen Argumenten
 *         oder einem erkannten Lesefehler.
 */
int text_file_read(text_file *tf, analysis_result *result) {
    if (!tf || !tf->file) return -1;
    if (!result) return -1;

    if (tf->buffer_pos >= tf->buffer_used && !tf->eof_reached) {
        size_t bytes_read = fread(tf->buffer, 1, tf->buffer_capacity, tf->file);
        tf->buffer_used = bytes_read;
        tf->buffer_pos = 0;

        
        text_file_normalized(tf);
        text_file_word_count(tf, result);
        text_file_char_count(tf, result);


        if (bytes_read < tf->buffer_capacity) {
            if (feof(tf->file)) {
                tf->eof_reached = true;
            } else {
                return -1; // Read error
            }
        }
    }

    return 0;
}

/**
 * @brief Wandelt die belegten Pufferbytes mit tolower in Kleinbuchstaben um.
 * @param tf Gueltige Textdateistruktur mit beschreibbarem Puffer.
 * @pre Die Pufferwerte muessen als Argumente fuer tolower gueltig sein.
 */
void text_file_normalized(text_file *tf){
    for(size_t i=0; i< tf->buffer_used;i++){
        tf->buffer[i] = tolower(tf->buffer[i]);
    }
    
}

/**
 * @brief Zaehlt durch Whitespace getrennte Woerter im gesamten belegten Puffer.
 * Addiert die Anzahl zu total_word_count. Wortteile an Blockgrenzen werden
 * getrennt gezaehlt, da kein Wortzustand zwischen Aufrufen erhalten bleibt.
 * @param tf Textdateistruktur mit dem auszuwertenden Puffer.
 * @param result Initialisierte Ergebnisstruktur fuer die Wortanzahl.
 * @return 0 bei Erfolg, -1 bei ungueltigen Zeigern.
 */
int text_file_word_count(text_file *tf, analysis_result *result){
    if(!tf || !tf->buffer) return -1;
    if(!result) return -1;
    int word_count = 0;
    int in_word = 0;

    for(size_t i=0; i< tf->buffer_used;i++){
        if(isspace((unsigned char)tf->buffer[i])){
            if(in_word){
                word_count++;
                in_word = 0;
            }
        }else{
            in_word = 1;
        }
    }

    if(in_word){
        word_count++;
    }

    result->total_word_count += word_count;
    

    return 0;

}

/**
 * @brief Addiert die Pufferlaenge und die Anzahl der Bytes ohne Whitespace.
 * Die Zaehler erfassen Bytes, nicht die Anzahl von Unicode-Zeichen.
 * @param tf Textdateistruktur mit dem auszuwertenden Puffer.
 * @param result Initialisierte Ergebnisstruktur fuer char_count und char_count_no_space.
 * @return 0 bei Erfolg, -1 bei ungueltigen Zeigern.
 */
int text_file_char_count(text_file *tf, analysis_result *result){
    if(!tf || !tf->buffer) return -1;
    if(!result) return -1;

    result->char_count += tf->buffer_used;

    for(size_t i=0; i< tf->buffer_used;i++){
        if(!isspace((unsigned char)tf->buffer[i])){
            result->char_count_no_space++;
        }
    }

    return 0;
}
