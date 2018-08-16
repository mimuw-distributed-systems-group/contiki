/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_LINKED_LIST_H__
#define __HENI_LINKED_LIST_H__

#include "HENIBase.h"

/**
 * @file
 * HENI: Types and functions describing linked lists.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


struct heni_linked_list_node_s;
/**
 * A node of a HENI linked list.
 *
 * This type is hidden in the detail file, so as
 * to prevent direct manipulations. However,
 * variables of this type may normally declared.
 */
typedef struct heni_linked_list_node_s   heni_linked_list_node_t;


struct heni_linked_list_s;
/**
 * A HENI linked list.
 *
 * This type is hidden in the detail file, so as
 * to prevent direct manipulations. However,
 * variables of this type may normally declared.
 */
typedef struct heni_linked_list_s   heni_linked_list_t;


struct heni_linked_list_fiter_s;
/**
 * A forward iterator over HENI linked list.
 *
 * This type is hidden in the detail file, so as
 * to prevent direct manipulations. However,
 * variables of this type may normally declared.
 */
typedef struct heni_linked_list_fiter_s   heni_linked_list_fiter_t;


struct heni_linked_list_riter_s;
/**
 * A reverse iterator over HENI linked list.
 *
 * This type is hidden in the detail file, so as
 * to prevent direct manipulations. However,
 * variables of this type may normally declared.
 */
