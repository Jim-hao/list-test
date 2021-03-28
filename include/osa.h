#ifndef __OSA_H__
#define __OSA_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <linux/unistd.h>
#include <time.h>    /* for nanosleep */
#include <dirent.h>  /* for dir */

#define THREAD_NAME_LEN  (20)

#define  Char      char
#define  Int32     signed int
#define  Uint32    unsigned int
#define  Ptr       void *

#define  OSA_SUCCESS         (1)
#define  OSA_SOK             (0)
#define  OSA_FALSE           (-1)
#define  OSA_EFAULT          (-1)
#define  OSA_NULL            (NULL)
#define  OSA_isFalse(value)  ((value) != OSA_SOK)
#define  OSA_isNull(ptr)     ((OSA_NULL == ptr) ? 1 : 0)
#define  OSA_isNotNull(ptr)  ((OSA_NULL != ptr) ? 1 : 0)

#define  NONE         "\033[m" 
#define  RED          "\033[0;32;31m" 
#define  GREEN        "\033[0;32;32m" 
#define  YELLOW       "\033[1;33m" 
#define  WHITE        "\033[0;38m"

#if  0    /* stdout color choice */  
#define  NONE         "\033[m" 
#define  RED          "\033[0;32;31m" 
#define  LIGHT_RED    "\033[1;31m" 
#define  GREEN        "\033[0;32;32m" 
#define  GREEN_LIGHT  "\033[1;32m" 
#define  BLUE         "\033[0;32;34m" 
#define  LIGHT_BLUE   "\033[1;34m" 
#define  DARY_GRAY    "\033[1;30m" 
#define  CYAN         "\033[0;36m" 
#define  LIGHT_CYAN   "\033[1;36m" 
#define  PURPLE       "\033[0;35m" 
#define  LIGHT_PURPLE "\033[1;35m" 
#define  BROWN        "\033[0;33m" 
#define  YELLOW       "\033[1;33m" 
#define  LIGHT_GRAY   "\033[0;37m" 
#define  WHITE        "\033[1;37m"
#endif

#ifdef   PRINT_ON
#define  OSA_ERROR(format, ...)   \
         do { printf(RED "[%s:%d] " format NONE,     \
             __FUNCTION__, __LINE__, ##__VA_ARGS__ ); }while(0);

#define  OSA_WARN(format, ...)    \
         do { printf(YELLOW "[%s:%d] " format NONE,   \
             __FUNCTION__, __LINE__, ##__VA_ARGS__ ); }while(0);

#define  OSA_INFO(format, ...)    \
         do { printf(GREEN "[%s:%d] " format NONE,    \
             __FUNCTION__, __LINE__, ##__VA_ARGS__ ); }while(0);

#define  OSA_DEBUG(format, ...)   \
         do { printf(WHITE "[%s:%d] " format NONE,     \
              __FUNCTION__, __LINE__, ##__VA_ARGS__ ); }while(0);
#endif

#ifdef   PRINT_OFF
#define  OSA_INFO(format, ...)
#define  OSA_WARN(format, ...)
#define  OSA_ERROR(format, ...)
#define  OSA_DEBUG(format, ...)
#endif     

#define  OSA_msleep(time)  \
         usleep((time) * 1000)

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef  void *(*Func)(void *);
typedef  struct
{
    Int32            isalive;
    Char             name[THREAD_NAME_LEN];
}thread_info;

typedef  struct
{
    pthread_t        threadId;
    pthread_attr_t   attribute;
    Func             func;
    thread_info      param;
}OSA_Thread;

#endif