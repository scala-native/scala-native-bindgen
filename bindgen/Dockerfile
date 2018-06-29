ARG UBUNTU_VERSION=18.04
ARG LLVM_VERSION=6.0
ARG BUILD_TAG=ubuntu-$UBUNTU_VERSION-llvm-$LLVM_VERSION
FROM scalabindgen/scala-native-bindgen-builder:$BUILD_TAG as builder

WORKDIR /src
COPY . /src
RUN set -x \
 && mkdir target \
 && cd target \
 && cmake -DSTATIC_LINKING=ON .. \
 && make

FROM scratch

COPY --from=builder /src/target/scala-native-bindgen /scala-native-bindgen
WORKDIR /src
ENTRYPOINT ["/scala-native-bindgen"]
