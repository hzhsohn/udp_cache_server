#include "ck_assist.h"

#ifdef _WIN32
#include "windows.h"
#endif

char*getCurrentPath(char*filename,char*dst_buf,int dst_buf_len)
{
#ifdef _WIN32
	GetModuleFileName(NULL,dst_buf,dst_buf_len); 
	(strrchr(dst_buf, '\\'))[1] = 0; //删除文件名，只获得路径
#else
	//头文件：direct.h（TC2.0下为dir.h）
	getcwd(dst_buf, dst_buf_len);
	strcat(dst_buf,"/");
#endif
	if(filename)
	{ strcat(dst_buf,filename); }
	return dst_buf;
}

char*getWorkPath(char*work_path,char*directory,char*filename,char*dst_buf,int dst_buf_len)
{
	char buf[512];

	if(0==dst_buf_len){return NULL;}
	memset(dst_buf,0,dst_buf_len);

	if(NULL==work_path)
	{work_path="";}

	if(0==strcmp("./",work_path) || 0==strcmp(".\\",work_path) )
	{work_path="";}
	
	buf[0]=0;
#ifdef _WIN32
	if(strlen(work_path)>0)
	{
		sprintf(buf,"%s\\",work_path);
	}
	if(NULL!=directory && 0!=strlen(directory))
	{
		sprintf(buf,"%s%s\\",buf,directory);
	}
#else
	if(strlen(work_path)>0)
	{
		sprintf(buf,"%s/",work_path);
	}
	if(NULL!=directory && 0!=strlen(directory))
	{
		sprintf(buf,"%s%s/",buf,directory);
	}	
#endif
	if(NULL!=filename && 0!=strlen(filename))
	{
		sprintf(buf,"%s%s",buf,filename);
	}
	if(0==strcmp(".",work_path) || 0==strncmp("./",work_path,2) || 0==strncmp(".\\",work_path,2) ||
	0==strcmp("..",work_path) || 0==strncmp("../",work_path,3) || 0==strncmp("..\\",work_path,3) ||
	0==strlen(work_path))
	{
		getCurrentPath(buf,dst_buf,dst_buf_len);
	}
	else
	{
		strcpy(dst_buf,buf);
	}

	return dst_buf;
}
