#include  "list.h"

SceneManager gSceneHead =
{
    .listHead={&gSceneHead.listHead, &gSceneHead.listHead}
};

void OSA_listHeadInit(OSA_ListHead *pHead)
{
    pHead->prev = pHead;
    pHead->next = pHead;
}

static inline void _OSA_Listaddtail(OSA_ListHead *newList, OSA_ListHead *preNode, OSA_ListHead *head)
{
    head->prev = newList;
    newList->next = head;
    newList->prev = preNode;
    preNode->next  = newList;
}

static void OSA_listAddTail(OSA_ListHead *phead, OSA_ListHead *pnode)
{
    _OSA_Listaddtail(pnode, phead->prev, phead);
}

/*******************************************************************/

Int32  LIST_SCENTYPE_CONVERT(char DataType)
{
    Int32  type = -1;
    static  linenum = 0;

    switch(DataType)
    {
        case '0':
            type = SCENE_NORMAL;
            break;
        case '1':
            type = SCENE_FACE;
            break;
        case '2':
            type = SCENE_BMD;
            break;
        default:
            OSA_ERROR("no valid data-type %x  linenum=%d\n", DataType, linenum);
    }

    return type;
}

Int32  LIST_CHANNEL_CONVERT(char DataType)
{
    Int32  channel = -1;

    switch(DataType)
    {
        case '0':
            channel = 0;
            break;
        case '1':
            channel = 1;
            break;
        case '2':
            channel = 2;
            break;
        case '3':
            channel = 3;
            break;
        default:
            OSA_ERROR("no valid channel-num %c\n", DataType);
    }

    return channel;
}

Int32  LIST_MODETYPE_CONVERT(char DataType)
{
    Int32  mode = -1;
    static  linenum = 0;

    ++linenum;

    switch(DataType)
    {
        case '0':
        case 0:
            mode = MODE_PIC;
            break;
        case '1':
            mode = MODE_VIDEO;
            break;
        case '2':
            mode = MODE_BLACK;
            break;
        default:
            OSA_ERROR("no valid mode-type %x, linenum=%d\n", DataType, linenum);
    }

    return mode;
}

static ALG_Manager_ObjNodeInfo *LIST_GetSceneManager(SceneManager *pgSceneHead, 
                                                     Int32  sceneType)
{
    OSA_ListHead  *pos = OSA_NULL;  
    OSA_ListHead  *tmp = OSA_NULL;
    ALG_Manager_ObjNodeInfo  *pAlgManager = OSA_NULL;

    OSA_listForEachSafe(pos, tmp, &pgSceneHead->listHead)
    {
        pAlgManager = OSA_listEntry(pos, ALG_Manager_ObjNodeInfo, hsceneHead);
        if (pAlgManager->sceneType == sceneType)
            return pAlgManager;
    }

    /* 申请alg管理节点 */
    pAlgManager = calloc(1, sizeof(ALG_Manager_ObjNodeInfo));
    pAlgManager->sceneType = sceneType;
    OSA_listHeadInit(&pAlgManager->algObjInfo.hbindHead);
    OSA_listAddTail(&pgSceneHead->listHead, &pAlgManager->hsceneHead);

    return pAlgManager;
}

static ALG_Manager_WkflNodeInfo *LIST_GetWkflManager(ALG_Manager_ObjNodeInfo *pAlgManager,
                                                      Int32   channel)
{
    OSA_ListHead  *pos = OSA_NULL;  
    OSA_ListHead  *tmp = OSA_NULL;
    ALG_Manager_WkflNodeInfo *pWkflManager = OSA_NULL;

    OSA_listForEachSafe(pos, tmp, &pAlgManager->algObjInfo.hbindHead)
    {
        pWkflManager = OSA_listEntry(pos, ALG_Manager_WkflNodeInfo, bindList);       
        if (pWkflManager->pWkflInfoPriv.channel == channel)
            return pWkflManager;
    }

    pWkflManager = calloc(1, sizeof(ALG_Manager_WkflNodeInfo));
    pWkflManager->pWkflInfoPriv.channel = channel;
    OSA_listHeadInit(&pWkflManager->dualModeHead);
    OSA_listAddTail(&pAlgManager->algObjInfo.hbindHead, &pWkflManager->bindList);

    return pWkflManager;
}

