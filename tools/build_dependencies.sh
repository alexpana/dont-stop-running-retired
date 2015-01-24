#!/bin/sh

echo
echo "------------------------------------------"
echo "Installing tools and dendencies"
echo "------------------------------------------"
echo
sudo apt-get update
sudo apt-get install python libgl1-mesa-dev libsdl2-dev premake4 libnvtt-bin -y

echo
echo "------------------------------------------"
echo "Initializing submodules"
echo "------------------------------------------"
echo
git submodule init
git submodule update

echo
echo "------------------------------------------"
echo "Building bgfx"
echo "------------------------------------------"
echo
cd extern/bgfx
make .build/projects/gmake-linux
make linux-debug64
cd ../..

echo
echo "------------------------------------------"
echo "Building nanovg"
echo "------------------------------------------"
echo
cd extern/nanovg
premake4 gmake
cd build
make nanovg
cd ..
mkdir -p include/nanovg
cp src/*.h include/nanovg
cd ../..

echo
echo "------------------------------------------"
echo "Building assets"
echo "------------------------------------------"
echo
python tools/build_assets.py