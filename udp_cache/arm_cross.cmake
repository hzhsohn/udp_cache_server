SET(CROSS_COMPILE 1)

IF(CROSS_COMPILE) 
  
SET(CMAKE_SYSTEM_NAME Linux)
SET(TOOLCHAIN_DIR "/opt/FriendlyARM/toolschain/4.4.3")

set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-linux-g++)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/bin/arm-linux-gcc)
set(GNU_FLAGS "-mfpu=vfp -fPIC")
set(CMAKE_CXX_FLAGS "${GNU_FLAGS} ")
set(CMAKE_C_FLAGS "${GNU_FLAGS}  ")


SET(CMAKE_FIND_ROOT_PATH  ${TOOLCHAIN_DIR}
 ${TOOLCHAIN_DIR}/arm-none-linux-gnueabi/include
 ${TOOLCHAIN_DIR}/arm-none-linux-gnueabi/lib )

#include_directories(/usr/local/Trolltech/QtEmbedded-4.8.5-arm/include/)
#link_directories(/home/zchx/Downloads/boost_1_49_0_arm/stage/lib)
  
ENDIF(CROSS_COMPILE)
