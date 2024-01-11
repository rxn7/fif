#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

cmake --build build/debug && pushd build/debug/fifed >/dev/null ; ./fifed ; popd >/dev/null

popd >/dev/null
