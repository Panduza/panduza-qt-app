#!/bin/bash

#rm -rf build
mkdir -p build
pushd build

cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/opt/qt/6.2.4/gcc_64/ ..
cmake --build . --parallel 6
popd
