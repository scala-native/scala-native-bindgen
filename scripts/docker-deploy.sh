#!/bin/bash
#
# Travis deploy script to push docker images from master and tags.

set -euo pipefail
IFS=$'\n\t'

IMAGE="scalabindgen/scala-native-bindgen:${1#v}"
echo "Pushing $IMAGE"

docker tag scalabindgen/scala-native-bindgen:latest "$IMAGE"

echo "$DOCKER_PASSWORD" | docker login --password-stdin --username "$DOCKER_USERNAME"

docker push "$IMAGE"
