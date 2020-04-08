#ifndef __CK_PRINTF_MSG_H_
#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include "print_msg.h"
#include <time.h>
#include "encoding.h"
#include <memory.h>

//是否打印输出到a.txt文件
#define PRINT_TO_FILE 		0
#define PRINT_CACHE_SIZE	1024

#ifdef _WIN32
		#include <windows.h>
		#define PRINTMSG_LOCK_CS(p)					EnterCriticalSection(p)	
		#define PRINTMSG_UNLOCK_CS(p)				LeaveCriticalSection(p)
		#define PRINTMSG_INIT_CS(p)					InitializeCriticalSection(p)
		#define PRINTMSG_DELETE_CS(p)				DeleteCriticalSection(p)					
		#define PRINTMSG_TYPE_CS					CRITICAL_SECTION 
		#define PMSG_VSNPRINTF(a,b,c,d)				_vsnprintf_s(a,b,b,c,d)

		#define strcasecmp  strcmpi
		#define strtok_r    strtok_s
		#define snprintf    sprintf_s

		#define localtime_r(a,b) localtime_s((struct tm*)b,(const time_t*)a)
		#define asctime_r(a,b) asctime_s(b,a)

#else
		#include <sys/time.h>
		#include <stddef.h>
		#include <unistd.h>
		#include <stdlib.h>
		#include <sys/wait.h>
		/* thread operate */
		#include <pthread.h>
		#include <semaphore.h>
		#include <stdarg.h>

		#define PMSG_VSNPRINTF(a,b,c,d)				vsnprintf(a,b,c,d)
	
		#define PRINTMSG_LOCK_CS(p)					sem_wait(p)
		#define PRINTMSG_UNLOCK_CS(p)				sem_post(p)
		#define PRINTMSG_INIT_CS(p)					sem_init(p,0,1)
		#define PRINTMSG_DELETE_CS(p)				sem_destroy(p)
		#define PRINTMSG_TYPE_CS					sem_t
	
		#define strcmpi     strcasecmp

		#define localtime_s(a,b) localtime_r(b,a)
		#define asctime_s(a,b) asctime_r(b,a)
#endif

//模块全局参数
int g_ckPrintDebugMsg = 0;
int g_ckPrintErrCount=0;

PRINTMSG_TYPE_CS g_ckPrintfMsgCS;

void PrintMsgInit()
{	
	PRINTMSG_INIT_CS(&g_ckPrintfMsgCS);
}

void PrintDebugMsgEnable(int isEnable)
{
	g_ckPrintDebugMsg=isEnable;
}

int getMsgErrCount()
{
	return g_ckPrintErrCount;
}

