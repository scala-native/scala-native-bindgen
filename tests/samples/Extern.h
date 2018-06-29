extern int forty_two;
extern const char version[];

enum mode { SYSTEM, USER };
extern enum mode mode;

struct version {
    int major;
    int minor;
    int patch;
};
extern struct version *semver;