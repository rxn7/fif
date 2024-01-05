#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${SCRIPT_DIR}/.. >/dev/null

cmake --build build/debug && pushd build/debug/fifed >/dev/null ; ./fifed ; popd >/dev/null

popd >/dev/null
