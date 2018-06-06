typedef int match;
typedef match var[5];

struct object {
  match yield;
  int val;
};

typedef struct object object;
typedef struct object type;

union lazy {
  object *instance;
  match forSome;
  struct {
    char def;
    type *super;
  } implicit;
};

typedef union lazy lazy;

type with(match sealed, var implicit, lazy forSome);

typedef match def;
typedef struct { def val; lazy finally; } finally;
match implicit(finally type[12]);
