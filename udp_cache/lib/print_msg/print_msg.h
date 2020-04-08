#ifndef __CK_PRINTF_MSG_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "os_define.h"


#ifdef _WIN32
	#include <windows.h>
	#undef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
	#define VSNPRINTF(a,b,c,d) _vsnprintf(a,b,c,d)

	/* thread operate*/
	#ifndef _zh_thread_type
		#define CREATE_THREAD(func,arg)		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)func,(LPVOID)arg,0,NULL)
		#define CREATE_THREAD_RET(ret)		((ret)==0)
		#define _zh_thread_type
	#endif

	#ifndef _zh_cs_type
		#define LOCK_CS(p)					EnterCriticalSection(p)	
		#define UNLOCK_CS(p)				LeaveCriticalSection(p)
		#define INIT_CS(p)					InitializeCriticalSection(p)
		#define DELETE_CS(p)				DeleteCriticalSection(p)					
		#define TYPE_CS						CRITICAL_SECTION 
		#define _zh_cs_type
	#endif

	#define	ZH_INLINE					__inline
#else
	#define LINUX
	#include <sys/time.h>
	#include <stddef.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <sys/wait.h>

	#define VSNPRINTF(a,b,c,d) vsnprintf(a,b,c,d)
	/* thread operate */
	#include <pthread.h>
	#include <semaphore.h>

	#ifndef _zh_thread_type
		extern pthread_t _pthreadid;
		#define CREATE_THREAD(func,arg)		pthread_create(&_pthreadid,NULL,(void *(*)(void *))func,(void*)arg)
		#define CREATE_THREAD_RET(ret)		((ret)!=0)
		#define _zh_thread_type
	#endif

	#ifndef _zh_cs_type
		#define LOCK_CS(p)					sem_wait(p)
		#define UNLOCK_CS(p)				sem_post(p)
		#define INIT_CS(p)					sem_init(p,0,1)
		#define DELETE_CS(p)				sem_destroy(p)
		#define TYPE_CS						sem_t
	#define _zh_cs_type
	#endif

	#define	ZH_INLINE					inline
#endif


//������������ת������
#define TO_UTF8(src,src_len,dst)		Gb2312ToUtf8((const char*)src, (int)src_len,(char *)dst)
#define FROM_UTF8(src,src_len,dst)		Utf8ToGb2312((const char*)src, (int)src_len,(char *)dst)

/*****
*
* ���û�رմ�ӡ��Ϣ
*/
void PrintMsgInit();
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
