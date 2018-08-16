/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_VECTORED_IO_H__
#define __HENI_VECTORED_IO_H__

#include "HENIBase.h"

/**
 * @file
 * HENI: Types and functions describing I/O buffer lists.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

struct heni_iobuf_list_node_s;
/**
 * A node of a HENI I/O buffer list.
 *
 * This type is hidden in the detail file, so as
 * to prevent direct manipulations. However,
 * variables of this type may normally declared.
 */
typedef struct heni_iobuf_list_node_s   heni_iobuf_list_node_t;

struct heni_iobuf_list_s;
/**
 * A HENI I/O buffer list.
 *
 * This type is hidden in the detail file, so as
 * to prevent direct manipulations. However,
 * variables of this type may normally declared.
 */
typedef struct heni_iobuf_list_s   heni_iobuf_list_t;

struct heni_iobuf_list_fiter_s;
/**
 * A forward iterator over a HENI buffered I/O list.
 *
 * This type is hidden in the detail file, so as
 * to prevent direct manipulations. However,
 * variables of this type may normally declared.
 */
typedef struct heni_iobuf_list_fiter_s   heni_iobuf_list_fiter_t;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                        Direct list manipulation                        *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Initializes a HENI I/O buffer list to
 * an empty one.
 * @param iol The list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufListInit(
        heni_iobuf_list_t * iol
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Initializes a HENI I/O buffer list node
 * with a given I/O buffer.
 * @param ioln The list node.
 * @param iobuf The I/O buffer.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufNodeInitBuf(
        heni_iobuf_list_node_t * ioln,
        heni_iobuf_t iobuf
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Initializes a HENI I/O buffer list node
 * with a given I/O buffer represented
 * as a pointer to a memory and the size
 * of memory available at that pointer.
 * @param ioln The list node.
 * @param iobufPtr The I/O buffer pointer.
 * @param iobufLen The length of the I/O buffer.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufNodeInitMem(
        heni_iobuf_list_node_t * ioln,
        void * iobufPtr,
        size_t iobufLen
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the I/O buffer bound to a HENI
 * I/O buffer list node.
 * @param node The node.
 * @return The buffer bound to the node.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_iobuf_t const * heniIOBufNodeGetBuf(
        heni_iobuf_list_node_t const * ioln
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a pointer to the I/O buffer bound
 * to a HENI I/O buffer list node.
 * @param node The node.
 * @return A pointer to the I/O buffer.
 */
HENI_INL_FUNCT_DEC_PREFIX void * heniIOBufNodeGetMemPtr(
        heni_iobuf_list_node_t const * ioln
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the length of the I/O buffer bound
 * to a HENI I/O buffer list node.
 * @param node The node.
 * @return The length of the I/O buffer.
 */
HENI_INL_FUNCT_DEC_PREFIX size_t heniIOBufNodeGetMemLen(
        heni_iobuf_list_node_t const * ioln
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Adds a node at the front of a HENI I/O buffer list.
 * @param iol The list.
 * @param ioln The node.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufNodeAddFront(
        heni_iobuf_list_t * iol,
        heni_iobuf_list_node_t * ioln
) HENI_INL_FUNCT_DEC_SUFFIX;


/**
 * Adds a node at the back of a HENI I/O buffer list.
 * @param iol The list.
 * @param ioln The node.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufNodeAddBack(
        heni_iobuf_list_t * iol,
        heni_iobuf_list_node_t * ioln
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if a HENI I/O buffer list contains
 * no buffers.
 * @param iol The list.
 * @return Nonzero if the list is empty (contains no
 *   buffers) or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniIOBufListIsEmpty(
        heni_iobuf_list_t const * iol
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the number of buffers on a HENI
 * I/O buffer list.
 * @param iol The list.
 * @return The number of I/O buffers on the list.
 */
HENI_API_FUNCT_DEC_PREFIX size_t heniIOBufListGetCount(
        heni_iobuf_list_t * iol
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Returns the total capacity of buffers on a HENI
 * I/O buffer list.
 * @param iol The list.
 * @return The total capacity of I/O buffers
 *   on the list.
 */
HENI_API_FUNCT_DEC_PREFIX size_t heniIOBufListGetCapacity(
        heni_iobuf_list_t * iol
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Merges two HENI I/O buffer lists, a head list and
 * a tail list, into the head list, thereby making
 * the tail list empty.
 * @param iolHead The head I/O buffer list.
 * @param iolTail The tail I/O buffer list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufListMerge(
        heni_iobuf_list_t * iolHead,
        heni_iobuf_list_t * iolTail
) HENI_INL_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                    Iterator-based list manipulation                    *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Initializes a forward iterator for a HENI
 * I/O buffer list.
 * @param iter The iterator.
 * @param list The list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufListFIterInit(
        heni_iobuf_list_fiter_t * iter,
        heni_iobuf_list_t * iol
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if the iteration process for a forward
 * iterator for a HENI I/O buffer list is still
 * active or has finished.
 * @param iter The iterator.
 * @return Nonzero if the iteration process is still
 *   active or zero if the process has finished.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniIOBufListFIterIsActive(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the HENI I/O buffer list iterated over by
 * a given forward iterator.
 * @param iter The iterator.
 * @return The HENI I/O buffer list.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_iobuf_list_t * heniIOBufListFIterGetList(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the node currently pointed by a forward
 * iterator for a HENI I/O buffer list.
 * @param iter The iterator.
 * @return The node currently pointer by the iterator
 *   or NULL if the iteration process has finished.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_iobuf_list_node_t * heniIOBufListFIterGetNode(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the I/O buffer pointed by a forward iterator
 * for a HENI I/O buffer list taking into account the offset
 * of the iterator with respect to the base of the
 * I/O buffer node currently pointed by the iterator.
 * If the iteration process is finished, a buffer with a
 * NULL base and zero length is returned.
 * @param iter The iterator.
 * @return A pointer to the memory buffer or NULL.
 */
HENI_API_FUNCT_DEC_PREFIX heni_iobuf_t heniIOBufListFIterGetBuf(
        heni_iobuf_list_fiter_t const * iter
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Returns a pointer to the memory buffer pointed by
 * a forward iterator for a HENI I/O buffer list
 * The pointer takes into account the offset of
 * the iterator with respect to the base of the
 * I/O buffer node currently pointed by the iterator.
 * If the iteration process is finished, NULL is returned.
 * @param iter The iterator.
 * @return A pointer to the memory buffer or NULL.
 */
HENI_INL_FUNCT_DEC_PREFIX void * heniIOBufListFiterGetMemPtr(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the length of the memory buffer pointed by
 * a forward iterator for a HENI I/O buffer list
 * The length takes into account the offset of
 * the iterator with respect to the base of the
 * I/O buffer node currently pointed by the iterator.
 * If the iteration process is finished, zero is returned.
 * @param iter The iterator.
 * @return The length of the memory buffer or zero.
 */
HENI_INL_FUNCT_DEC_PREFIX size_t heniIOBufListFiterGetMemLen(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Invalidates a forward iterator for a HENI
 * I/O buffer list as if the iteration finished.
 * The iterator must have been initialized earlier
 * for some list.
 * @param iter The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufListFIterFinish(
        heni_iobuf_list_fiter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Copies one forward iterator for a HENI
 * I/O buffer list into another iterator.
 * @param srcIter The source iterator.
 * @param dstIter The destination iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufListFIterCopy(
        heni_iobuf_list_fiter_t const * srcIter,
        heni_iobuf_list_fiter_t * dstIter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if one forward iterator for a HENI
 * I/O buffer list equals to another iterator.
 * @param iter1 The first iterator.
 * @param iter2 The second iterator.
 * @return Nonzero if the two iterators are equal or
 *   zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniIOBufListFIterEquals(
        heni_iobuf_list_fiter_t const * iter1,
        heni_iobuf_list_fiter_t const * iter2
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Tries to advances a forward iterator for a HENI
 * I/O buffer list by a given number of bytes. Returns
 * the actual number of bytes by which the iterator
 * has been advanced. The number can be zero if advancing
 * the iterator even by one byte was impossible, that is,
 * when the iteration had finished.
 * @param iter The iterator.
 * @param delta The number of bytes by which the iterator
 *   is to be advanced.
 * @return The number of bytes by which the iterator has
 *   been advanced.
 */
HENI_API_FUNCT_DEC_PREFIX size_t heniIOBufListFIterTryAdvance(
        heni_iobuf_list_fiter_t * iter,
        size_t delta
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Tries to set a single byte at the position pointed at by
 * a forward iterator for a HENI I/O buffer list and
 * advance the iterator to the next position.
 * @param iter The iterator.
 * @param b The byte to set.
 * @return One if the operation was successful, or zero if
 *   the iteration process has finished.
 */
HENI_INL_FUNCT_DEC_PREFIX size_t heniIOBufListFIterTrySetByteAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        uint8_t b
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Tries to get a single byte from the position pointed at by
 * a forward iterator for a HENI I/O buffer list and advance
 * the iterator to the next position.
 * @param iter The iterator.
 * @return A negative value if the iteration process has
 *   finished or the fetched byte otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast16_t heniIOBufListFIterTryGetByteAndAdvance(
        heni_iobuf_list_fiter_t * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Tries to copy a given number of bytes from a buffer
 * into a HENI I/O buffer list starting from the position
 * pointed at by a forward iterator for such a list.
 * The iterator is advanced in effect.
 * @param iter The iterator.
 * @param bufPtr A pointer to the buffer.
 * @param bufLen The length of the buffer.
 * @return The number of bytes actually copied. It can be
 *   smaller than the buffer length, or even zero.
 */
HENI_API_FUNCT_DEC_PREFIX size_t heniIOBufListFIterTryCopyFromOneBufAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        void const * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Tries to copy a given number of bytes from the position
 * pointed at by a forward iterator for a HENI I/O buffer
 * list to a given buffer. The iterator is advanced in effect.
 * @param iter The iterator.
 * @param bufPtr A pointer to the buffer.
 * @param bufLen The length of the buffer.
 * @return The number of bytes actually copied. It can be
 *   smaller than the buffer length, or even zero.
 */
HENI_API_FUNCT_DEC_PREFIX size_t heniIOBufListFIterTryCopyIntoOneBufAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        void * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Tries to copy a given number of bytes from one HENI I/O
 * buffer list to another. The positions in the lists
 * from and to where the bytes will be copied are indicated
 * by the respective forward iterators. The iterators are
 * advanced appropriately as a result of this function.
 * @param fromIter The iterator over an I/O buffer list from
 *   which the bytes are to be copied.
 * @param toIter The iterator over an I/O buffer list to
 *   which the bytes are to be copied.
 * @param num The number of bytes to be copied.
 * @return The number of bytes actually copied. It can be
 *   smaller than the given number, or even zero.
 */
HENI_API_FUNCT_DEC_PREFIX size_t heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(
        heni_iobuf_list_fiter_t * fromIter,
        heni_iobuf_list_fiter_t * toIter,
        size_t num
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Splits a HENI I/O vector buffer list into two lists at
 * a position pointed by a forward iterator. The nodes on
 * the list already iterated (those from the front of the
 * list to the position pointed by the iterator) will
 * remain on the list, the nodes on the list not yet iterated
 * (those from the position pointed by the iterator to the
 * back of the list) will be transferred to a tail list given
 * as a parameter. The extra node is used if the current
 * node representing an I/O buffer has to be split into two
 * I/O buffers. In such a case, the extra node will become
 * the first node of the tail list.
 * If the iteration process has finished, all nodes will
 * remain on the iterated list and the tail list will become
 * empty. The tail list is thus completely overwritten by
 * this function.
 * The iterator will be associated with the tail list and
 * will point at its first node, if any.
 * @param iter The iterator.
 * @param iolTail The tail I/O buffer list.
 * @param iolExtraNode The extra node.
 * @return Nonzero if the extra node was used or zero
 *   otherwise.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniIOBufListFIterSplitAt(
        heni_iobuf_list_fiter_t * iter,
        heni_iobuf_list_t * iolTail,
        heni_iobuf_list_node_t * iolExtraNode
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Merges two HENI I/O vector buffer lists, a head list and
 * a tail list, into the head list and sets an iterator,
 * if provided, to the first element of the former tail list.
 * If the tail list was empty, the iteration process will
 * be set to finished. The tail list will always become empty.
 * @param iterOrNull An iterator to set or NULL.
 * @param iolHead The head I/O buffer list.
 * @param iolTail The tail I/O buffer list.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniIOBufListFIterSetAfterMerge(
        heni_iobuf_list_fiter_t * iterOrNull,
        heni_iobuf_list_t * iolHead,
        heni_iobuf_list_t * iolTail
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Tries to concatenate a HENI I/O vector buffer with the
 * previous buffer on a list iterated by a given iterator.
 * A concatenation is possible if the memory corresponding
 * to the buffers forms a contiguous region. If the
 * concatenation is performed, the current buffer is removed
 * from the list and returned as the result; otherwise,
 * including the case when there is no previous buffer, NULL
 * is returned. The iteration process MUST NOT be finished
 * upon invocation of this function. Conceptually, the
 * iterator remains unmodified after the function; in practice,
 * underneath, it may point to the previous buffer if a
 * concatenation was performed.
 * @param iter The iterator pointing to the element to be
 *   concatenated.
 * @return The removed buffer or NULL.
 */
HENI_API_FUNCT_DEC_PREFIX heni_iobuf_list_node_t * heniIOBufListFIterTryConcatenateWithPrevious(
        heni_iobuf_list_fiter_t * iter
) HENI_API_FUNCT_DEC_SUFFIX;


#include "detail/HENIVectoredIODetail.h"

#endif /* __HENI_VECTORED_IO_H__ */

