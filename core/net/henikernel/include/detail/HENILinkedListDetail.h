/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_LINKED_LIST_DETAIL_H__
#define __HENI_LINKED_LIST_DETAIL_H__


#ifndef __HENI_LINKED_LIST_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_LINKED_LIST_H__ */

#include "HENIError.h"



struct heni_linked_list_node_s
{
    struct heni_linked_list_node_s *   next;
    struct heni_linked_list_node_s *   prev;
};



struct heni_linked_list_s
{
    struct heni_linked_list_node_s *   first;
    struct heni_linked_list_node_s *   last;
};



struct heni_linked_list_fiter_s
{
    struct heni_linked_list_s *        list;
	struct heni_linked_list_node_s *   currNode;
};



struct heni_linked_list_riter_s
{
    struct heni_linked_list_s *        list;
	struct heni_linked_list_node_s *   currNode;
};



/**
 * This is a private implementation function.
 *
 * Adds to a HENI linked list a node after a given node.
 * @param list The list.
 * @param nodePrececingOrNull The node that is to precede
 *   the added node or NULL if the added node should
 *   become the first node of the list.
 * @param nodeToAdd The node to be added.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniLinkedListNodeAddAfter(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodePrececingOrNull,
        heni_linked_list_node_t * nodeToAdd
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Adds to a HENI linked list a node before a given node.
 * @param list The list.
 * @param nodeSucceedingOrNull The node that is to succeed
 *   the added node or NULL if the added node should
 *   become the last node of the list.
 * @param nodeToAdd The node to be added.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniLinkedListNodeAddBefore(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodeSucceedingOrNull,
        heni_linked_list_node_t * nodeToAdd
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Removes a node from a HENI linked list.
 * @param list The list.
 * @param nodeToRemove The node to be removed.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniLinkedListNodeRemove(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodeToRemove
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Splits one HENI linked list after a given node
 * such that the node becomes the last node of
 * the first list. If the node is NULL, the second
 * list becomes the first and the first becomes empty.
 * @param listToSplit The list to split that must
 *   contain the node.
 * @param listAfterSplit A buffer that will become the
 *   second list.
 * @param nodeForSplitOrNull The node before which
 *   the split is to be done or NULL.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniLinkedListSplitAfterNode(
        heni_linked_list_t * listToSplit,
        heni_linked_list_t * listAfterSplit,
        heni_linked_list_node_t * nodeForSplitOrNull
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Splits one HENI linked list before a given node
 * such that the node becomes the first node of
 * the second list. If the node is NULL, the second
 * list becomes empty.
 * @param listToSplit The list to split that must
 *   contain the node.
 * @param listAfterSplit A buffer that will become the
 *   second list.
 * @param nodeForSplitOrNull The node before which
 *   the split is to be done or NULL.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniLinkedListSplitBeforeNode(
        heni_linked_list_t * listToSplit,
        heni_linked_list_t * listAfterSplit,
        heni_linked_list_node_t * nodeForSplitOrNull
) HENI_HID_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                        Direct list manipulation                        *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListInit(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
    list->first = NULL;
    list->last = NULL;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniLinkedListIsEmpty(
        heni_linked_list_t const * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return list->first == NULL ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListCopyShallow(
        heni_linked_list_t const * srcList,
        heni_linked_list_t * dstList
) HENI_INL_FUNCT_DEF_SUFFIX
{
    if (srcList != dstList)
    {
        dstList->first = srcList->first;
        dstList->last = srcList->last;
    }
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListSwap(
        heni_linked_list_t * list1,
        heni_linked_list_t * list2
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t * node;
    if (list1 != list2)
    {
        node = list1->first;
        list1->first = list2->first;
        list2->first = node;
        node = list1->last;
        list1->last = list2->last;
        list2->last = node;
    }
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListNodeInit(
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEF_SUFFIX
{
    node->next = NULL;
    node->prev = NULL;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListNodeAddFront(
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEF_SUFFIX
{
	heniLinkedListNodeAddAfter(list, NULL, node);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListNodeAddBack(
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEF_SUFFIX
{
	heniLinkedListNodeAddBefore(list, NULL, node);
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniLinkedListNodeGetFront(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return list->first;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniLinkedListNodeGetBack(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return list->last;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniLinkedListNodeTryRemoveFront(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
	heni_linked_list_node_t * const   node = list->first;
    if (node != NULL)
    {
        heniLinkedListNodeRemove(list, node);
    }
    return node;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniLinkedListNodeTryRemoveBack(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
	heni_linked_list_node_t * const   node = list->last;
    if (node != NULL)
    {
        heniLinkedListNodeRemove(list, node);
    }
    return node;
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                    Iterator-based list manipulation                    *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterInit(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListFIterInitWithNode(iter, list, list->first);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterInit(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListRIterInitWithNode(iter, list, list->last);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterInitWithNode(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEF_SUFFIX
{
    iter->list = list;
    iter->currNode = node;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterInitWithNode(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEF_SUFFIX
{
    iter->list = list;
    iter->currNode = node;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniLinkedListFIterIsActive(
        heni_linked_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return iter->currNode != NULL ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniLinkedListRIterIsActive(
        heni_linked_list_riter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return iter->currNode != NULL ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterFinish(
        heni_linked_list_fiter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    iter->currNode = NULL;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterFinish(
        heni_linked_list_riter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    iter->currNode = NULL;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterToRIter(
        heni_linked_list_fiter_t const * fIter,
        heni_linked_list_riter_t * rIter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    rIter->list = fIter->list;
    rIter->currNode = fIter->currNode;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterToFIter(
        heni_linked_list_riter_t const * rIter,
        heni_linked_list_fiter_t * fIter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    fIter->list = rIter->list;
    fIter->currNode = rIter->currNode;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterCopy(
        heni_linked_list_fiter_t const * srcIter,
        heni_linked_list_fiter_t * dstIter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    dstIter->list = srcIter->list;
    dstIter->currNode = srcIter->currNode;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterCopy(
        heni_linked_list_riter_t const * srcIter,
        heni_linked_list_riter_t * dstIter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    dstIter->list = srcIter->list;
    dstIter->currNode = srcIter->currNode;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_t * heniLinkedListFIterGetList(
        heni_linked_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return iter->list;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_t * heniLinkedListRIterGetList(
        heni_linked_list_riter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return iter->list;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniLinkedListFIterGetNode(
        heni_linked_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return iter->currNode;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniLinkedListRIterGetNode(
        heni_linked_list_riter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return iter->currNode;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterAdvance(
        heni_linked_list_fiter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(iter->currNode != NULL);
    iter->currNode = iter->currNode->next;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterAdvance(
        heni_linked_list_riter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(iter->currNode != NULL);
    iter->currNode = iter->currNode->prev;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterRemoveAndAdvance(
        heni_linked_list_fiter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t * const   nodeToRemove = iter->currNode;
    HENI_DASSERT(nodeToRemove != NULL);
    iter->currNode = nodeToRemove->next;
    heniLinkedListNodeRemove(iter->list, nodeToRemove);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterRemoveAndAdvance(
        heni_linked_list_riter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t * const   nodeToRemove = iter->currNode;
    HENI_DASSERT(nodeToRemove != NULL);
    iter->currNode = nodeToRemove->prev;
    heniLinkedListNodeRemove(iter->list, nodeToRemove);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterAddAfterAndAdvance(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEF_SUFFIX
{
    if (iter->currNode == NULL)
    {
        heniLinkedListNodeAddBefore(iter->list, iter->currNode, nodeToAdd);
    }
    else
    {
        heniLinkedListNodeAddAfter(iter->list, iter->currNode, nodeToAdd);
    }
    iter->currNode = nodeToAdd;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterAddBeforeWithoutMoving(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListNodeAddBefore(iter->list, iter->currNode, nodeToAdd);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterAddBeforeAndAdvance(
        heni_linked_list_riter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEF_SUFFIX
{
    if (iter->currNode == NULL)
    {
        heniLinkedListNodeAddAfter(iter->list, iter->currNode, nodeToAdd);
    }
    else
    {
        heniLinkedListNodeAddBefore(iter->list, iter->currNode, nodeToAdd);
    }
    iter->currNode = nodeToAdd;
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListRIterAddAfterWithoutMoving(
        heni_linked_list_riter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListNodeAddAfter(iter->list, iter->currNode, nodeToAdd);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLinkedListFIterSplitBeforeWithoutMoving(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * tailList
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListSplitBeforeNode(iter->list, tailList, iter->currNode);
    iter->list = tailList;
    HENI_DASSERT(iter->currNode == tailList->first);
}



#endif /* __HENI_LINKED_LIST_DETAIL_H__ */

