#ifndef _ZH_PROCESS_TCP_H_
#include <stdio.h>
#include "session.h"

#ifdef __cplusplus
extern "C"{
#endif

bool InitProc(int argc,char *argv[]);
bool RunProc();
void EndProc();

#ifdef __cplusplus
}
#endif
#define _ZH_PROCESS_TCP_H_
#endif