static DualModeInfo *LIST_GetDualModeNode(ALG_Manager_WkflNodeInfo * pWkflManager,
                                          Int32  mode)
{
    OSA_ListHead  *pos = OSA_NULL;  
    OSA_ListHead  *tmp = OSA_NULL;
    DualModeInfo  *pDualModeInfo = OSA_NULL;

    OSA_listForEachSafe(pos, tmp, &pWkflManager->dualModeHead)
    {
        pDualModeInfo = OSA_listEntry(pos, DualModeInfo, dualList);       
        if (pDualModeInfo->mode == mode)
            return pDualModeInfo;
    }

    pDualModeInfo = calloc(1, sizeof(DualModeInfo));
    pDualModeInfo->mode = mode;
    OSA_listAddTail(&pWkflManager->dualModeHead, &pDualModeInfo->dualList);

    return pDualModeInfo;
}

static void LIST_Data_Insert(Char *data)
{
    SceneManager  *pgSceneHead = &gSceneHead;
    SceneType sceneType = LIST_SCENTYPE_CONVERT(data[1]);
    Int32 channel = LIST_CHANNEL_CONVERT(data[2]);
    ModeType mode = LIST_MODETYPE_CONVERT(data[3]);

    /* 1. get sceneManager */
    ALG_Manager_ObjNodeInfo *pAlgManager = LIST_GetSceneManager(pgSceneHead, sceneType);

    /* 2. get wkflManager */
    ALG_Manager_WkflNodeInfo *pWkflManager = LIST_GetWkflManager(pAlgManager, channel);

    /* 3. get dualMode Node */
    DualModeInfo *pDualModeInfo = LIST_GetDualModeNode(pWkflManager, mode);
    pDualModeInfo->count++;
}

LIST_NodeDelete(SceneManager *pgSceneHead, Int32 sceneType, Int32 channel, Int32 mode)
{
    ALG_Manager_ObjNodeInfo *pAlgManager = LIST_GetSceneManager(pgSceneHead, sceneType);
    ALG_Manager_WkflNodeInfo *pWkflManager = LIST_GetWkflManager(pAlgManager, channel);
    DualModeInfo *pDualModeInfo = LIST_GetDualModeNode(pWkflManager, mode);
    pDualModeInfo->count--;
}

static void LIST_Data_Delete(Char  *data)
{
    Int32  i = 0;
    OSA_ListHead *pnode = OSA_NULL;
    OSA_ListHead *pnext = OSA_NULL;
    SceneManager *pgSceneHead = &gSceneHead;

    SceneType sceneType = LIST_SCENTYPE_CONVERT(data[1]);
    Int32 channel = LIST_CHANNEL_CONVERT(data[2]);
    ModeType mode = LIST_MODETYPE_CONVERT(data[3]);

    LIST_NodeDelete(pgSceneHead, sceneType, channel, mode);
}

static Int32 LIST_Data_Reduce(Char *data)
{
    switch (data[0])
    {
        case '+':
            LIST_Data_Insert(data);
            break;
        case '-':
            LIST_Data_Delete(data);
            break;
        case 0:
            break;
        default:
            OSA_ERROR("novalid type %x.\n", data[0]);
            break;
    }
}


Int32 LIST_Check_DualModeNum(ALG_Manager_WkflNodeInfo *pWkflManager, List_WkflCheck *wkflCheck)
{
    Int32          i = 0;
    OSA_ListHead  *pos = OSA_NULL;  
    OSA_ListHead  *tmp = OSA_NULL;
    DualModeInfo  *pDualModeInfo = OSA_NULL;

    /* 计算该wkfl下的mode数目 */
    OSA_listForEachSafe(pos, tmp, &pWkflManager->dualModeHead)
    {
        pDualModeInfo = OSA_listEntry(pos, DualModeInfo, dualList);
        wkflCheck->bindInfo[i] = pDualModeInfo->count;  /* 每个bind节点有多少个模式 */
        wkflCheck->bindCount++;                         /* 有i个bind节点 */
        i++;
    }   
}

