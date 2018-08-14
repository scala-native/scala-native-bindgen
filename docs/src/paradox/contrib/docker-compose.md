# Building the executable with `docker-compose`

You can use [docker-compose] to build and test the program:

```sh
# Build the docker image with LLVM version 6.0.
docker-compose build ubuntu-18.04-llvm-6.0
# Build the bindgen tool and run the tests.
docker-compose run --rm ubuntu-18.04-llvm-6.0 ./script/test.sh
# Run the bindgen tool inside the container.
docker-compose run --rm ubuntu-18.04-llvm-6.0 \
    bindgen/target/scala-native-bindgen --name union tests/samples/Union.h --
```

 [docker-compose]: https://docs.docker.com/compose/
