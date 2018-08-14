# Integrating Bindings

To reuse already generated bindings create a `config.json` file that defines which headers correspond to which Scala objects:

```json
{
  "_stdio.h": "scala.scalanative.native.stdio",
  "/full/path/to/regexp4.h": "bindings.regexp4"
}
```

Bindgen assumes that type names in header files match type names in generated binding (spaces in struct, union and enum
names are replaces with underscores), but it is possible to specify custom names mapping:

```json
{
  "hiredis.h": {
    "object": "bindings.hiredis.hiredis",
    "names": {
      "redisContext": "Context"
    }
  }
}
```

Provide a path to `config.json` to bindgen using `--binding-config` command-line option or `NativeBinding.bindingConfig`
sbt plugin option (see @ref:[Using the sbt plugin](sbt.md)).
