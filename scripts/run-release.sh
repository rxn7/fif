#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${SCRIPT_DIR}/.. >/dev/null

cmake --build build/release && pushd build/release/fifed ; ./fifed ; popd

popd
