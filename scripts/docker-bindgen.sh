#!/bin/bash
#
# Wrapper script for running the dockerized scala-native-bindgen binary.

set -euo pipefail
IFS=$'\n\t'

volumes=(
  "--volume=$(pwd):/src"
  "--volume=/usr/include:/usr/include"
)

if [[ "$#" -gt 1 ]] && [[ "$1" == /* ]]; then
  volumes+="--volume=$1:$1"  
fi

exec docker run --rm "${volumes[@]}" "scalabindgen/scala-native-bindgen:${VERSION:-latest}" "$@"
