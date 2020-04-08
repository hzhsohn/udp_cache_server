message(-> src目录)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. BINROOT_SRC_LIST)
#设置成静态库
ADD_LIBRARY(src ${BINROOT_SRC_LIST})

