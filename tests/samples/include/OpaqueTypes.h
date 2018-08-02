struct s;

extern struct s externVar; // removed. No warning printed

typedef struct undefinedIncludedStruct undefinedIncludedStruct;

void useUndefinedIncludedStruct(undefinedIncludedStruct);
