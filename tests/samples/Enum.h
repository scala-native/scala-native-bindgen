enum days {
    MONDAY, // = 0
    TUESDAY = 200,
    WEDNESDAY, // = 201
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 3,
    SUNDAY // = 4
};

enum bigValues {
    BIG_A = 10000000000, // does not fit into int
    BIG_B = 1
};

enum { // anonymous enum
    ANON_A,
    ANON_B
};

enum negativeValues { NEG_A = -1, NEG_B = -2 };

enum bigNegativeValues { BIG_NEG_A = -10000000000, BIG_NEG_B = -1 };

enum days get_WEDNESDAY();

char *check_BIG_NEG_A(enum bigNegativeValues big_neg_a);
