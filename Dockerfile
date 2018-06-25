ARG UBUNTU_VERSION=18.04
FROM ubuntu:$UBUNTU_VERSION

RUN set -x \
 && : Remove pre-bundled libunwind \
 && find /usr -name "*libunwind*" -delete \
 && apt update \
 && apt install -y --no-install-recommends apt-transport-https gnupg2 ca-certificates \
 && echo "deb https://dl.bintray.com/sbt/debian /" > /etc/apt/sources.list.d/sbt.list \
 && apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 2EE0EA64E40A89B84B2DF73499E82A75642AC823 \
 && apt update \
 && apt install -y --no-install-recommends \
            locales locales-all \
            g++ openjdk-8-jdk-headless sbt cmake make curl git \
            zlib1g-dev \
            libgc-dev libunwind8-dev libre2-dev \
            nlohmann-json-dev \
 && rm -rf /var/lib/apt/lists/*

ARG LOCALE=en_US.UTF-8
ENV LANG=$LOCALE
ENV LANGUAGE=$LOCALE
ENV LC_ALL=$LOCALE

ARG LLVM_VERSION=6.0
ENV LLVM_VERSION=$LLVM_VERSION
RUN set -x \
 && apt update \
 && apt install -y --no-install-recommends \
            clang-$LLVM_VERSION clang-format-$LLVM_VERSION \
            libclang-$LLVM_VERSION-dev llvm-$LLVM_VERSION-dev \
 && rm -rf /var/lib/apt/lists/*

ENV PATH=$PATH:/usr/lib/llvm-$LLVM_VERSION/bin

##bindings-dev-package
# Install binding dependencies
RUN set -x \
 && apt update \
 && apt install -y --no-install-recommends \
	libutf8proc-dev \
 && rm -rf /var/lib/apt/lists/*
##bindings-dev-package

WORKDIR /src
