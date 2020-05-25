#ifndef _CK_SOURCE_LIB_ASSIST_FUNCTION_

#include <stdio.h>
#include <time.h>
#include <memory.h>

#ifdef __cplusplus
extern "C"{
#endif

//time_t 转 "年-月-日 时:分:秒"
char* trLongTimeToStr(time_t tme,char*dstString);
//time_t 转 "年月日时分秒"
char* trLongTimeToStrNoSplit(time_t tme,char*dstString);
//time_t 转 "年-月-日"
char* trLongTimeToStrDate(time_t tme,char*dstString);
//time_t 转 "年月日"
char* trLongTimeToStrDateNoSplit(time_t tme,char*dstString);
//time_t 转 "时:分:秒"
char* trShortTimeToStr(time_t tme,char*dstString);
//"年-月-日 时:分:秒" 转 time_t
time_t trStrToLongTime(char*szTime);
//"年-月-日" 转 time_t
time_t trStrDataToLongTime(char*szTime);
//"时:分:秒" 转 time_t
time_t trStrToShortTime(char*szTime);
//获取星期几  0星期日 1-6星期一到六
char getWeekDay();


///////////////////////////////
//去除字符串空格
char *ltrim( char *str );    

/**去除右边空格**/
char *rtrim( char *str);

/**去除两边空格**/
char *trim(char *str);

#ifdef __cplusplus
}
#endif
#define _CK_SOURCE_LIB_ASSIST_FUNCTION_
#endif
