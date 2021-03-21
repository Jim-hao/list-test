#ifndef __LIST_H__
#define __LIST_H__

#include "osa.h" 

typedef struct
{
    Ptr  pre;
    Ptr  next;
}Head;

typedef struct _Node
{
    struct _Node   *pnext;
    struct _Node   *pre;
}Node;


#endif