void SYS_PRINTF(const char*format,...)
{
	char tme_buf[64]={0};
	time_t tt;		
	struct tm ptm={0};
	char buf[1024]={0};
	va_list args;

	PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);

	va_start(args, format);
	
	//时间
	tt=time(NULL);
	localtime_r(&tt,&ptm);
	snprintf(tme_buf,sizeof(tme_buf), "%02d:%02d:%02d",
			ptm.tm_hour,
			ptm.tm_min,
			ptm.tm_sec);

	PMSG_VSNPRINTF(buf,sizeof(buf),format,args);
	
   	printf("System-[%s]: %s\r\n",tme_buf,buf);
	
	va_end(args);
	
	#if PRINT_TO_FILE
	{
	FILE*fp=fopen("./a.txt","a+");
	fprintf(fp,"System-[%s]: %s\r\n",tme_buf,buf);
	fclose(fp);
	}
	#endif
	
	PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
}
void WARNING_PRINTF(const char*format,...)
{
		char tme_buf[30];
		time_t tt;		
		struct tm ptm={0};
		char buf[1024]={0};
		va_list args;

		PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);

		va_start(args, format);
	
		//收集错误消息数量
		g_ckPrintErrCount++;

		//时间
		tt=time(NULL);
		localtime_r(&tt,&ptm);
		snprintf(tme_buf,sizeof(tme_buf), "%02d:%02d:%02d",
		ptm.tm_hour,
		ptm.tm_min,
		ptm.tm_sec);

		PMSG_VSNPRINTF(buf,1024,format,args);
		//----------------------
		//打印消息
		PMSG_VSNPRINTF(buf,1024,format,args);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
   			printf("Warning-[%s]: %s\r\n",tme_buf,buf);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_INTENSITY );
		#endif
		va_end(args);
		
		#if PRINT_TO_FILE
		{
		FILE*fp=fopen("./a.txt","a+");
		fprintf(fp,"Warning-[%s]: %s\r\n",tme_buf,buf);
		fclose(fp);
		}
		#endif

		PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
}
void ERR_PRINTF(const char*format,...)
{
		char tme_buf[64]={0};
		time_t tt;		
		struct tm ptm={0};
		char buf[1024]={0};
		va_list args;
		
		PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);
		va_start(args, format);
	
		//收集错误消息数量
		g_ckPrintErrCount++;

		//时间
		tt=time(NULL);
		localtime_r(&tt,&ptm);
		snprintf(tme_buf,sizeof(tme_buf), "%02d:%02d:%02d",
		ptm.tm_hour,
		ptm.tm_min,
		ptm.tm_sec);

		PMSG_VSNPRINTF(buf,sizeof(buf),format,args);
		//----------------------
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY);
		#endif
   			printf("Error-[%s]: %s\r\n",tme_buf,buf);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_INTENSITY );
			
		#endif
		va_end(args);
				
		#if PRINT_TO_FILE
		{
		FILE*fp=fopen("./a.txt","a+");
		fprintf(fp,"Error-[%s]: %s\r\n",tme_buf,buf);
		fclose(fp);
		}
		#endif
		
		PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
}

void ALERT_PRINTF_UTF8(const char*format,...)
{	
	char gbValue[1024]={0};
	char buf[1024]={0};
	va_list args;
	char tme_buf[64]={0};
	time_t tt;		
	struct tm ptm={0};

	//
	PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);

	//时间
	tt=time(NULL);
	localtime_r(&tt,&ptm);

	snprintf(tme_buf,sizeof(tme_buf), "%02d:%02d:%02d",
			ptm.tm_hour,
			ptm.tm_min,
			ptm.tm_sec);

	//数据处理	
	va_start(args, format);
	
	PMSG_VSNPRINTF(buf,sizeof(buf),format,args);
	#ifdef Q_OS_WIN32		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
	#endif
		FROM_UTF8(buf,strlen(buf),gbValue);
   		printf("Alert-[%s]: %s\r\n",tme_buf,gbValue);
	#ifdef Q_OS_WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_INTENSITY );
			
	#endif
	va_end(args);
		
	#if PRINT_TO_FILE
	{
	FILE*fp=fopen("./a.txt","a+");
	fprintf(fp,"Alert-[%s]: %s\r\n",tme_buf, gbValue);
	fclose(fp);
	}
	#endif

	PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
}

/*****
*
* 打印黑底绿色字
*/
void DEBUG_PRINTF(const char*format,...)
{
	if(g_ckPrintDebugMsg)
	{
		char buf[1024]={0};
		va_list args;
		char tme_buf[64]={0};
		time_t tt;		
		struct tm ptm;
		
		PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);
				
		//时间
		tt=time(NULL);
		localtime_r(&tt,&ptm);

		snprintf(tme_buf,sizeof(tme_buf), "%02d:%02d:%02d",
				ptm.tm_hour,
				ptm.tm_min,
				ptm.tm_sec);

		va_start(args, format);
	
		PMSG_VSNPRINTF(buf,sizeof(buf),format,args);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
   			printf("Debug-[%s]: %s\r\n",tme_buf,buf);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
		va_end(args);

		#if PRINT_TO_FILE
		{
		FILE*fp=fopen("./a.txt","a+");
		fprintf(fp,"Debug-[%s]: %s\r\n",tme_buf, gbValue);
		fclose(fp);
		}
		#endif
		}
	
		PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
}

