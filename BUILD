# bring in SDL2
sudo apt-get install libgl1-mesa-dev libsdl2-dev -y

# bring in bgfx
git submodule init
git submodule update

# build bgfx
cd extern/bgfx
make
make linux-debug64

# install nvidia tools
sudo apt-get install libnvtt-bin -y

# build the assets
tools/build.py