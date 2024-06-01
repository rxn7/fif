#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

COMMON_FLAGS="-DBUILD_SHARED_LIBS=FALSE -GNinja"

cmake fifed -Bfifed/build/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS $COMMON_FLAGS 
cmake fifed -Bfifed/build/release -DCMAKE_BUILD_TYPE=Release $COMMON_FLAGS 

cmake runtime -Bruntime/build/debug -DCMAKE_BUILD_TYPE=Debug $COMMON_FLAGS 
cmake runtime -Bruntime/build/release -DCMAKE_BUILD_TYPE=Release $COMMON_FLAGS 

# Mingw builds
if command -v x86_64-w64-mingw32-cmake &> /dev/null ; then
	echo "x86_64-w64-mingw32-cmake found. Windows build will be created"

	x86_64-w64-mingw32-cmake fifed -Bfifed/build/mingw/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE $COMMON_FLAGS 
	x86_64-w64-mingw32-cmake fifed -Bfifed/build/mingw/release -DCMAKE_BUILD_TYPE=Release -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE $COMMON_FLAGS 
	x86_64-w64-mingw32-cmake runtime -Bruntime/build/mingw -DCMAKE_BUILD_TYPE=Release -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE $COMMON_FLAGS 
else
	echo "!!!"
	echo "x86_64-w64-mingw32-cmake not found. Windows build will not be created!"
	sleep 3
fi

popd >/dev/null
