ARG UBUNTU_VERSION=18.04
FROM ubuntu:$UBUNTU_VERSION

RUN set -x \
 && apt update \
 && apt install -y apt-transport-https gnupg2 ca-certificates \
 && echo "deb https://dl.bintray.com/sbt/debian /" > /etc/apt/sources.list.d/sbt.list \
 && apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 2EE0EA64E40A89B84B2DF73499E82A75642AC823 \
 && apt update \
 && apt install -y curl build-essential openjdk-8-jdk-headless sbt cmake make \
 && rm -rf /var/lib/apt/lists/*

ARG LLVM_VERSION=6.0
ENV LLVM_VERSION=$LLVM_VERSION
# LLVM dev versions do not have a "-x.y" version suffix.
ARG LLVM_DEB_COMPONENT=-$LLVM_VERSION
RUN set -x \
 && curl https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
 && . /etc/lsb-release \
 && echo "deb https://apt.llvm.org/$DISTRIB_CODENAME/ llvm-toolchain-$DISTRIB_CODENAME$LLVM_DEB_COMPONENT main" > /etc/apt/sources.list.d/llvm.list \
 && apt update \
 && apt install -y clang-$LLVM_VERSION libclang-$LLVM_VERSION-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /src
