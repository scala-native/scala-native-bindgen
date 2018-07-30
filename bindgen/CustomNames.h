struct page {
    char *content;
    struct page *nextStruct;
};

struct book {
    struct page *firstPage;
};

typedef int myInt;
