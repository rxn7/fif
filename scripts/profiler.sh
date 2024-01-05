#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${SCRIPT_DIR}/../build/release/fifed >/dev/null

if ! command -v valgrind &> /dev/null ; then 
	echo "Valgrind is not installed!"
	exit
fi

if ! command -v gprof2dot &> /dev/null ; then 
	echo "gprof2dot is not installed!"
	exit
fi

if ! command -v dot &> /dev/null ; then 
	echo "dot is not installed!"
	exit
fi

valgrind --tool=callgrind --callgrind-out-file=callgrind-result.out ./fifed
gprof2dot --format=callgrind --output=gprof2dot-result.dot ./callgrind-result.out
dot -Tsvg ./gprof2dot-result.dot -o profiler-graph.svg

rm ./callgrind-result.out
rm ./gprof2dot-result.dot

xdg-open profiler-graph.svg

popd >/dev/null
