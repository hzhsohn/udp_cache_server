#include "zhlist_exp.h"
#include <stdlib.h>

void zhListNodeInit(TzhListElementNode* eleNode)
{
	memset(eleNode,0,sizeof(TzhListElementNode));
}

TzhListElementNode * zhListNodeFind(TzhList* pList,void *elementNode)
{
	TzhListElementNode *eleNode;
	for(eleNode = (TzhListElementNode *)zhListFirst(pList);
        eleNode != NULL;
        eleNode = (TzhListElementNode *)zhListNext((TzhListNode *)eleNode))
	{
		if(eleNode==elementNode)
		{
			return eleNode;
		}
	}
	return NULL;
}

TzhListElementNode * zhListNodeFindWithObject(TzhList* pList,void *pElement)
{
	TzhListElementNode *eleNode;
	for(eleNode = (TzhListElementNode *)zhListFirst(pList);
        eleNode != NULL;
        eleNode = (TzhListElementNode *)zhListNext((TzhListNode *)eleNode))
	{
		if(eleNode->pElement==pElement)
		{
			return eleNode;
		}
	}
	return NULL;
}

TzhListElementNode *zhListNodeFindWithIndex(TzhList *pList, int index)
{
	int i;
    TzhListElementNode *eleNode;
    eleNode=NULL;
	
    if (index>=0 && index<zhListCount(pList)) {
        eleNode=(TzhListElementNode *)zhListFirst(pList);
        for(i=0;i<index;i++)
        {
            eleNode=(TzhListElementNode *)zhListNext((TzhListNode*)eleNode);
        }
    }
    return eleNode;
}

TzhListElementNode * zhListNodeAdd(TzhList *pList,void *pElement,int nElementSize,int elementType)
{
	TzhListElementNode*pListNode;

	pListNode = (TzhListElementNode *)calloc(1, sizeof(TzhListElementNode));

	if(pListNode == NULL) {
		printf("TzhElementNodeList: error to get memory");
		return NULL;
	}

	zhListNodeInit(pListNode);

	pListNode->pElement=pElement;
	pListNode->nElementSize=nElementSize;
	pListNode->nEleType=elementType;

	zhListAdd(pList, (TzhListNode *)pListNode);
	return pListNode;
}

//ɾ���ɹ����ض���Ԫ��
TzhListElementNode * zhListNodePopFirst(TzhList *pListNodeList)
{
	TzhListElementNode*pFormListNode,*pNew;

	pFormListNode =(TzhListElementNode*)zhListFirst(pListNodeList);

	if(pFormListNode == NULL)
	{
		printf("TzhElementNodeList: no pNode element");
		return NULL;
	}
	pNew=(TzhListElementNode *)zhListNext((TzhListNode *)pFormListNode);
	zhListDelete(pListNodeList, (TzhListNode *)pFormListNode);
	free(pFormListNode);
	return pNew;
}

//ɾ���ɹ��������һ��Ԫ��
TzhListElementNode * zhListNodePopBack(TzhList *pListNodeList)
{
	TzhListElementNode*pFormListNode,*pNew;

	pFormListNode =(TzhListElementNode*)zhListLast(pListNodeList);

	if(pFormListNode == NULL)
	{
		printf("TzhElementNodeList: no pNode last element");
		return NULL;
	}
	pNew=(TzhListElementNode *)zhListPrev((TzhListNode *)pFormListNode);
	zhListDelete(pListNodeList, (TzhListNode *)pFormListNode);
	free(pFormListNode);
	return pNew;
}

//ɾ��ָ���ڵ�,���ͷ�Ԫ��
TzhListElementNode* zhListNodeDelete(TzhList* pListNodeList,TzhListElementNode* node)
{
	TzhListElementNode *pNew=NULL;
	if(node)
	{
		pNew=(TzhListElementNode *)zhListNext((TzhListNode *)node);
		zhListDelete(pListNodeList, (TzhListNode *)node);
		free(node);	
		return pNew;
	}
	return NULL;
}

void zhListNodeFree(TzhListElementNode* node)
{
	if(node->pElement)
	{
		free(node->pElement);
		node->pElement=NULL;
		node->nElementSize=0;
		node->nEleType=0;
	}
}

TzhListElementNode* zhListNodeFreeAndDelete(TzhList* pListNodeList,TzhListElementNode* node)
{
	TzhListElementNode *pNew=NULL;
	if(node)
	{
		pNew=(TzhListElementNode *)zhListNext((TzhListNode *)node);
		zhListDelete(pListNodeList, (TzhListNode *)node);
		if(node->pElement)
		{
			free(node->pElement);
			node->pElement=NULL;
			node->nElementSize=0;
			node->nEleType=0;
		}
		free(node);	
		return pNew;
	}
	return NULL;
}

//ɾ������,�����ع���ӳ���������һ������ڵ������ж���
void zhListNodeDeleteAll(TzhList *pList)
{
	TzhListElementNode *pListNode;
	if(pList)
	{
		for(pListNode = (TzhListElementNode *)zhListFirst(pList); pListNode != NULL;)
		{
			pListNode=zhListNodeDelete(pList,pListNode);
		}
	}
}

//�������Ԫ��,����ɾ���ڵ�
void zhListNodeFreeAll(TzhList* pList)
{
	TzhListElementNode *eleNode;
	for(eleNode = (TzhListElementNode *)zhListFirst(pList);
        eleNode != NULL;
        eleNode = (TzhListElementNode *)zhListNext((TzhListNode *)eleNode))
	{
		if(eleNode->pElement)
		{
			free(eleNode->pElement);
			eleNode->pElement=NULL;
			eleNode->nElementSize=0;
			eleNode->nEleType=0;
		}
	}
}

//�ͷ���������������
void zhListNodeFreeAndDeleteAll(TzhList* pList)
{
	TzhListElementNode *pListNode;
	if(pList)
	{
		for(pListNode = (TzhListElementNode *)zhListFirst(pList); pListNode != NULL;)
		{
			if(pListNode->pElement)
			{
				free(pListNode->pElement);
				pListNode->pElement=NULL;
				pListNode->nElementSize=0;
				pListNode->nEleType=0;
			}
			pListNode=zhListNodeDelete(pList,pListNode);
		}
	}
}
