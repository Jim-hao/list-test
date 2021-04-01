#include "osa.h"
#include "list.h"
#include "thread.h"
#include "file.h"
#include "demo.h"

void *Thread_func(void *ptr)
{
    Int32  i = 0;
    thread_info *threadInfo = (thread_info *)ptr;
    WKFL_DEMO *pObj = (WKFL_DEMO *)(threadInfo->ptr);
    FileInfo *pfile = pObj->pfileInfo + threadInfo->filenumber;
    LIST_PARAM  param = {0};

    pid_t  tid = gettid();
    for (i = 0; i < pfile->lineNum; i++)
    {
        Char *pdata = pfile->pDataResult + (i * 3);
        param.data  = pdata;
        param.type  = DATA_REDUCE;
        threadInfo->callback(&param);
        //OSA_DEBUG("%s  %1c%1c%1c\n",threadInfo->name, pdata[0], pdata[1], pdata[2]);
    }

    OSA_mutexLock(&pObj->wkflMutex);
    pObj->extThreadNum++;
    OSA_mutexUnLock(&pObj->wkflMutex);

    OSA_INFO("thread:%s exit %d\n", threadInfo->name, tid);
}

void Thread_create(OSA_Thread  *pthread, Int32  threadNum, void *ptr)
{
    Int32  status =  0;
    WKFL_DEMO  *pObj = (WKFL_DEMO *)ptr; 

    pthread->func  = Thread_func;
    pthread->param.isalive    = 1;
    pthread->param.filenumber = 0;
    pthread->param.callback   = (Func)LIST_CallBack;
    pthread->param.ptr        = ptr;
    snprintf(pthread->param.name, THREAD_NAME_LEN, "%s%d", "Thread", threadNum);
    status = OSA_threadCreate(pthread);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("OSA_threadCreate faild! %d\n", status);
    }
}

/* 1.Create threads. 2.Distribute the tasks. */
Int32  Thread_taskCreate(void *ptr)
{
    Int32  i = 0;
    WKFL_DEMO *pObj = (WKFL_DEMO *)ptr;
    OSA_mutexCreate(&pObj->wkflMutex);
    pObj->pthreads = calloc(pObj->dirInfo->fileNum, sizeof(OSA_Thread));

    for (i = 0; i < pObj->dirInfo->fileNum; i++)
    {
        Thread_create(&pObj->pthreads[i], i, ptr);
    }

    while(pObj->extThreadNum < pObj->dirInfo->fileNum)
    {
        OSA_msleep(100);
        OSA_INFO("main is running  exitThreadNum:%d\n", pObj->extThreadNum);
    };

    //OSA_WARN("total line:%d\n", pObj->totoalLine);

    OSA_mutexDestroy(&pObj->wkflMutex);

    for (i = 0; i < pObj->dirInfo->fileNum; i++)
    {
        OSA_threadDelete(&pObj->pthreads[i]);
    }

    free(pObj->pthreads);
}