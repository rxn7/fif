#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${SCRIPT_DIR}/.. >/dev/null

cmake . -Bbuild/debug -GNinja -DCMAKE_BUILD_TYPE=Debug
cmake . -Bbuild/release -GNinja -DCMAKE_BUILD_TYPE=Release

popd >/dev/null
