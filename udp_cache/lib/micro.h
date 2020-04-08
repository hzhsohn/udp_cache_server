#ifndef __MICRO_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "os_define.h"
#include "platform.h"
#include "print_msg.h"
#include "encoding.h"

#ifdef Q_OS_WIN32
	#include <direct.h>
#else
	#include <sys/stat.h>
    #include <sys/types.h>
#endif

//�ض��岿�ֿ�ƽ̨����
#ifdef Q_OS_WIN32
//win���� 
#define strcasecmp  strcmpi
#define strtok_r    strtok_s
#define snprintf    sprintf_s

#else
//linuxϵͳ����
#define strcmpi     strcasecmp
#endif

//���Դ���
#ifdef Q_OS_WIN32
#pragma warning (disable:4819)
#endif

#ifdef Q_OS_WIN32
    #define MKDIR(path)					mkdir(path)
	#define RMDIR(path)					rmdir(path)
#endif

#ifdef Q_OS_MACX
    #define MKDIR(path)    [[NSFileManager defaultManager] createDirectoryAtPath: [NSString stringWithUTF8String:path] withIntermediateDirectories:YES attributes:nil error:nil]
	#define RMDIR(path)    [[NSFileManager defaultManager] removeItemAtPath:path error:nil]
#endif

#ifdef Q_OS_LINUX 
    #define MKDIR(path)					mkdir(path,S_IRWXU)
	#define RMDIR(path)					rmdir(path)
#endif

//������������ת������
#define TO_UTF8(src,src_len,dst)		Gb2312ToUtf8((const char*)src, (int)src_len,(char *)dst)
#define FROM_UTF8(src,src_len,dst)		Utf8ToGb2312((const char*)src, (int)src_len,(char *)dst)

#ifndef _WIN32
#define DWORD	    unsigned int
#define WORD	    unsigned short
#define BOOL	    int
#define strcmpi     strcasecmp
#endif


#ifdef __cplusplus
}
#endif
#define __MICRO_H_
#endif
