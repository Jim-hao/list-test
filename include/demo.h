#ifndef __DEMO_H__
#define __DEMO_H__

#include "osa.h"
#include "list.h"
#include "file.h"
#include "thread.h"

typedef struct wkfl_demo
{
    DirInfo    *dirInfo;
    FileInfo   *pfileInfo;
    OSA_Thread *pthreads;
    Int32       totoalLine;
    Int32       extThreadNum;
    OSA_mutex   wkflMutex;  
}WKFL_DEMO;

#endif