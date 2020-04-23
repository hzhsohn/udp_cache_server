#include "assist_func.h"
#include "platform.h"

#ifdef _WIN32
  #define localtime_r(a,b) localtime_s(b,a)
  #define asctime_r(a,b) asctime_s(b,a)
#else
  #define localtime_s(a,b) localtime_r(b,a)
  #define asctime_s(a,b) asctime_r(b,a)
#endif

// ------------------------------- 时间处理函数 ----------------------------------
//time_t 转 "年-月-日 时:分:秒"
char* trLongTimeToStr(time_t tme,char*dstString)
 {
	struct tm ptm;
	localtime_r(&tme,&ptm);

	sprintf(dstString, "%4d-%02d-%02d %02d:%02d:%02d", 
			(ptm.tm_year + 1900),
			ptm.tm_mon+1,
			ptm.tm_mday,
			ptm.tm_hour,
			ptm.tm_min,
			ptm.tm_sec);
	return dstString;
 }
//time_t 转 "年月日时分秒"
char* trLongTimeToStrNoSplit(time_t tme,char*dstString)
 {
	struct tm ptm;
	localtime_r(&tme,&ptm);

	sprintf(dstString, "%4d%02d%02d%02d%02d%02d", 
			(ptm.tm_year + 1900),
			ptm.tm_mon+1, 
			ptm.tm_mday, 
			ptm.tm_hour, 
			ptm.tm_min,ptm.tm_sec);
	return dstString;
 }
//time_t 转 "年-月-日"
char* trLongTimeToStrDate(time_t tme,char*dstString)
 {
	struct tm ptm;
	localtime_r(&tme,&ptm);

	sprintf(dstString, "%4d-%02d-%02d",
			(ptm.tm_year + 1900),
			ptm.tm_mon+1, 
			ptm.tm_mday);
	return dstString;
 }
//time_t 转 "年月日"
char* trLongTimeToStrDateNoSplit(time_t tme,char*dstString)
{
	struct tm ptm;
	localtime_r(&tme,&ptm);

	sprintf(dstString, "%4d%02d%02d", 
			(ptm.tm_year + 1900),
			ptm.tm_mon+1, 
			ptm.tm_mday);
	return dstString;
}
//time_t 转 "时:分:秒"
char* trShortTimeToStr(time_t tme,char*dstString)
 {
	struct tm ptm;
	localtime_r(&tme,&ptm);

	sprintf(dstString, "%02d:%02d:%02d",
			ptm.tm_hour,
			ptm.tm_min,
			ptm.tm_sec);
	return dstString;
 }

//"年-月-日 时:分:秒" 转 time_t
time_t trStrToLongTime(char*szTime)
{
	struct tm tm1;
	time_t time1;

	memset(&tm1,0,sizeof(tm1));
	sscanf(szTime, "%4d-%02d-%02d %02d:%02d:%02d",
			&tm1.tm_year,&tm1.tm_mon,  &tm1.tm_mday, 
			&tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);          

	tm1.tm_year -= 1900;
	tm1.tm_mon --;
	tm1.tm_isdst=-1;
	time1 = mktime(&tm1);
	return time1;
}

//"年-月-日" 转 time_t
time_t trStrDataToLongTime(char*szTime)
{
	struct tm tm1;
	time_t time1;

	memset(&tm1,0,sizeof(tm1));
	sscanf(szTime, "%4d-%02d-%02d",
			&tm1.tm_year,
			&tm1.tm_mon,  
			&tm1.tm_mday);          

	tm1.tm_year -= 1900;
	tm1.tm_mon --;
	tm1.tm_isdst=-1;
	time1 = mktime(&tm1);
	return time1;
}

//"时:分:秒" 转 time_t
time_t trStrToShortTime(char*szTime)
{
	struct tm tm1;
	struct tm *ptm;
	time_t time1;
	int a,b,c;

	a=b=c=0;

	memset(&tm1,0,sizeof(tm1));
	sscanf(szTime, "%02d:%02d:%02d",
			&tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);          

	a=tm1.tm_hour*3600;
	b=tm1.tm_min*60;
	c=tm1.tm_sec;
	time1=a+b+c;
	//转换成格式威治时间
	ptm=gmtime(&time1);	
	time1 = mktime(ptm);
	return time1;
}

/*
获取星期几
0星期日
1-6星期一到六
*/
char getWeekDay()
{
	struct tm ptm;
	time_t tme;
	time(&tme);
	localtime_r(&tme,&ptm);
	return ptm.tm_wday;
}




///////////////////////////////
//去除字符串空格
char *ltrim( char *str ) 
{
    /**去除左边空格**/
    int length = (int)strlen( str );
    char *i;
    char *len;
    int m = 0;
    int n = 0;
    i = str;
    len = str + length;
    
    for (; i<len; i++ ) {
        if ( *i == ' ' || *i == '\t' || *i == '\n' ) {
            n ++;
        } else {
            break;
        }
    }
    for ( m=0; m<=length-n; m++ ) {
        *(str + m) = *(str + n + m);
    }
    return str;
}        

/**去除右边空格**/
char *rtrim( char *str) 
{
    char *i;
    i = str + strlen( str ) - 1;
    
    for (; i>=str; i-- ) {
        if ( *i == ' ' || *i == '\t' || *i == '\n' ) {
            *(str + strlen(str) -1) = '\0';
        } else {
            break;
        }
    }
    return str;                                                                                                                            
}

/**去除两边空格**/
char *trim(char *str)
{
    ltrim(str);
    rtrim(str);
    return str;
}