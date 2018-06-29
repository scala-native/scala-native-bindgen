#include "Extern.h"

int forty_two = 42;
const char version[] = "0.1.0";

enum mode mode = USER;

struct version semver_instance = {.major = 1, .minor = 2, .patch = 3};
struct version *semver = &semver_instance;