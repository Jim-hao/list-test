#include "osa.h"
#include "list.h"

#define SCENE_TYPE_TOTAL  10

int  data[SCENE_TYPE_TOTAL] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
SceneManager *pNodeArray[SCENE_TYPE_TOTAL] = {0};
extern SceneManager gSceneHead;

void *func(void *ptr)
{
    thread_info  *tinfo = (thread_info *)ptr;
    pid_t  tid = gettid();

    int i = 0;
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
    }

    while(tinfo->isalive > 0)
    {
        OSA_INFO("gettid:%d\n", tid);
        sleep(2);
    }

    OSA_INFO("thread:%s exit %d\n", tinfo->name, tid);
}

void create()
{
    Int32  status =  0;

    OSA_Thread  *pthread = calloc(1, sizeof(OSA_Thread));
    pthread->func  = func;
    pthread->param.isalive  = 1;
    strncpy(pthread->param.name, "func1", strlen("func1")-1);
    status = OSA_threadCreate(pthread);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("OSA_threadCreate faild! %d\n", status);
    }

    int count = 2;
    while(count > 0)
    {
        sleep(10);
        count--;
        OSA_INFO("main is running\n");
    };

    OSA_threadCancle(pthread);
    OSA_threadDelete(pthread);
}