/* 计算该scene的所有wkfl数目 */
Int32 LIST_Check_WkflNum(ALG_Manager_ObjNodeInfo *pAlgManager, List_SceneCheck *sceneCheck)
{
    Int32          i = 0;
    OSA_ListHead  *pos = OSA_NULL;  
    OSA_ListHead  *tmp = OSA_NULL;
    ALG_Manager_WkflNodeInfo *pWkflManager = OSA_NULL;

    OSA_listForEachSafe(pos, tmp, &pAlgManager->algObjInfo.hbindHead)
    {
        pWkflManager = OSA_listEntry(pos, ALG_Manager_WkflNodeInfo, bindList);
        /* 计算该wkfl下的dualBind数目 */
        LIST_Check_DualModeNum(pWkflManager, &sceneCheck->wkflInfo[i]);
        sceneCheck->wkflCount++;
        i++;
    }
}

/* 计算scene的数目 */
void LIST_Check_SceneNum(SceneManager *pgSceneHead, List_Check *check)
{
    Int32         i = 0;
    OSA_ListHead  *pos = OSA_NULL;  
    OSA_ListHead  *tmp = OSA_NULL;
    ALG_Manager_ObjNodeInfo  *pAlgManager = OSA_NULL;

    /* 遍历每一个scene */
    OSA_listForEachSafe(pos, tmp, &pgSceneHead->listHead)
    {
        /* 获取这个scene的所有wkfl数目 */
        pAlgManager = OSA_listEntry(pos, ALG_Manager_ObjNodeInfo, hsceneHead);
        LIST_Check_WkflNum(pAlgManager, &check->sceneInfo[i]);
        check->sceneCount++;
        i++;
    }
}

void LIST_Check_Print(List_Check *pcheck)
{
    Int32  total = 0;

    Int32  scene = 0;
    Int32  c_scene = 0;
    for (scene = 0; scene < pcheck->sceneCount; scene++)
    { /* 遍历scene */
        Int32 wkfl = 0;
        Int32 c_wkfl = 0;
        for (wkfl = 0; wkfl < pcheck->sceneInfo[scene].wkflCount; wkfl++)
        {   /* 遍历wkfl */
            Int32  dualMode = 0;
            Int32  c_dual = 0;
            for (dualMode = 0; dualMode < pcheck->sceneInfo[scene].wkflInfo[wkfl].bindCount; dualMode++)
            { /* 遍历模式 */
                c_dual += pcheck->sceneInfo[scene].wkflInfo[wkfl].bindInfo[dualMode];
                //printf("mode:%d:%d ", dualMode, pcheck->sceneInfo[scene].wkflInfo[wkfl].bindInfo[dualMode]);
            }
            c_wkfl += c_dual;
            printf("channel:%d, total:%d\n", wkfl, c_dual);
        }

        c_scene += c_wkfl;
        printf("--------scene:%d  total:%d-------\n\n", scene, c_wkfl);
    }

    printf("-------total:%d-------\n", c_scene);
}

Int32 LIST_Data_Check()
{
    Int32  total = 0;
    List_Check check = {0};

    LIST_Check_SceneNum(&gSceneHead, &check);

    LIST_Check_Print(&check);

    OSA_DEBUG("check total = %d\n", total);
}

void LIST_CallBack(void *ptr)
{
    LIST_PARAM  *param = (LIST_PARAM *)ptr;

    switch(param->type)
    {
        case DATA_REDUCE:
            LIST_Data_Reduce(param->data);
            break;
        case DATA_CHECK:
            LIST_Data_Check(param->data);
            break;
        default:
            break;
    }
}
