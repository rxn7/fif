#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/../build/debug/fifed >/dev/null

gdb ./fifed 

popd >/dev/null
