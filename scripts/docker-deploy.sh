#!/bin/bash
#
# Travis deploy script to push docker images from master and tags.

set -euo pipefail
IFS=$'\n\t'

SOURCE_IMAGE="scalabindgen/scala-native-bindgen:${1}"
TARGET_IMAGE="scalabindgen/scala-native-bindgen:${2#v}"
echo "Pushing $TARGET_IMAGE from $SOURCE_IMAGE"

if [[ "$SOURCE_IMAGE" != "$TARGET_IMAGE" ]]; then
  docker tag "$SOURCE_IMAGE" "$TARGET_IMAGE"
fi

echo "$DOCKER_PASSWORD" | docker login --password-stdin --username "$DOCKER_USERNAME"

docker push "$TARGET_IMAGE"
