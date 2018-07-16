# Limitations

There are multiple unsupported cases that should be considered when generating bindings.

## Passing structs by value

Scala Native does not support passing structs by value, bindgen skips such functions.
```c
struct MyStruct {
    int a;
};

struct MyStruct returnStruct(); // skipped

void handleStruct(struct MyStruct mystr); // skipped
```
To support such cases one should generate bindings for C wrapper functions that use pointers to structs instead of
actual structs.

## Limited support of `#define`s

`#define`s for literals and variables are supported. For other types of `#define`s,
write wrapper functions that return defined values.

```c
// Supported
#define ESC 0x1b            /* Defines for numerical and string literals. */
extern const int pi_const;
#define PI pi_const         /* Defines aliasing extern variables. */

// Not supported (non-exhaustive list)
#define COLS  (getenv("COLS") ? atoi(getenv("COLS")) : 80)
#define MAX(a, b) (a > b ? a : b)
```

## Reusing generated bindings

There is no way to reuse already generated bindings.  
Bindgen outputs bindings also for headers that were included in a given header. See @github[#2](#2).

## Type qualifiers

Type qualifiers `const`, `volatile` and `restrict` are not supported.

## Updating extern variables

Extern variables are read-only. See @github[scala-native/scala-native#202](scala-native/scala-native#202).
