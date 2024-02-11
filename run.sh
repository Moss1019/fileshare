[ -x ./build ] || mkdir build

cd build

cmake ..
make && ./fileshare $1

cd ..