#!/usr/bin/env bash

# Bash strict mode
# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

if [[ ! -e bindgen/target/.llvm-version ]] || [[ "$(<bindgen/target/.llvm-version)" != "${LLVM_VERSION:-}" ]]; then
  rm -rf bindgen/target
  mkdir -p bindgen/target
  echo "${LLVM_VERSION:-}" > bindgen/target/.llvm-version
  (cd bindgen/target && cmake ..)
fi

make -C bindgen/target
sbt "${@:-test}"
