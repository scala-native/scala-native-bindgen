# Using Docker Container

This option requires [Docker].

Download docker image with the binary:

```sh
docker pull scalabindgen/scala-native-bindgen
```

Mount directories with required header files and run bindgen:

```sh
docker run -v "$(pwd)":/src -v /usr/include:/usr/include \
  --rm scalabindgen/scala-native-bindgen \
  relative/path/to/my_header.h --name my_header --
```

The docker image does not contain standard headers so it is important to
mount all system include directories that are used by the header file
passed to `scala-native-bindgen`. See the @github[docker-bindgen.sh](/scripts/docker-bindgen.sh) script for
how to wrap the dockerized program. The `$CWD` of the container is
`/src` which should be mounted from `$(pwd)` in case relative paths are
used.

Note, the `scalabindgen/scala-native-bindgen:latest` image is updated on
each merge to the `master` branch.

 [Docker]: https://www.docker.com/
 