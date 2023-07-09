#!/bin/sh
set -e
# conan install . --output-folder cmake-build-release --build=missing
# mkdir -p Debug Release
conan install . --output-folder cmake-build-release --settings=build_type=Release
conan install . --output-folder cmake-build-debug --settings=build_type=Debug

cd cmake-build-release
cmake -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -S./..
cmake --build .

cd ./..

cd cmake-build-debug
cmake -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -S./..
cmake --build .



# cmake --build . --config Debug
