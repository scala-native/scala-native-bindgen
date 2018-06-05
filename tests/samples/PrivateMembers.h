typedef int __int32_t;
typedef __int32_t __darwin_pid_t;
typedef __darwin_pid_t pid_t; // should associate pid_t with int

typedef int __private_type;

struct structWithPrivateType {
    int field1;
    __private_type field2;
}; // will not be removed

union __unionWithPrivateName {
    int a;
}; // will not be removed

struct structWithPrivateStruct {
    struct structWithPrivateType *s;
}; // will not be removed

struct normalStruct {
    int a;
};

enum __privateEnum { A, B }; // will not be removed

enum enumWithPrivateMembers { __C, D }; // will not be removed

enum { __E, F }; // will not be removed

typedef struct {
    __private_type *a;
} privateStructWithTypedef, *privateStructWithTypedefPtr; // will not be removed

pid_t getTypeThatUsesPrivateTypes(); // will not be removed

// functions that should be removed:
void __privateFunction();

// functions that should not be removed:
__private_type *getPrivateType();
void usesPrivateUnion(union __unionWithPrivateName);
void usesPrivateStruct(struct structWithPrivateType *, struct normalStruct *);
void usesPrivateEnum(enum __privateEnum *);
