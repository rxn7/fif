#!/usr/bin/env bash

mkdir -p dist
cp ../build-emsc/editor/fifed.js dist
cp ../build-emsc/editor/fifed.wasm dist
cp ../build-emsc/editor/fifed.html dist/index.html

pushd dist
npx gh-pages@3.0.0 --dist .
popd
