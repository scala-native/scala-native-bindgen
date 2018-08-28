#include <stdio.h>

struct wordcount {
    long chars;
    long lines;
    long words;
};

//#using-stdio-file
int wordcount(struct wordcount *wordcount, FILE *file);
//#using-stdio-file
