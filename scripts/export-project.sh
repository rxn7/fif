#!/usr/bin/env bash

# TODO: Convert this script to C++ and use it inside the editor.

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
pushd ${script_dir}/.. >/dev/null

project_dir=$1
runtime_build_path="build/release/runtime/fif-runtime"

function print_usage () {
	echo "Usage: ./export-project.sh <project_directory_path>"
}

if [[ $# -ne 1 ]]; then
	print_usage
	exit
fi

if [ ! -f "$runtime_build_path" ]; then
	echo "$runtime_build_path doesn't exist. Build the runtime first."
	exit
fi

if [ ! -d $project_dir ]; then
	echo "Project directory $PROJECT_DIR doesn't exist."
	print_usage
	exit
fi

if [ ! -f "$project_dir/project.fifproj" ]; then
	echo "File $PROJECT_DIR/project.fifproj doesn't exist!"
	print_usage
	exit
fi

export_dir="${project_dir}_export"
echo "Exporting $project_dir to $export_dir"

rm -rf $export_dir
mkdir -p $export_dir

cp $runtime_build_path $export_dir/$(basename ${project_dir})
cp -r ${project_dir}/* $export_dir
cp -r $(dirname $runtime_build_path)/assets $export_dir

# Export the default font
mkdir -p $export_dir/assets ; mkdir -p $export_dir/assets/fonts
cp fifed/assets/fonts/CozetteVector.ttf $export_dir/assets/fonts/CozetteVector.ttf

popd >/dev/null
