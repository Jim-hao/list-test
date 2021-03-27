#include  "list.h"

SceneManager gSceneHead =
{
    .listHead={&gSceneHead.listHead, &gSceneHead.listHead}
};

static void OSA_listHeadInit(OSA_ListHead *pHead)
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

void OSA_listAddTail(OSA_ListHead *phead, OSA_ListHead *pnode)
{
    _OSA_Listaddtail(pnode, phead->prev, phead);
}

void OSA_nodeDelete()
{

}
