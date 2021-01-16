#!/bin/bash 
echo "Install cmake and libpng"
brew install cmake
brew install libpng

BASEDIR="$PWD"
echo "Exporting CXX and LDFLAGS"
export CXXFLAGS="-nostdinc++ -isystem$BASEDIR/CppND-Route-Planning-Project/thirdparty/clang/include/c++/v1"
export LDFLAGS="$LDFLAGS -L/opt/X11/lib -L/opt/local/lib -L/usr/local/lib"
export LDFLAGS="-L$BASEDIR/CppND-Route-Planning-Project/thirdparty/clang/lib -Wl,-rpath,$BASEDIR/CppND-Route-Planning-Project/thirdparty/clang/lib"

cd CppND-Route-Planning-Project/thirdparty

echo "Download modified headers"
curl -O "https://releases.llvm.org/6.0.0/clang+llvm-6.0.0-x86_64-apple-darwin.tar.xz"
tar -xf clang+llvm-6.0.0-x86_64-apple-darwin.tar.xz
mv clang+llvm-6.0.0-x86_64-apple-darwin clang
rm clang+llvm-6.0.0-x86_64-apple-darwin.tar.xz

echo "Download / Install io2p"
cd P0267_RefImpl
mkdir Debug && cd Debug
cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" -DIO2D_DEFAULT=COREGRAPHICS_MAC ..
cmake --build .
sudo make install

echo "Build the main project"
cd "$BASEDIR/CppND-Route-Planning-Project"
mkdir build && cd build
cmake ..
make

echo "Test running the main project"
./OSM_A_star_search


