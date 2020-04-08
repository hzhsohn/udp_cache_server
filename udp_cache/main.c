#include "src/proc.h"
#include "print_msg.h"

/*-----------------------------

------------------------------ */
int main(int argc,char *argv[])
{
	int i=0;
 	//初始化日志
	PrintMsgInit();

	//开始程序
	if(InitProc(argc,argv))
	{
		while(RunProc());
	}
	EndProc();
	return 0;
}
