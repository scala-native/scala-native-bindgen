#!/bin/bash
#
# Run tests similar to the BindgenSpec using the dockerized bindgen
# program.
#
# Assumes you have built the bindgen docker image by running:
#
#     docker-compose build bindgen

set -euo pipefail
IFS=$'\n\t'

outdir="tests/target/docker-samples"
rm -rf "$outdir"
mkdir -p "$outdir"

echo "Using version '${VERSION:-latest}'"

for input in tests/samples/*.h; do
  name="$(basename "$input" .h)"
  output="$outdir/$name.scala"
  expected="tests/samples/$name.scala"

  echo "Testing $name"
  scripts/docker-bindgen.sh "$input" \
    --name="$name" \
    --link=bindgentests \
    --package=org.scalanative.bindgen.samples \
    --exclude-prefix=__ \
    -- > "$output"
  git diff --exit-code "$expected" "$output"
done
