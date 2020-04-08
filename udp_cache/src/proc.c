#include "proc.h"
#include "resolv_cfg.h"
#include "net_logic.h"
#include "ck_assist.h"
#include "service_logic.h"
#include "json/cJSON.h"
#include "ck_define.h"
#include "templ-collect.config.h"

TckSourceCfg g_source;

char g_szPathBuf[512]={0};

bool InitProc(int argc,char *argv[])
{
	int isSetWorkPath=0;
	int iParam=0;
	//--------------------------------------------------------------------------
	PrintDebugMsgEnable(true);
	//--------------------------------------------------------------------------
	//��ȡ�Ƿ�Ϊ��Ŀ����ģʽ�Ĳ���
	for (iParam=0; iParam < argc; iParam++)
	{
        printf("Argument %d is %s\n", iParam, argv[iParam]);
	}
		printf("------------------------------------------------\r\n");
    for (iParam=0; iParam < argc; iParam++)
	{
		if(0==strcmp(argv[iParam],"-?") ||0==strcmp(argv[iParam],"-help") ||
		   0==strcmp(argv[iParam],"?") ||0==strcmp(argv[iParam],"help"))
		{
			printf("used parameter:\r\n");
			printf("\r\n");

			exit(0);
		}
	}

	//װ�ص�ǰ·���������ļ�
	if(0==isSetWorkPath)
	{
		getCurrentPath(ZH_SOURCE_NET_XML,g_szPathBuf,sizeof(g_szPathBuf));
	}

	memset(&g_source,0,sizeof(TckSourceCfg));
	if(readSrcConfig(&g_source,g_szPathBuf))
	{
		SYS_PRINTF("Load \"%s\" fail!",g_szPathBuf);
		return false;
	}

	SYS_PRINTF("Load successfully \"%s\"",g_szPathBuf);

	return true;
}

bool RunProc()
{
	zhPlatSleep(1);
	return true;
}

void EndProc()
{
}
