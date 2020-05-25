#ifndef __CK_PRINTF_MSG_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "os_define.h"

//������������ת������
#define TO_UTF8(src,src_len,dst)		Gb2312ToUtf8((const char*)src, (int)src_len,(char *)dst)
#define FROM_UTF8(src,src_len,dst)		Utf8ToGb2312((const char*)src, (int)src_len,(char *)dst)

/*****
*
* ���û�رմ�ӡ��Ϣ
*/
void PrintDebugMsgEnable(int isEnable);
int getMsgErrCount(); //��ȡ������Ϣ����

/*****
*
* ��ӡ
*/

void SYS_PRINTF(const char*format,...);   //����
void WARNING_PRINTF(const char*format,...);   //��ɫ��
void ERR_PRINTF(const char*format,...);   //��ɫ��

//���ڵ��Ե���Ϣ
void ALERT_PRINTF_UTF8(const char*format,...);  //�ۺ���
void DEBUG_PRINTF(const char*format,...);  //��ɫ��
void DEBUG_PRINTF_UTF8(const char*format,...);  //��ɫ��
void WARNING_PRINTF_UTF8(const char*format,...);//��ɫ��
void DEBUG_PRINT_HEX16(unsigned char* data,int len); //��ӡ16��������

#ifdef __cplusplus
}
#endif
#define __CK_PRINTF_MSG_H_
#endif
