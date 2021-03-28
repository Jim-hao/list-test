#ifndef __THREAD_H__
#define __THREAD_H__

#include "osa.h"
#include "file.h"

#define THREAD_NAME_LEN  (50)

typedef  void *(*Func)(void *);
typedef  struct
{
    Int32            isalive;
    Int32            filenumber;
    Char             name[THREAD_NAME_LEN];
    void            *ptr;
}thread_info;

typedef  struct
{
    pthread_t        threadId;
    pthread_attr_t   attribute;
    Func             func;
    thread_info      param;
}OSA_Thread;

#endif