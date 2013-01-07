#!/bin/bash
svn co https://sdlgfx.svn.sourceforge.net/svnroot/sdlgfx

cd sdlgfx
sh autogen.sh
./configure --prefix=$(dirname $(dirname $(dirname $(find /usr -name "SDL.h" 2>/dev/null))))
make
sudo make install
cd ..
