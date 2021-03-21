#include <linux/unistd.h>   /* for gettid */
#include  "list.h"

pid_t gettid(void){ return syscall(__NR_gettid); }

typedef struct _Node
{
    struct _Node   *pnext;
    struct _Node   *pre;
}Node;

void OSA_listHeadInit()
{

}

void OSA_listAddTail()
{

}

void OSA_nodeDelete()
{

}

void OSA_traverListSafe()
{

}

int alive = 1;

void *func(void *ptr)
{
    thread_info  *tinfo = (thread_info *)ptr;
    pid_t  tid = gettid();

    while(tinfo->isalive > 0)
    {
        OSA_DEBUG("gettid:%d\n", tid);
        sleep(2);
    }

    OSA_INFO("thread:%s exit %d\n", tinfo->name, tid);
}

Int32 main(Int32 argc, char *argv[])
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

    OSA_INFO("main exit\n");
    return  0;
}