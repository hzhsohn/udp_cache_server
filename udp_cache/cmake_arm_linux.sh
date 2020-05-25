rm -rf build
mkdir build
chmod 777 build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE="./arm_cross.cmake" ..
make

