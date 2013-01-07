#!/bin/bash
git clone git://repo.or.cz/openal-soft.git
cd openal-soft
cd build
cmake -D CMAKE_INSTALL_PREFIX=/usr -D CMAKE_BUILD_TYPE=Release ..
make
openal-info
sudo make install
cd ../..
