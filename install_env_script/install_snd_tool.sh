#!/bin/bash
git clone https://github.com/erikd/sndfile-tools.git
cd sndfile-tools
sh autogen.sh
./configure
make
sudo make install
cd ..
