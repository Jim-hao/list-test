#ifndef __LIST_H__
#define __LIST_H__

#include "osa.h" 

#define SCENE_TYPE_TOTAL    (10)

typedef enum
{
    DATA_REDUCE = 0,
    DATA_CHECK,
} LIST_REDUCE_TYPE;

typedef struct OSA_ListHead
{
    struct OSA_ListHead    *prev;
    struct OSA_ListHead    *next;
} OSA_ListHead;

typedef struct
{
    LIST_REDUCE_TYPE        type;
    Char                   *data;
} LIST_PARAM;

typedef struct _NodeScene
{
    OSA_ListHead            listHead;
    Int32                   sceneType;
}SceneManager;

typedef  struct
{
    OSA_ListHead            hsceneNodeHead;
    OSA_mutex               sceneListLock;
} ALG_Mgr_ObjList;

/*
    gmutex (gSceneHead)
                 \_
                 nodeInfo_1 -> nodeInfo_2 -> nodeInfo_3 
                    |
     bindMutex    bind1
     enableMutex-(wkfl.cameraId, wkfl.enable)
     enableMutex-(wkfl.dualHead)
                    |
                  bind2 - dualMode1 - dualMode2 - dualMode3
*/

typedef struct
{
    Int32                    algInfo;
    Int32                    sceneType;
    OSA_ListHead             hbindHead;
    OSA_mutex                bindMutex;
} ALG_Manager_ObjectInfo; /* 管理算法信息 */
typedef struct            /* 将算法信息加入scene list */
{
    OSA_ListHead             hsceneHead;
    Int32                    sceneType;
    ALG_Manager_ObjectInfo   algObjInfo;
} ALG_Manager_ObjNodeInfo;

typedef struct
{
    size_t                   checkSum;
    Uint32                   camera_id;
    Uint32                   enable;
    OSA_mutex                enableMutex;
} ALG_ManagerInfo;        /* wkfl信息 */
typedef struct            /* 将wkfl信息加入bind list */
{
    OSA_ListHead             bindList;
    OSA_ListHead             dualModeHead;
    ALG_ManagerInfo          pWkflInfoPriv;
} ALG_Manager_WkflNodeInfo;

typedef struct
{
    OSA_ListHead             dualList;
    Int32                    count; /* 遇到相同模式则先累加 */
    Int32                    mode;
} DualModeInfo;

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

extern void LIST_CallBack();

#endif

