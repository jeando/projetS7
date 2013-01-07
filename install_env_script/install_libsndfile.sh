#!/bin/bash
git clone https://github.com/erikd/libsndfile.git
cd libsndfile
sh autogen.sh
./configure
make
sudo make install
cd ..
