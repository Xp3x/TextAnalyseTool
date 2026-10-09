#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "analisys_result.h"

typedef struct text_file {
    char *filename;         // Dateiname
    size_t file_size;       // Größe in Bytes
    
    FILE *file;             // FILE* Handle (optional, wenn stream-based)
    
    char *buffer;           // Buffer für eingelesene Daten
    size_t buffer_capacity; // Allokierter Puffer-Größe
    size_t buffer_used;     // Aktuell genutzte Bytes
    size_t buffer_pos;      // Nächste Leseposition
    
    bool eof_reached;       // EOF erreicht?
} text_file;

int text_file_open(text_file *tf, const char *filename);
int text_file_read(text_file *tf, analysis_result *result);
void text_file_normalized(text_file *tf);
int text_file_word_count(text_file *tf, analysis_result *result);
int text_file_char_count(text_file *tf, analysis_result *result);

#endif // TEXT_FILE_H

