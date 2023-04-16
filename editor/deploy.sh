#!/usr/bin/env bash

rm -rf dist/*
cp ../build-emsc/editor/fifed.js dist/
cp ../build-emsc/editor/fifed.wasm dist/
cp ../build-emsc/editor/fifed.html dist/index.html

pushd dist
git add .
git commit -a -m "Update"
git push origin gh-pages
popd
