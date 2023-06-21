#!/bin/sh
conan install . --output-folder cmake-build-release --build=missing
cmake . -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
