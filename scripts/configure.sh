#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${SCRIPT_DIR}/.. >/dev/null

cmake . -Bbuild/debug -GNinja -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=TRUE
cmake . -Bbuild/release -GNinja -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=TRUE

if command -v x86_64-w64-mingw32-cmake &> /dev/null ; then
	echo "x86_64-w64-mingw32-cmake found. Windows build will be created"
	x86_64-w64-mingw32-cmake . -Bbuild/mingw/debug -GNinja -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=TRUE -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE
	x86_64-w64-mingw32-cmake . -Bbuild/mingw/release -GNinja -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=TRUE -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE
else
	echo "x86_64-w64-mingw32-cmake not found. Windows build will not be created"
fi

if command -v bear &> /dev/null ; then
	read -r -p "bear found. Do you want to create compile_commands.json (it might take a few minutes) (y/n)" choice
	choice=${choice,,} # to lower

	if [[ $choice =~ ^(y| ) ]] || [[ -z $choice ]]; then
		bear -- cmake --build build/debug --clean-first
	fi
else
	echo "bear not found. compile_commands.json will not be created"
fi

popd >/dev/null
