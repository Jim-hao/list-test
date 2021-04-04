#ifndef __LIST_H__
#define __LIST_H__

#include "osa.h" 

#define SCENE_TYPE_TOTAL    (10)

/* sceneType:  0  1  2
   channel:    0  1  2  3
   modeType:   0  1  2
*/
typedef enum
{
    SCENE_NORMAL = 0,
    SCENE_FACE   = 1,
    SCENE_BMD    = 2
}SceneType;

typedef enum
{
    MODE_PIC     = 0,
    MODE_VIDEO   = 1,
    MODE_BLACK   = 2
}ModeType;

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
} ALG_Manager_ObjNodeInfo;/* struct 1 */

typedef struct
{
    size_t                   checkSum;
    Int32                   camera_id;
    Int32                   enable;
    Int32                    channel;
    OSA_mutex                enableMutex;
} ALG_ManagerInfo;        /* wkfl信息 */
typedef struct            /* 将wkfl信息加入bind list */
{
    OSA_ListHead             bindList;
    OSA_ListHead             dualModeHead;
    ALG_ManagerInfo          pWkflInfoPriv;
} ALG_Manager_WkflNodeInfo;/* struct 2 */

typedef struct
{
    OSA_ListHead             dualList;
    Int32                    count; /* 遇到相同模式则先累加 */
    Int32                    mode;
} DualModeInfo;            /* struct 3 */

#define MAX_DATA_NUM  (100)


typedef struct
{
    Int32                    bindCount;
    Int32                    bindInfo[MAX_DATA_NUM];
} List_WkflCheck;
typedef struct
{
    Int32                    wkflCount;
    List_WkflCheck           wkflInfo[MAX_DATA_NUM];
} List_SceneCheck;
typedef struct
{
    Int32                    sceneCount;
    List_SceneCheck          sceneInfo[MAX_DATA_NUM];
} List_Check;

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

