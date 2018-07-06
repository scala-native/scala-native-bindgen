# Limitations

There are multiple unsupported cases that should be considered when generating bindings:

 1. Currently bindgen does not support passing structs by value.  
    For example, it will not be possible to call these two functions from Scala Native code:
    
    ```c
    struct MyStruct {
        int a;
    };
    
    struct MyStruct returnStruct();
    
    void handleStruct(struct MyStruct mystr);
    ```
    To support such cases one should generate bindings for C wrapper functions that use pointers to structs instead of actual structs.
 2. `#define`s for literals and variables are supported. For other types of `#define`s,
    write wrapper functions that return defined values.
    
    ```c
    // Supported
    #define ESC 0x1b            /* Defines for numerical and string literals. */
    extern const int pi_const;
    #define PI pi_const         /* Defines aliasing extern variables. */

    // Not supported (non-exhaustive list)
    #define COLS      (getenv("COLS") ? atoi(getenv("COLS")) : 80)
    #define MAX(a, b) (a > b ? a : b)
    ```

 3. There is no way to reuse already generated bindings.  
    Bindgen outputs bindings also for headers that were included in a given header. See @github[#2](#2).
 4. Type qualifiers `const`, `volatile` and `restrict` are not supported.
 5. Extern variables are read-only. See @github[scala-native/scala-native#202](scala-native/scala-native#202).
