#include "word_cutting.h"
#include "word_entry.h"


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


/**
 * @brief Initialisiert eine neue word_cutting-Struktur. DIe beiden Zeiger auf die Worthälften werden auf NULL gesetzt und das cutting_flag wird auf 0 gesetzt. 
 * Entsprechend müssen die Einträge in den beiden Zeigern später noch initialisiert werden.
 * @return Zeiger auf die initialisierte word_cutting-Struktur oder NULL bei Fehler.
 */
word_cutting *word_cutting_initialize(){
    word_cutting *wc = malloc(sizeof(word_cutting));
    if(!wc) return NULL;

    wc->firsthalf = NULL;
    wc->secondhalf = NULL;
    wc->cutting_flag = 0;

    return wc;
}

/**
 * @brief Gibt den Speicher einer word_cutting-Struktur frei. Dabei werden auch die beiden Zeiger auf die Worthälften freigegeben, falls sie nicht NULL sind.
 * @param wc Zeiger auf die freizugebende word_cutting-Struktur.
 */
void word_cutting_free(word_cutting *wc){
    if(!wc) return;

    if(wc->firsthalf){
        free(wc->firsthalf);
    }
    if(wc->secondhalf){
        free(wc->secondhalf);
    }

    free(wc);
}