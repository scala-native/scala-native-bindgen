struct page {
    char *content;
    struct page *nextStruct;
};

struct book {
    struct page *firstPage;
};

union weight {
    int kilos;
    float grams;
};

typedef int myInt;
typedef enum { CONST } enumWithTypedef;
