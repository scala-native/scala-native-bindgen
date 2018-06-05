enum days {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
};

typedef enum { OFF, ON } toggle_e;

typedef int (*int2int)(int);
typedef const char *(*day2string)(enum days);
typedef void (*toggle)(toggle_e state);