#include "osa.h"
#include "thread.h"

pid_t gettid(void){ return syscall(__NR_gettid); }

void OSA_mutexCreate(void *pmutex)
{
    pthread_mutex_init(pmutex, NULL);
}

void OSA_mutexLock(void *pmutex)
{
    pthread_mutex_lock(pmutex);
}

void OSA_mutexUnLock(void *pmutex)
{
    pthread_mutex_unlock(pmutex);
}

void OSA_mutexDestroy(void *pmutex)
{
    pthread_mutex_destroy(pmutex);
}

int  OSA_threadCreate(void  *pHandle)
{
    int status = 0;
    OSA_Thread *pthreadHand = (OSA_Thread *)pHandle;

    if (OSA_isNull(pthreadHand))
    {
        OSA_ERROR("ptr is NULL!\n");
        return OSA_FALSE;
    }

    status = pthread_attr_init(&pthreadHand->attribute);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("pthread_attr_init faild! %d\n", status);
    }

    /*  param1:  threadId
        param2:  attribute
        param3:  function
        param4:  param to function */
    status = pthread_create(&pthreadHand->threadId, &pthreadHand->attribute,
                                pthreadHand->func, (void *)&pthreadHand->param);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("pthread_create faild! %d\n", status);
    }

    return status;
}

Int32  OSA_threadCancle(void *pHandle)
{
    OSA_Thread *pthreadHand = (OSA_Thread *)pHandle;

    if (OSA_isNull(pthreadHand))
    {
        OSA_ERROR("%s is NULL\n", __FUNCTION__);
        return OSA_FALSE;
    }

    pthreadHand->param.isalive = -1;

    sleep(2);

    return OSA_SOK;
}

Int32  OSA_threadDelete(void *pHandle)
{
    Int32    status = 0;
    void     *joinmsg = NULL;
    OSA_Thread *pthreadHand = (OSA_Thread *)pHandle;

    if (OSA_isNull(pthreadHand))
    {
        OSA_ERROR("%s is NULL\n", __FUNCTION__);
        return OSA_FALSE;
    }

    status = pthread_attr_destroy(&pthreadHand->attribute);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("%s faild!\n", __FUNCTION__);
        handle_error_en(status, "pthread_attr_destroy");
    }

    status = pthread_join(pthreadHand->threadId, joinmsg);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("%s faild!\n", __FUNCTION__);
        handle_error_en(status, "pthread_join");        
    }

    //OSA_INFO("thread %s delele. join value:%s\n", pthreadHand->param.name, (char *)joinmsg);
    memset(pthreadHand->param.name, 0, THREAD_NAME_LEN);
    free(joinmsg);
    joinmsg = OSA_NULL;

    return OSA_SOK;
}