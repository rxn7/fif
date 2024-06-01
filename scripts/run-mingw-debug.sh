#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

cmake --build fifed/build/mingw/debug && pushd fifed/build/mingw/debug >/dev/null ; wine ./fifed.exe ; popd >/dev/null

popd >/dev/null
