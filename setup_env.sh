mv Makefile Makefile.back
./autogen.sh
./configure --prefix=$(pwd)/bin
./configure --prefix=$(pwd)/bin
make
mv Makefile.back Makefile
