#!/usr/bin/env bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

COMMON_FLAGS="-DBUILD_SHARED_LIBS=FALSE -GNinja"

# Native builds
cmake . -Bbuild/debug -DCMAKE_BUILD_TYPE=Debug $COMMON_FLAGS 
cmake . -Bbuild/release -DCMAKE_BUILD_TYPE=Release $COMMON_FLAGS 

# Mingw builds
if command -v x86_64-w64-mingw32-cmake &> /dev/null ; then
	echo "x86_64-w64-mingw32-cmake found. Windows build will be created"
	x86_64-w64-mingw32-cmake . -Bbuild/mingw/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE $COMMON_FLAGS
	x86_64-w64-mingw32-cmake . -Bbuild/mingw/release -DCMAKE_BUILD_TYPE=Release -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE $COMMON_FLAGS
else
	echo "!!!"
	echo "x86_64-w64-mingw32-cmake not found. Windows build will not be created!"
	sleep 3
fi

if command -v bear &> /dev/null ; then
	read -r -p "bear found. Do you want to create compile_commands.json (it might take a few minutes) (y/N): " choice
	choice=${choice,,} # to lower

	if [[ $choice =~ ^(y| ) ]] || [[ -z $choice ]]; then
		bear -- cmake --build build/debug --clean-first
	fi
else
	echo "bear not found. compile_commands.json will not be created"
fi

popd >/dev/null
