## Build the dependencies

### install the premake build tool
sudo apt-get install premake4 -y

### install the python scripting language
sudo apt-get install python -y

### bring in SDL2
sudo apt-get install libgl1-mesa-dev libsdl2-dev -y

### bring in the other dependencies
git submodule init
git submodule update

### build bgfx
cd extern/bgfx
make
make linux-debug64
cd ../..

### build and prepare nanovg
cd extern/nanovg
premake4 gmake
cd build
make nanovg
cd ..
mkdir -p include/nanovg
cp src/*.h include/nanovg
cd ../..

## Build the assets

### install nvidia tools
sudo apt-get install libnvtt-bin -y

### build the assets
tools/build_assets.py