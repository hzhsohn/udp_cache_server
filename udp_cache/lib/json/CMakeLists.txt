message(-> json)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. CJSON_T_SRC_LIST)
#设置成静态库
ADD_LIBRARY(json ${CJSON_T_SRC_LIST})