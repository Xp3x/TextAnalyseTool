#ifndef BUCKET_H
#define BUCKET_H

#include "word_entry.h"

typedef struct bucket {
    word_entry *value;
    struct bucket *next;
} bucket;

bucket *create_bucket(word_entry *value);
int initialize_bucket(bucket *b, word_entry *entry);



#endif