typedef struct heni_linked_list_riter_s   heni_linked_list_riter_t;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                        Direct list manipulation                        *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Initializes a HENI linked list to an empty one.
 * @param list The list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListInit(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if a HENI linked list is empty.
 * @param list The list.
 * @return Nonzero if the list is empty or
 *   zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniLinkedListIsEmpty(
        heni_linked_list_t const * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Creates a shallow copy of a HENI linked list.
 * "Shallow" means that the two lists will refer
 * to the same nodes.
 * Works even if the same list is given
 * as both parameters.
 * @param srcList The source list.
 * @param dstList The destination list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListCopyShallow(
        heni_linked_list_t const * srcList,
        heni_linked_list_t * dstList
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Swaps two HENI linked lists.
 * Works even if the same list is given
 * as both parameters.
 * @param list1 The first list.
 * @param list2 The second list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListSwap(
        heni_linked_list_t * list1,
        heni_linked_list_t * list2
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Initializes a node of a HENI linked list.
 * @param node The node.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListNodeInit(
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Adds a node at the front of a HENI linked list.
 * @param list The list.
 * @param node The node.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListNodeAddFront(
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Adds a node at the back of a HENI linked list.
 * @param list The list.
 * @param node The node.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListNodeAddBack(
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the node from the front of a HENI list.
 * @param list The list.
 * @return The node from the front of the list or
 *   NULL if the list is empty.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniLinkedListNodeGetFront(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the node from the back of a HENI list.
 * @param list The list.
 * @return The node from the back of the list or
 *   NULL if the list is empty.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniLinkedListNodeGetBack(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Removes a node from the front of a HENI linked list.
 * @param list The list.
 * @return The removed node or NULL if the list
 *   was empty before the invocation of this function.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniLinkedListNodeTryRemoveFront(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Removes a node from the back of a HENI linked list.
 * @param list The list.
 * @return The removed node or NULL if the list
 *   was empty before the invocation of this function.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniLinkedListNodeTryRemoveBack(
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if a given HENI linked list contains a given node.
 * @param list The list.
 * @param node The node.
 * @return Nonzero if the list contains the node or zero otherwise.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniLinkedListContainsNode(
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Merges two HENI linked lists into the head list,
 * thereby making the tail list empty.
 * If the lists are the same object, nothing is done.
 * @param headList The list that will be merged
 *   at the front and that will become the final
 *   list.
 * @param tailList The list that will be merged
 *   at the back and that will become empty.
 */
HENI_API_FUNCT_DEC_PREFIX void heniLinkedListMerge(
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                    Iterator-based list manipulation                    *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Initializes a forward iterator for a HENI list.
 * @param iter The iterator.
 * @param list The list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterInit(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Initializes a reverse iterator for a HENI list.
 * @param iter The iterator.
 * @param list The list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterInit(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * list
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Initializes a forward iterator to a particular
 * node on a HENI list.
 * WARNING: No check is performed as to whether
 * the node actually belongs to the list, so
 * the operation may be unsafe.
 * @param iter The iterator.
 * @param list The list.
 * @param node The node on the list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterInitWithNode(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Initializes a reverse iterator to a particular
 * node on a HENI list.
 * WARNING: No check is performed as to whether
 * the node actually belongs to the list, so
 * the operation may be unsafe.
 * @param iter The iterator.
 * @param list The list.
 * @param node The node on the list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterInitWithNode(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if the iteration process of a forward
 * iterator for a HENI list is still active or
 * has finished.
 * @param iter The iterator.
 * @return Nonzero if the iteration process is still
 *   active or zero if the process has finished.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniLinkedListFIterIsActive(
        heni_linked_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if the iteration process of a reverse
 * iterator for a HENI list is still active or
 * has finished.
 * @param iter The iterator.
 * @return Nonzero if the iteration process is still
 *   active or zero if the process has finished.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniLinkedListRIterIsActive(
        heni_linked_list_riter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Invalidates a forward iterator for a HENI list
 * as if the iteration finished. The iterator
 * must have been initialized earlier for some list.
 * @param iter The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterFinish(
        heni_linked_list_fiter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Invalidates a reverse iterator for a HENI list
 * as if the iteration finished. The iterator
 * must have been initialized earlier for some list.
 * @param iter The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterFinish(
        heni_linked_list_riter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Converts a forward iterator for a HENI list to
 * a reverse iterator. The original iterator remains
 * unchanged. If the original iterator indicates that
 * the iteration process has finished, the other
 * iterator will indicate the same.
 * @param fIter The forward iterator.
 * @param rIter The reverse iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterToRIter(
        heni_linked_list_fiter_t const * fIter,
        heni_linked_list_riter_t * rIter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Converts a reverse iterator for a HENI list to
 * a forward iterator. The original iterator remains
 * unchanged. If the original iterator indicates that
 * the iteration process has finished, the other
 * iterator will indicate the same.
 * @param rIter The reverse iterator.
 * @param fIter The forward iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterToFIter(
        heni_linked_list_riter_t const * rIter,
        heni_linked_list_fiter_t * fIter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Copies one forward iterator for a HENI list to
 * another iterator.
 * @param srcIter The source iterator.
 * @param dstIter The destination iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterCopy(
        heni_linked_list_fiter_t const * srcIter,
        heni_linked_list_fiter_t * dstIter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Copies one reverse iterator for a HENI list to
 * another iterator.
 * @param srcIter The source iterator.
 * @param dstIter The destination iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterCopy(
        heni_linked_list_riter_t const * srcIter,
        heni_linked_list_riter_t * dstIter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the HENI list associated with a forward iterator.
 * @param iter The iterator.
 * @return A pointer to the list.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_t * heniLinkedListFIterGetList(
        heni_linked_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the HENI list associated with a reverse iterator.
 * @param iter The iterator.
 * @return A pointer to the list.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_t * heniLinkedListRIterGetList(
        heni_linked_list_riter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the node currently pointed by a forward
 * iterator for a HENI list.
 * @param iter The iterator.
 * @return The node currently pointer by the iterator
 *   or NULL if the iteration process has finished.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniLinkedListFIterGetNode(
        heni_linked_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the node currently pointed by a reverse
 * iterator for a HENI list.
 * @param iter The iterator.
 * @return The node currently pointer by the iterator
 *   or NULL if the iteration process has finished.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniLinkedListRIterGetNode(
        heni_linked_list_riter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Advances a forward iterator for a HENI list
 * to the next node. The iteration process
 * must not be finished.
 * @param iter The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterAdvance(
        heni_linked_list_fiter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Advances a reverse iterator for a HENI list
 * to the next node. The iteration process
 * must not be finished.
 * @param iter The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterAdvance(
        heni_linked_list_riter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Removes the node pointed at by a forward
 * iterator for a HENI list and advances the iterator
 * to the next node. The iteration process
 * must not be finished.
 * @param iter The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterRemoveAndAdvance(
        heni_linked_list_fiter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Removes the node pointed at by a reverse
 * iterator for a HENI list and advances the iterator
 * to the next node. The iteration process
 * must not be finished.
 * @param iter The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterRemoveAndAdvance(
        heni_linked_list_riter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Adds to a HENI list a new node after the node
 * pointed at by a forward iterator and advances
 * the iterator to the newly added node. If the
 * iteration process is finished, the new node is
 * added at the back of the list and the iterator
 * points to the newly added node anyway (the
 * iteration process becomes active).
 * @param iter The iterator.
 * @param nodeToAdd The node to be added.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterAddAfterAndAdvance(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Adds to a HENI list a new node before the node
 * pointed at by a forward iterator and leaves
 * the iterator unmodified. If the iteration
 * process is finished, the new node is added at
 * the back of the list and the iterator remains
 * unmodified anyway (the iteration process remains
 * finished).
 * @param iter The iterator.
 * @param nodeToAdd The node to be added.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterAddBeforeWithoutMoving(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Adds to a HENI list a new node before the node
 * pointed at by a reverse iterator and advances
 * the iterator to the newly added node. If the
 * iteration process is finished, the new node is
 * added at the front of the list and the iterator
 * points to the newly added node anyway (the
 * iteration process becomes no active).
 * @param iter The iterator.
 * @param nodeToAdd The node to be added.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterAddBeforeAndAdvance(
        heni_linked_list_riter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Adds to a HENI list a new node after the node
 * pointed at by a reverse iterator and leaves
 * the iterator unmodified. If the iteration
 * process is finished, the new node is added at
 * the front of the list and the iterator remains
 * unmodified anyway (the iteration process remains
 * finished).
 * @param iter The iterator.
 * @param nodeToAdd The node to be added.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListRIterAddAfterWithoutMoving(
        heni_linked_list_riter_t * iter,
        heni_linked_list_node_t * nodeToAdd
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Splits a HENI linked list before the position pointed
 * by a given forward iterator into two lists. The
 * tail list is assigned to the provided buffer,
 * which is completely overwritten by this operation.
 * After the split, the iterator will point to the
 * front node on the tail list, that is, the node
 * at which the iterator pointed before the split.
 * If the iteration process has finished, no split
 * is performed but the iterator will still refer
 * to the tail list and the provided tail list
 * will be initialized to an empty list.
 * @param iter The iterator.
 * @param secondList A buffer for the second list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniLinkedListFIterSplitBeforeWithoutMoving(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * tailList
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Splits a HENI linked list after the position pointed
 * by a given forward iterator into two lists. The
 * tail list is assigned to the provided buffer,
 * which is completely overwritten by this operation.
 * After the split, the iterator will point to the
 * front node on the tail list, that is, the next
 * node in the forward iteration order after the
 * node pointed at by the iterator before the split.
 * In other words, the iterator is advanced.
 * If the iteration process has finished, no split
 * is performed but the iterator will still refer
 * to the tail list and the provided tail list
 * will initialized to an empty list.
 * @param iter The iterator.
 * @param secondList A buffer for the second list.
 */
HENI_API_FUNCT_DEC_PREFIX void heniLinkedListFIterSplitAfterAndAdvance(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Splits a HENI linked list before the position pointed
 * by a given reverse iterator into two lists. The
 * head list is assigned to the provided buffer,
 * which is completely overwritten by this operation.
 * After the split, the iterator will point to the
 * back node on the head list, that is, the node
 * at which the iterator pointed before the split.
 * If the iteration process has finished, no split
 * is performed but the iterator will still refer
 * to the head list and the provided head list
 * will be initialized to an empty list.
 * @param iter The iterator.
 * @param secondList A buffer for the second list.
 */
HENI_API_FUNCT_DEC_PREFIX void heniLinkedListRIterSplitBeforeWithoutMoving(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * headList
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Splits a HENI linked list after the position pointed
 * by a given reverse iterator into two lists. The
 * head list is assigned to the provided buffer,
 * which is completely overwritten by this operation.
 * After the split, the iterator will point to the
 * back node on the head list, that is, the next
 * node in the reverse iteration order after the
 * node pointed at by the iterator before the split.
 * In other words, the iterator is advanced.
 * If the iteration process has finished, no split
 * is performed but the iterator will still refer
 * to the head list and the provided head list
 * will initialized to an empty list.
 * @param iter The iterator.
 * @param secondList A buffer for the second list.
 */
HENI_API_FUNCT_DEC_PREFIX void heniLinkedListRIterSplitAfterAndAdvance(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * headList
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Merges two HENI linked lists into the head list,
 * thereby making the tail list empty.
 * If the lists are the same object, nothing is done.
 * If a forward iterator is provided, it is set to
 * the former back element of the head list but
 * on the merged list.
 * If the head list was empty, the iterator is
 * set to denote that the iteration process has
 * finished.
 * @param iterOrNull An iterator that will be
 *   set appropriately or NULL.
 * @param headList The list that will be merged
 *   at the front and that will become the final
 *   list.
 * @param tailList The list that will be merged
 *   at the back and that will become empty.
 */
HENI_API_FUNCT_DEC_PREFIX void heniLinkedListFIterSetAfterMerge(
        heni_linked_list_fiter_t * iterOrNull,
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Merges two HENI linked lists into the head list,
 * thereby making the tail list empty.
 * If the lists are the same object, nothing is done.
 * If a reverse iterator is provided, it is set to
 * the former front element of the tail list  but
 * on the merged list.
 * If the tail list was empty, the iterator is
 * set to denote that the iteration process has
 * finished.
 * @param iterOrNull An iterator that will be
 *   set appropriately or NULL.
 * @param headList The list that will be merged
 *   at the front and that will become the final
 *   list.
 * @param tailList The list that will be merged
 *   at the back and that will become empty.
 */
HENI_API_FUNCT_DEC_PREFIX void heniLinkedListRIterSetAfterMerge(
        heni_linked_list_riter_t * iterOrNull,
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEC_SUFFIX;



#include "detail/HENILinkedListDetail.h"

#endif /* __HENI_LINKED_LIST_H__ */

