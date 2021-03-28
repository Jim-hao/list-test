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

int  OSA_threadCreate(OSA_Thread *pthreadHand)
{
    int status = 0;

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

Int32  OSA_threadCancle(OSA_Thread *pthread)
{
    if (OSA_isNull(pthread))
    {
        OSA_ERROR("%s is NULL\n", __FUNCTION__);
        return OSA_FALSE;
    }

    pthread->param.isalive = -1;

    sleep(2);

    return OSA_SOK;
}

Int32  OSA_threadDelete(OSA_Thread *pthread)
{
    Int32    status = 0;
    void     *joinmsg = NULL;

    if (OSA_isNull(pthread))
    {
        OSA_ERROR("%s is NULL\n", __FUNCTION__);
        return OSA_FALSE;
    }

    status = pthread_attr_destroy(&pthread->attribute);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("%s faild!\n", __FUNCTION__);
        handle_error_en(status, "pthread_attr_destroy");
    }

    status = pthread_join(pthread->threadId, joinmsg);
    if (OSA_isFalse(status))
    {
        OSA_ERROR("%s faild!\n", __FUNCTION__);
        handle_error_en(status, "pthread_join");        
    }

    OSA_INFO("thread %s delele. join value:%s\n", pthread->param.name, (char *)joinmsg);
    memset(pthread->param.name, 0, THREAD_NAME_LEN);
    free(joinmsg);
    joinmsg = OSA_NULL;

    return OSA_SOK;
}