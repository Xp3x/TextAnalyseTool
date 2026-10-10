#include "text_file.h"
#include "word_cutting.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check_stream(const char *text, size_t capacity,
                         const char *const *expected, size_t expected_count){
    FILE *file = tmpfile();
    assert(file);
    assert(fwrite(text, 1, strlen(text), file) == strlen(text));
    rewind(file);

    char buffer[8];
    text_file tf = {0};
    tf.file = file;
    tf.buffer = buffer;
    tf.buffer_capacity = capacity;
    analysis_result result = {0};
    word_cutting *wc = word_cutting_initialize();
    assert(wc);

    size_t found = 0;
    for(;;){
        assert(text_file_read(&tf, &result) == 0);
        word_entry *entry;
        while((entry = build_word(&tf, wc)) != NULL){
            assert(found < expected_count);
            assert(strcmp(entry->word, expected[found++]) == 0);
            assert(entry->count == 1);
            assert(wc->firsthalf == NULL && wc->cutting_flag == 0);
            free_word_entry(entry);
        }
        assert(tf.buffer_pos == tf.buffer_used);
        if(tf.eof_reached) break;
        assert((wc->firsthalf != NULL) == (wc->cutting_flag == 1));
    }
    assert(found == expected_count);
    assert(wc->firsthalf == NULL && wc->secondhalf == NULL);
    word_cutting_free(wc);
    fclose(file);
}

int main(void){
    const char *words[] = {"textanalyse", "hallo", "welt"};
    check_stream("Textanalyse hallo! welt", 3, words, 3);
    const char *single[] = {"hallo"};
    check_stream("hallo!", 3, single, 1); /* Voller letzter Block, dann leerer EOF-Block. */
    check_stream("hallo!", 8, single, 1);
    check_stream("hallo ", 3, single, 1); /* Whitespace am Blockende. */
    check_stream("hallo welt", 5, words + 1, 2); /* Whitespace am Blockanfang. */
    const char *joined[] = {"hallowelt"};
    check_stream("hallo!welt", 3, joined, 1);
    check_stream("!!! 123", 3, NULL, 0);
    check_stream("", 3, NULL, 0);

    char long_text[151];
    memset(long_text, 'a', 150);
    long_text[150] = '\0';
    char shortened[100];
    memset(shortened, 'a', 99);
    shortened[99] = '\0';
    const char *long_word[] = {shortened};
    check_stream(long_text, 3, long_word, 1);

    word_cutting *wc = word_cutting_initialize();
    assert(wc);
    char partial[] = "teil";
    text_file tf = {0};
    tf.buffer = partial;
    tf.buffer_used = strlen(partial);
    assert(build_word(&tf, wc) == NULL);
    assert(wc->firsthalf && wc->cutting_flag == 1);
    assert(build_word(&tf, wc) == NULL); /* Kein erneutes Anhaengen ohne neuen Block. */
    assert(strcmp(wc->firsthalf->word, "teil") == 0);
    word_cutting_free(wc); /* Auch ein unvollstaendiger Eintrag wird freigegeben. */

    assert(build_word(NULL, NULL) == NULL);
    puts("build_word tests passed");
    return 0;
}
