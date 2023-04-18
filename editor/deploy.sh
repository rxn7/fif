#!/usr/bin/env bash

rm -rf dist/*
mkdir -p dist
cp ../build-emsc/editor/fifed.js dist/fifed.js
cp ../build-emsc/editor/fifed.wasm dist/fifed.wasm
cp ../build-emsc/editor/fifed.html dist/index.html

pushd dist
git add . -f
git commit -a -m "Update"
git push origin gh-pages 
popd
