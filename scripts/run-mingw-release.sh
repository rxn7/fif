#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

cmake --build fifed/build/mingw/release 
pushd fifed/build/mingw/release >/dev/null 

if [[ $(grep -i Microsoft /proc/version) ]]; then
    echo "WSL detected, running exe directly"
    ./fifed.exe
else
    echo "Running via wine"
    wine ./fifed.exe
fi

popd >/dev/null
