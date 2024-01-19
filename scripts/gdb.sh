#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/../fifed/build/debug/ >/dev/null

gdb -ex run ./fifed 

popd >/dev/null
