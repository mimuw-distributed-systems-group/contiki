/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIError.h"
#include "HENILinkedList.h"



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniLinkedListContainsNode(
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_fiter_t   fiter;
    heniLinkedListFIterInit(&fiter, list);
    while (heniLinkedListFIterIsActive(&fiter))
    {
        if (heniLinkedListFIterGetNode(&fiter) == node)
        {
            return 1;
        }
        heniLinkedListFIterAdvance(&fiter);
    }
    return 0;
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListMerge(
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (headList != tailList)
    {
        if (tailList->first != NULL)
        {
            tailList->first->prev = headList->last;
            if (headList->last != NULL)
            {
                headList->last->next = tailList->first;
            }
            else
            {
                headList->first = tailList->first;
            }
            headList->last = tailList->last;
            tailList->first = NULL;
            tailList->last = NULL;
        }
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListNodeAddAfter(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodePrececingOrNull,
        heni_linked_list_node_t * nodeToAdd
) HENI_HID_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(list != NULL);
    HENI_DASSERT(nodeToAdd != NULL);
    /* HENI_DASSERT(nodeToAdd->next == NULL && nodeToAdd->prev == NULL); */

    nodeToAdd->prev = nodePrececingOrNull;
    if (nodePrececingOrNull != NULL)
    {
        nodeToAdd->next = nodePrececingOrNull->next;
        if (nodePrececingOrNull->next != NULL)
        {
            nodePrececingOrNull->next->prev = nodeToAdd;
        }
        else
        {
            list->last = nodeToAdd;
        }
        nodePrececingOrNull->next = nodeToAdd;
    }
    else
    {
        nodeToAdd->next = list->first;
        if (list->first != NULL)
        {
            list->first->prev = nodeToAdd;
        }
        else
        {
            list->last = nodeToAdd;
        }
        list->first = nodeToAdd;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListNodeAddBefore(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodeSucceedingOrNull,
        heni_linked_list_node_t * nodeToAdd
) HENI_HID_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(list != NULL);
    HENI_DASSERT(nodeToAdd != NULL);
    /* HENI_DASSERT(nodeToAdd->next == NULL && nodeToAdd->prev == NULL); */

    nodeToAdd->next = nodeSucceedingOrNull;
    if (nodeSucceedingOrNull != NULL)
    {
        nodeToAdd->prev = nodeSucceedingOrNull->prev;
        if (nodeSucceedingOrNull->prev != NULL)
        {
            nodeSucceedingOrNull->prev->next = nodeToAdd;
        }
        else
        {
            list->first = nodeToAdd;
        }
        nodeSucceedingOrNull->prev = nodeToAdd;
    }
    else
    {
        nodeToAdd->prev = list->last;
        if (list->last != NULL)
        {
            list->last->next = nodeToAdd;
        }
        else
        {
            list->first = nodeToAdd;
        }
        list->last = nodeToAdd;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListNodeRemove(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodeToRemove
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   prevNode;
    heni_linked_list_node_t *   nextNode;

    HENI_DASSERT(list != NULL);
    HENI_DASSERT(nodeToRemove != NULL);

    prevNode = nodeToRemove->prev;
    nextNode = nodeToRemove->next;
    if (prevNode != NULL)
    {
        prevNode->next = nextNode;
        nodeToRemove->prev = NULL;
    }
    else
    {
        HENI_DASSERT(list->first == nodeToRemove);
        list->first = nextNode;
    }
    if (nextNode != NULL)
    {
        nextNode->prev = prevNode;
        nodeToRemove->next = NULL;
    }
    else
    {
        HENI_DASSERT(list->last == nodeToRemove);
        list->last = prevNode;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListSplitAfterNode(
        heni_linked_list_t * listToSplit,
        heni_linked_list_t * listAfterSplit,
        heni_linked_list_node_t * nodeForSplitOrNull
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   nextNode;

    if (nodeForSplitOrNull == NULL)
    {
        listAfterSplit->first = listToSplit->first;
        listToSplit->first = NULL;
        listAfterSplit->last = listToSplit->last;
        listToSplit->last = NULL;
    }
    else
    {
        nextNode = nodeForSplitOrNull->next;
        listAfterSplit->first = nextNode;
        if (nextNode == NULL)
        {
            HENI_DASSERT(nodeForSplitOrNull == listToSplit->last);
            listAfterSplit->last = NULL;
        }
        else
        {
            listAfterSplit->last = listToSplit->last;
            listToSplit->last = nodeForSplitOrNull;
            nodeForSplitOrNull->next = NULL;
            nextNode->prev = NULL;
        }
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListSplitBeforeNode(
        heni_linked_list_t * listToSplit,
        heni_linked_list_t * listAfterSplit,
        heni_linked_list_node_t * nodeForSplitOrNull
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   prevNode;

    listAfterSplit->first = nodeForSplitOrNull;
    if (nodeForSplitOrNull == NULL)
    {
        listAfterSplit->last = NULL;
    }
    else
    {
        listAfterSplit->last = listToSplit->last;
        prevNode = nodeForSplitOrNull->prev;
        listToSplit->last = prevNode;
        if (prevNode == NULL)
        {
            HENI_DASSERT(nodeForSplitOrNull == listToSplit->first);
            listToSplit->first = NULL;
        }
        else
        {
            nodeForSplitOrNull->prev = NULL;
            prevNode->next = NULL;
        }
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListFIterSplitAfterAndAdvance(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iter->currNode == NULL)
    {
        heniLinkedListInit(tailList);
    }
    else
    {
        heniLinkedListSplitAfterNode(iter->list, tailList, iter->currNode);
        iter->currNode = tailList->first;
    }
    iter->list = tailList;
}






HENI_API_FUNCT_DEF_PREFIX void heniLinkedListRIterSplitBeforeWithoutMoving(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * headList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iter->currNode == NULL)
    {
        heniLinkedListInit(headList);
    }
    else
    {
        heniLinkedListSplitAfterNode(iter->list, headList, iter->currNode);
        heniLinkedListSwap(iter->list, headList);
    }
    iter->list = headList;
    HENI_DASSERT(iter->currNode == headList->last);
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListRIterSplitAfterAndAdvance(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * headList
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniLinkedListSplitBeforeNode(iter->list, headList, iter->currNode);
    if (iter->currNode != NULL)
    {
        heniLinkedListSwap(iter->list, headList);
        iter->currNode = headList->last;
    }
    iter->list = headList;
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListFIterSetAfterMerge(
        heni_linked_list_fiter_t * iterOrNull,
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iterOrNull != NULL)
    {
        iterOrNull->list = headList;
        iterOrNull->currNode = headList->last;
    }
    heniLinkedListMerge(headList, tailList);
    heniLinkedListInit(tailList);
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListRIterSetAfterMerge(
        heni_linked_list_riter_t * iterOrNull,
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iterOrNull != NULL)
    {
        iterOrNull->list = headList;
        iterOrNull->currNode = tailList->first;
    }
    heniLinkedListMerge(headList, tailList);
    heniLinkedListInit(tailList);
}

