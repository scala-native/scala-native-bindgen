# Release Workflow

First build the `scala-native-bindgen` executable for both macOS and
Linux:

```sh
scripts/prepare-release.sh
```

You should now have `scala-native-bindgen-linux` and
`scala-native-bindgen-darwin` if you ran the script on a macOS machine.

Then release version `x.y.z` by running:

```sh
sbt -Dproject.version=x.y.z release
```

Finally, upload the `scala-native-bindgen-linux` and
`scala-native-bindgen-darwin` executables to the release page at:
<https://github.com/kornilova-l/scala-native-bindgen/releases/tag/vx.y.z>