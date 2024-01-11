#!/usr/bin/env bash
# Count lines of code of each module 

shopt -s globstar

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

total=0
output=""

function count_loc_in_dir () {
	dir="$1"
	loc=$(cat $dir/**/*.cpp $dir/**/*.hpp 2> /dev/null | sed '/^\s*$/d' | wc -l)
	output="$output\n$dir:$loc\n"

	((total += loc))
}

function count_loc_in_module () {
	module_name="$1"
	loc=$(cat fif/src/$module_name/**/*.cpp fif/src/$module_name/**/*.hpp fif/include/fif/$module_name/**/*.hpp 2> /dev/null | sed '/^\s*$/d' | wc -l)
	output="$output\n$module_name:$loc\n"

	((total += loc))
}

for module in fif/src/*/; do
	count_loc_in_module $(basename $module)
done

count_loc_in_dir fifed

output="$output\ntotal:$total\n"
printf "$output" | column -s: -t

popd >/dev/null
