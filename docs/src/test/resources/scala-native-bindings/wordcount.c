#include "wordcount.h"
#include <ctype.h>
#include <string.h>

int wordcount(struct wordcount *wordcount, FILE *file) {
    char buf[4096];

    // FIXME: This doesn't seem to have any effect
    // memset(wordcount, sizeof(*wordcount), 0);

    wordcount->chars = 0;
    wordcount->words = 0;
    wordcount->lines = 0;

    while (fgets(buf, sizeof(buf), file)) {
        char *pos;
        int in_word = 0;

        wordcount->lines++;
        wordcount->chars += strlen(buf);

        for (pos = buf; *pos; pos++) {
            if (isspace(*pos)) {
                if (in_word) {
                    wordcount->words++;
                }
                in_word = 0;
            } else {
                in_word = 1;
            }
        }

        if (in_word) {
            wordcount->words++;
        }
    }

    return 0;
}
