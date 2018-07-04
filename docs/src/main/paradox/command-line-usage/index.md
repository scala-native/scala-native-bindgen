# Command Line Usage

Calling the tool is pretty easy, you need to specify the file(s) and the name of the created bindings.

```sh
scala-native-bindgen --name uv /usr/include/uv.h --
```

Running the previous command wild also yield warnings along with the translation. To keep only the bindings please redirect the output to a file like this:

```sh
scala-native-bindgen --name uv /usr/include/uv.h -- > uv.scala
```

## Bindgen Options

| Option               | Description                                                                                                  |
|----------------------|--------------------------------------------------------------------------------------------------------------|
| `--link`             | Library to link with, e.g. `--link` uv for libuv.                                                            |
| `--no-link`          | Library is static and does not require linking.                                                              |
| `--name`             | Scala object name that contains bindings. If `--no-link` is specified then `name` should match library name. |
| `--package`          | Package name of generated Scala file.                                                                        |
| `--exclude-prefix`   | Functions and unused typedefs will be removed if their names have given prefix.                              |
| `--extra-arg`        | Additional argument to append to the compiler command line.                                                  |
| `--extra-arg-before` | Additional argument to prepend to the compiler command line.                                                 |
