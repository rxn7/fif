#!/usr/bin/env bash

emcmake cmake . -Bbuild-emsc -GNinja
cmake --build build-emsc
mkdir -p dist

cp build-emsc/fifed/fifed.js dist
cp build-emsc/fifed/fifed.wasm dist
cp build-emsc/fifed/fifed.html dist/index.html

pushd dist
npx gh-pages@3.0.0 --dist .
popd

rm -rf dist
