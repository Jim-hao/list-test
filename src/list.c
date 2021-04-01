#include  "list.h"

SceneManager gSceneHead =
{
    .listHead={&gSceneHead.listHead, &gSceneHead.listHead}
};

void OSA_listHeadInit(OSA_ListHead *pHead)
{
    pHead->prev = pHead;
    pHead->next = OSA_NULL;
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

static void LIST_Data_Insert(LIST_PARAM  *param)
{


}

static void LIST_Data_Delete(LIST_PARAM  *param)
{
    Int32  i = 0;
    OSA_ListHead *pnode = OSA_NULL;
    OSA_ListHead *pnext = OSA_NULL;
    SceneManager *pSceneNode = OSA_NULL;

/*
    for (i = 0;  i < SCENE_TYPE_TOTAL; i++)
    {
        pNodeArray[i] = calloc(1, sizeof(SceneManager));
        pNodeArray[i]->sceneType = data[i];
        OSA_listAddTail(&gSceneHead.listHead, &pNodeArray[i]->listHead);
    }

    OSA_listForEachSafe(pnode, pnext, &gSceneHead.listHead)
    {
        pSceneNode = OSA_listEntry(pnode, SceneManager, listHead);
        OSA_INFO("%d\n", pSceneNode->sceneType);        
    }

    for (i = 0; i < SCENE_TYPE_TOTAL;  i++)
    {
        free(pNodeArray[i]);
    } */
}

static Int32 LIST_Data_Reduce(LIST_PARAM  *param)
{
    switch (param->data[0])
    {
        case '+':
            LIST_Data_Insert(param);
            break;
        case '-':
            LIST_Data_Delete(param);
            break;
        default:
            OSA_ERROR("novalid type\n");
            break;
    }
}

static Int32 LIST_Data_Check(LIST_PARAM  *param)
{

}

void LIST_CallBack(void *ptr)
{
    LIST_PARAM  *param = (LIST_PARAM *)ptr;

    switch(param->type)
    {
        case DATA_REDUCE:
            LIST_Data_Reduce(param);
            break;
        case DATA_CHECK:
            LIST_Data_Check(param);
            break;
        default:
            break;
    }
}
