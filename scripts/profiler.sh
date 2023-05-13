#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${SCRIPT_DIR}/../build/release/fifed >/dev/null
valgrind --tool=callgrind ./fifed
qcachegrind 
popd
