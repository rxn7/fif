#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

cmake --build fifed/build/debug && pushd fifed/build/debug >/dev/null ; ./fifed ; popd >/dev/null

popd >/dev/null
