#ifndef __LIST_H__
#define __LIST_H__

#include "osa.h" 

typedef struct OSA_ListHead
{
    struct OSA_ListHead   *prev;
    struct OSA_ListHead   *next;
}OSA_ListHead;

typedef struct _NodeScene
{
    OSA_ListHead   listHead;
    Int32          sceneType;
}SceneManager;

#define  OSA_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define  OSA_listEntry(ptr, type, member) ({                            \
            const typeof( ((type *)0)->member ) *__mptr = (ptr);        \
            (type *)( (char *)__mptr - offsetof(type,member) );})

#define OSA_listForEachSafe(pos, tmp, head)                             \
        for (pos = (head)->next, tmp = pos->next; pos != (head);        \
             pos = tmp, tmp = pos->next)

#define list_for_each_entry(pos, head, member)                          \
        for (pos = list_first_entry(head, typeof(*pos), member);        \
             &pos->member != (head);                                    \
             pos = list_next_entry(pos, member))

#endif