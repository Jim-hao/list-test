#include "osa.h"
#include "list.h"
#include "thread.h"
#include "file.h"
#include "demo.h"

#define SCENE_TYPE_TOTAL  10

extern SceneManager gSceneHead;

void *Thread_func(void *ptr);
void Thread_create(OSA_Thread  *pthread, Int32 threadNum, void *ptr);
Int32  Thread_taskCreate(void *ptr);

void *Thread_func(void *ptr)
{
    Int32  i = 0;
    thread_info *threadInfo = (thread_info *)ptr;
    WKFL_DEMO *pObj = (WKFL_DEMO *)(threadInfo->ptr);
    FileInfo *pfile = pObj->pfileInfo + threadInfo->filenumber;

    pid_t  tid = gettid();
    for (i = 0; i < pfile->lineNum; i++)
    {
        Char *pdata = pfile->pDataResult + (i * 3);
        // OSA_mutexLock(&pObj->wkflMutex);
        // pObj->totoalLine++;
        // OSA_mutexUnLock(&pObj->wkflMutex);
        //OSA_DEBUG("%s  %1c%1c%1c\n",threadInfo->name, pdata[0], pdata[1], pdata[2]);
        //OSA_msleep(250);
    }

/*     int i = 0;
    OSA_ListHead *pnode = OSA_NULL;
    OSA_ListHead *pnext = OSA_NULL;
    SceneManager *pSceneNode = OSA_NULL;

    for (i = 0;  i < SCENE_TYPE_TOTAL; i++)
    {
        pNodeArray[i] = calloc(1, sizeof(SceneManager));
        pNodeArray[i]->sceneType = data[i];
        OSA_listAddTail(&gSceneHead.listHead, &pNodeArray[i]->listHead);
    }

    OSA_listForEachSafe(pnode, pnext, &gSceneHead.listHead)
    {
        pSceneNode = OSA_listEntry(pnode, SceneManager, listHead);
        OSA_INFO("%d\n", pSceneNode->sceneType);        
    }

    for (i = 0; i < SCENE_TYPE_TOTAL;  i++)
    {
        free(pNodeArray[i]);
    } */

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
    pthread->param.ptr        = ptr;
    snprintf(pthread->param.name, THREAD_NAME_LEN, "%s%d", "Thread__", threadNum);
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