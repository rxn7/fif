#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${SCRIPT_DIR}/.. >/dev/null

cmake --build build/mingw/debug && pushd build/mingw/debug/fifed >/dev/null ; wine ./fifed.exe ; popd >/dev/null
popd >/dev/null
