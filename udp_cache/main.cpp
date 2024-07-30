#include "src/main_proc.h"
#include "print_msg.h"

/*-----------------------------

------------------------------ */
MainPROC g_main;

int main(int argc,char *argv[])
{
	printf("VERSION: v1.1\n");

	//开始程序
	if(g_main.InitProc(argc,argv))
	{
		while(g_main.RunProc());
	}
	g_main.EndProc();
	return 0;
}
