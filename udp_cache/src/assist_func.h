#ifndef _CK_SOURCE_LIB_ASSIST_FUNCTION_

#include <stdio.h>
#include <time.h>
#include <memory.h>

#ifdef __cplusplus
extern "C"{
#endif

//time_t ת "��-��-�� ʱ:��:��"
char* trLongTimeToStr(time_t tme,char*dstString);
//time_t ת "������ʱ����"
char* trLongTimeToStrNoSplit(time_t tme,char*dstString);
//time_t ת "��-��-��"
char* trLongTimeToStrDate(time_t tme,char*dstString);
//time_t ת "������"
char* trLongTimeToStrDateNoSplit(time_t tme,char*dstString);
//time_t ת "ʱ:��:��"
char* trShortTimeToStr(time_t tme,char*dstString);
//"��-��-�� ʱ:��:��" ת time_t
time_t trStrToLongTime(char*szTime);
//"��-��-��" ת time_t
time_t trStrDataToLongTime(char*szTime);
//"ʱ:��:��" ת time_t
time_t trStrToShortTime(char*szTime);
//��ȡ���ڼ�  0������ 1-6����һ����
char getWeekDay();

char trStrToBool(const char *szValue);
char* trBoolToStr(char c);

#ifdef __cplusplus
}
#endif
#define _CK_SOURCE_LIB_ASSIST_FUNCTION_
#endif
