#include "main_proc.h"
#include "print_msg.h"
#include "ck_assist.h"
#include "json/cJSON.h"
#include "udp_magr.h"

bool MainPROC::InitProc(int argc,char *argv[])
{
	int isSetWorkPath=0;
	int iParam=0;

	//--------------------------------------------------------------------------
	PrintDebugMsgEnable(true);
	//--------------------------------------------------------------------------

	//获取是否为项目调试模式的参数
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

	//初始化
	_udpMagr.setdelegate.set_recv_cb(this,&MainPROC::udp_recvf);
	_udpMagr.init(6699);

	SYS_PRINTF("Load successfully");
	return true;
}

bool MainPROC::RunProc()
{
	zhPlatSleep(1);
	return true;
}

void MainPROC::EndProc()
{

}

//--------------------------------------------------
void MainPROC::udp_recvf(char*ip,int port,char* data,int len)
{
	DEBUG_PRINTF("udp_recvf ip=%s, port=%d,int len=%d)",ip,port,len);

}
