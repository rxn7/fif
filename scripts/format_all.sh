#!/usr/bin/env bash

shopt -s globstar

clang-format -i ./fif/src/**/*.cpp
clang-format -i ./fif/include/**/*.h

clang-format -i ./fifed/src/**/*.cpp
