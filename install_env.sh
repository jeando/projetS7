#!/bin/bash
git clone https://github.com/erikd/libsndfile.git
cd libsndfile
sh autogen.sh
./configure
make
sudo make install
cd ..
git clone https://github.com/erikd/sndfile-tools.git
cd sndfile-tools
sh autogen.sh
./configure
make
sudo make install
cd ..

git clone git://repo.or.cz/openal-soft.git
cd openal-soft
cd build
cmake -D CMAKE_INSTALL_PREFIX=/usr -D CMAKE_BUILD_TYPE=Release ..
make
openal-info
sudo make install
cd ../..
