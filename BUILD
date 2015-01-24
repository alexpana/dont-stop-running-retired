# bring in SDL2
sudo apt-get install libgl1-mesa-dev libsdl2-dev -y

# bring in bgfx
git submodule init
git submodule update

# build bgfx
cd extern/bgfx
make
make linux-debug64

# install the premake build tool
sudo apt-get install premake4 -y

# build and prepare nanovg
cd extern/nanovg
premake4 gmake
cd build
make nanovg
cd ..
mkdir -p include/nanovg
mv src/*.h include/nanovg

# install nvidia tools
sudo apt-get install libnvtt-bin -y

# build the assets
tools/build.py