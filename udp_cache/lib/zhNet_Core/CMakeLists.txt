message(-> zhNetCore库)

#增加其它编译选项
add_definitions("-lpthread")

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. ZHNETCORE_SRC_LIST)
#设置成静态库
ADD_LIBRARY(zhNetCore ${ZHNETCORE_SRC_LIST})
