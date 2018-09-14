# Contributing Bindings

## Adding a new Bindings

To add a new binding, add a new project in `build.sbt` with the name of the artifact as well as a binding configuration for each header file and optionally the library to link with.

@@snip [build.sbt](../../../../build.sbt) { #sbt-binding-project }

It is also possible to generate multiple bindings per project. For most libraries a single binding should be sufficient.

@@snip [build.sbt](../../../../build.sbt) { #sbt-binding-project-multi-header }

The new sbt project should also be added to the `bindings` project's list of aggregated projects.

Optionally update the top-level `Dockerfile` to install the binding library's `-dev` package. 

@@snip [Dockerfile](../../../../Dockerfile) { #bindings-dev-package }

Remember to rebuild the development docker image if you change the `Dockerfile`.

```sh
docker-compose build ubuntu-18.04-llvm-6.0
```

Next, run `nativeBindgen` to generate the bindings. Note, to ensure consistency the bindings should be generated in the Linux docker environment.
This can be done by running:

```sh
docker-compose run --rm ubuntu-18.04-llvm-6.0 sbt lib$BINDING/nativeBindgen
```

Then write tests for the generated bindings. The test class should be in the `org.scalanative.bindgen.bindings.tests` package to make it easier to write examples for the binding documentation. Verify that the tests pass:

```sh
docker-compose run --rm ubuntu-18.04-llvm-6.0 sbt lib$BINDING/test
```

Finally, add a Markdown file in the `bindings` folder and document the binding, ideally with an example or two of how to use it. Remember to add the new file to the index in `bindings/index.md`.

## Update all Bindings

To update the bindings, build the Linux docker environment and run `bindings/nativeBindgen`:

```sh
docker-compose build ubuntu-18.04-llvm-6.0
docker-compose run --rm ubuntu-18.04-llvm-6.0 sbt bindings/nativeBindgen
```

Run the tests afterwards to verify that everything works:

```sh
docker-compose run --rm ubuntu-18.04-llvm-6.0 sbt bindings/test
```
