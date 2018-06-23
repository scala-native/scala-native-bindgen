ARG UBUNTU_VERSION=18.04
ARG LLVM_VERSION=6.0
ARG BUILD_TAG=ubuntu-$UBUNTU_VERSION-llvm-$LLVM_VERSION
FROM scalabindgen/scala-native-bindgen-builder:$BUILD_TAG as builder

WORKDIR /src
COPY . /src
RUN set -x \
 && mkdir target \
 && cd target \
 && cmake .. \
 && make

ARG UBUNTU_VERSION=18.04
FROM ubuntu:$UBUNTU_VERSION

ARG LLVM_VERSION=6.0
ENV LLVM_VERSION=$LLVM_VERSION
RUN set -x \
 && apt update \
 && apt install -y --no-install-recommends libclang1-$LLVM_VERSION \
 && rm -rf /var/lib/apt/lists/*

COPY --from=builder /src/target/scala-native-bindgen /usr/bin/scala-native-bindgen
WORKDIR /src
ENTRYPOINT ["scala-native-bindgen"]
