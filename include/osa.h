#ifndef __OSA_H__
#define __OSA_H__

#include <stdio.h>

#define  Char      char *
#define  Int32     signed int
#define  Uint32    unsigned int
#define  Ptr       void *

#define NONE         "\033[m" 
#define RED          "\033[0;32;31m" 
//#define LIGHT_RED    "\033[1;31m" 
#define GREEN        "\033[0;32;32m" 
//#define GREEN_LIGHT  "\033[1;32m" 
//#define BLUE         "\033[0;32;34m" 
//#define LIGHT_BLUE   "\033[1;34m" 
//#define DARY_GRAY    "\033[1;30m" 
//#define CYAN         "\033[0;36m" 
//#define LIGHT_CYAN   "\033[1;36m" 
//#define PURPLE       "\033[0;35m" 
//#define LIGHT_PURPLE "\033[1;35m" 
//#define BROWN        "\033[0;33m" 
#define YELLOW       "\033[1;33m" 
//#define LIGHT_GRAY   "\033[0;37m" 
#define WHITE        "\033[1;37m"

#ifdef  PRINT_ON
//   printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)  
#define   OSA_INFO(format, ...)    \
            printf(GREEN format NONE, ##__VA_ARGS__ )

#define   OSA_WARN(format, ...)    \
            printf(YELLOW format NONE, ##__VA_ARGS__ )

#define   OSA_ERROR(format, ...)   \
            printf(RED format NONE, ##__VA_ARGS__ )

#define   OSA_DEBUG(format, ...)   \
            printf(WHITE format NONE, ##__VA_ARGS__ )
#endif
#ifdef  PRINT_OFF
#define OSA_INFO(format, ...)
#define OSA_WARN(format, ...)
#define OSA_ERROR(format, ...)
#define OSA_DEBUG(format, ...)
#endif     

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define OSA_container(ptr, type, member) ({              \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct
{
    Ptr  pre;
    Ptr  next;
}Head;

#endif