void DEBUG_PRINTF_UTF8(const char*format,...)
{
	if(g_ckPrintDebugMsg)
	{
		va_list args;
		char *gbValue=NULL;
		char *tme_buf=NULL;
		char *buf=NULL;
		time_t tt;		
		struct tm ptm;


		PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);
		
		//时间
		tt=time(NULL);
		localtime_r(&tt,&ptm);
		
		gbValue=(char*)malloc(PRINT_CACHE_SIZE);
		buf=(char*)malloc(PRINT_CACHE_SIZE);
		tme_buf=(char*)malloc(100);
		memset(gbValue,0,PRINT_CACHE_SIZE);
		memset(buf,0,PRINT_CACHE_SIZE);
		memset(tme_buf,0,100);

		sprintf(tme_buf, "%02d:%02d:%02d",
				ptm.tm_hour,
				ptm.tm_min,
				ptm.tm_sec);

		va_start(args, format);
	
		PMSG_VSNPRINTF(buf,PRINT_CACHE_SIZE,format,args);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
			FROM_UTF8(buf,strlen(buf),gbValue);
   			printf("Debug-[%s]: %s\r\n",tme_buf,gbValue);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
		va_end(args);

		#if PRINT_TO_FILE
		{
		FILE*fp=fopen("./a.txt","a+");
		fprintf(fp,"Debug-[%s]: %s\r\n",tme_buf, gbValue);
		fclose(fp);
		}
		#endif
		
		free(gbValue);
		free(buf);
		free(tme_buf);
	}
	PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
}

/*****
*
* 打印黑底黄色字
*/
void WARNING_PRINTF_UTF8(const char*format,...)
{
	char gbValue[1024]={0};
	char buf[1024]={0};
	va_list args;
	char tme_buf[64]={0};
	time_t tt;		
	struct tm ptm;
	
	PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);	

	//时间
	tt=time(NULL);
	localtime_r(&tt,&ptm);

	sprintf(tme_buf,"%02d:%02d:%02d",
			ptm.tm_hour,
			ptm.tm_min,
			ptm.tm_sec);

	//数据处理	
	va_start(args, format);
	
	PMSG_VSNPRINTF(buf,sizeof(buf),format,args);
	#ifdef Q_OS_WIN32		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	#endif
		FROM_UTF8(buf,strlen(buf),gbValue);
   		printf("Warning-[%s]: %s\r\n",tme_buf,gbValue);
	#ifdef Q_OS_WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		
	#endif
	va_end(args);
	
	#if PRINT_TO_FILE
	{
	FILE*fp=fopen("./a.txt","a+");
	fprintf(fp,"Warning-[%s]: %s\r\n", tme_buf, gbValue);
	fclose(fp);
	}
	#endif

	PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
}

void DEBUG_PRINT_HEX16(unsigned char* data,int len)
{
	if(g_ckPrintDebugMsg)
	{
		int i;
		char* data_buf;
		char tme_buf[64]={0};
		time_t tt;		
		struct tm ptm;
		int j=0;
		
		PRINTMSG_LOCK_CS(&g_ckPrintfMsgCS);

		j=len*8;
		data_buf=(char*)malloc(j);
		memset(data_buf,0,j);
		//时间
		tt=time(NULL);
		localtime_r(&tt,&ptm);

		sprintf(tme_buf,"%02d:%02d:%02d",
				ptm.tm_hour,
				ptm.tm_min,
				ptm.tm_sec);

		//数据处理
		for(i=0;i<len;i++)
		{
			sprintf(data_buf,"%s 0x%02X",data_buf,(unsigned char)data[i]);
		}

		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
   			printf("Debug-[%s]:%s\r\n",tme_buf,data_buf);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
			
		#endif
		free(data_buf);

		PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCS);
	}
}

#ifdef __cplusplus
}
#endif
#define __CK_PRINTF_MSG_H_
#endif
