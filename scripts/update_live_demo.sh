#!/usr/bin/env bash

scripts/configure.sh
cmake --build build/emscripten
mkdir -p dist

cp build/emscripten/fifed/fifed.js dist
cp build/emscripten/fifed/fifed.wasm dist
cp build/emscripten/fifed/fifed.html dist/index.html

pushd dist
npx gh-pages@3.0.0 --dist .
popd

rm -rf dist
