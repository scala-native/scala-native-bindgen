#!/usr/bin/env bash

# Bash strict mode
# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

if [[ ! -e target/.llvm-version ]] || [[ "$(<target/.llvm-version)" != "${LLVM_VERSION:-}" ]]; then
  rm -rf target
  mkdir -p target
  echo "${LLVM_VERSION:-}" > target/.llvm-version
  (cd target && cmake ..)
fi

make -C target

cd tests
sbt "${@:-test}"
