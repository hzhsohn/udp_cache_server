#ifndef _ZH_LIB_ASSIST2_FUNCTION_
#include <stdio.h>
#include "platform.h"

#ifdef __cplusplus
extern "C"{
#endif

//��ȡ��ǰ�Ĺ���Ŀ¼·��
char*getCurrentPath(char*filename,char*dst_buf,int dst_buf_len);
char*getWorkPath(char*work_path,char*directory,char*filename,char*dst_buf,int dst_buf_len);

#ifdef __cplusplus
}
#endif
#define _ZH_LIB_ASSIST2_FUNCTION_
#endif
