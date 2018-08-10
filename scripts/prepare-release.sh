#!/usr/bin/env bash
#
# Build Linux and if possible macOS executables.

# Bash strict mode
# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

LINUX_EXEC="$ROOT/scala-native-bindgen-linux"
if [[ ! -e "$LINUX_EXEC" ]]; then
  docker-compose build bindgen
  container="$(docker container create "scalabindgen/scala-native-bindgen:${VERSION:-latest}")"
  docker cp "$container:/scala-native-bindgen" "$LINUX_EXEC"
  docker container rm "$container"
fi

MACOS_EXEC="$ROOT/scala-native-bindgen-darwin"
if [[ "$(uname -s)" = "Darwin" ]] && [[ ! -e "$MACOS_EXEC" ]]; then
  json_dir="$(ls /usr/local/Cellar/nlohmann_json/ | sort | tail -n 1)/include"
  rm -rf bindgen/target
  mkdir -p bindgen/target
  (cd bindgen/target && cmake -DSTATIC_LINKING=ON -DCMAKE_CXX_FLAGS="-isystem $json_dir" ..)
  make -C bindgen/target
  cp bindgen/target/scala-native-bindgen "$MACOS_EXEC"
fi
