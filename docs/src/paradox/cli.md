# Generating Bindings with the CLI

Statically linked executables are provided with each release for Linux and macOS. Head over to the [releases page] to download the latest version for your platform.

 [releases page]: https://github.com/kornilova-l/scala-native-bindgen/releases

@@@ note

In the following we assume you have renamed the downloaded `scala-native-bindgen-$PLATFORM` file to `scala-native-bindgen`.

@@@

## Command Line Usage

When generating bindings with the CLI, you need to specify the header file and provide the name of the created bindings using the `--name` option:

```sh
scala-native-bindgen --name fnmatch /usr/include/fnmatch.h --
```

When running the CLI it will also yield warnings along with the translation. To keep only the bindings please redirect the output to a file like this:

```sh
scala-native-bindgen --name fnmatch /usr/include/fnmatch.h -- > fnmatch.scala
```

By default it is assumed that you want to link with a library based on the name option.
In case the name of the library to link with is different from the binding name provide the library name using `--link`:

```sh
scala-native-bindgen --name zlib --link z /usr/include/zlib.h --
```

If the binding does not require any linking, pass `--no-link`:

```sh
scala-native-bindgen --name fnmatch --no-link /usr/include/fnmatch.h --
```

For libraries that require linking you m

## Options

The generated bindings can be configured using the different options and it is also possible to pass arguments directly to the Clang compiler using the `--extra-arg*` options.

| Option               | Description
|----------------------|---------------------------------------------------------------------------------|
| `--link`             | Library to link with, e.g. `--link uv` for libuv.
| `--no-link`          | Library does not require linking.
| `--name`             | Scala object name that contains bindings. Defaults to the library name.
| `--package`          | Package name of generated Scala file.
| `--exclude-prefix`   | Functions and unused typedefs will be removed if their names have the given prefix.
| `--binding-config`   | Path to a config file that contains the information about bindings that should be reused. See @ref:[Configuration](configuration.md) for more information.
| `--extra-arg`        | Additional argument to append to the compiler command line.
| `--extra-arg-before` | Additional argument to prepend to the compiler command line.
