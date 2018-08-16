/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include <string.h>
#include "HENIBase.h"
#include "HENIError.h"
#include "HENILinkedList.h"
#include "HENIVectoredIO.h"



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListGetCount(
        heni_iobuf_list_t * iol
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_fiter_t   liter;
    size_t                     count = 0;
    
    heniLinkedListFIterInit(&liter, &iol->llist);
    while (heniLinkedListFIterIsActive(&liter))
    {
        ++count;
        heniLinkedListFIterAdvance(&liter);
    }
    return count;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListGetCapacity(
        heni_iobuf_list_t * iol
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_fiter_t   liter;
    size_t                     capacity = 0;
    
    heniLinkedListFIterInit(&liter, &iol->llist);
    while (heniLinkedListFIterIsActive(&liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&liter)
                );
        capacity += ionode->iobuf.len;
        heniLinkedListFIterAdvance(&liter);
    }
    return capacity;
}



HENI_API_FUNCT_DEF_PREFIX heni_iobuf_t heniIOBufListFIterGetBuf(
        heni_iobuf_list_fiter_t const * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_iobuf_t                     res;
    heni_iobuf_list_node_t const *   ioln;
    ioln = heniIOBufListFIterGetNode(iter);
    if (ioln == NULL)
    {
        res.base = NULL;
        res.len = 0;
    }
    else
    {
        HENI_DASSERT(iter->boff < ioln->iobuf.len);
        res.base = (void *)((uint8_t *)ioln->iobuf.base + iter->boff);
        res.len = ioln->iobuf.len - iter->boff;
    }
    return res;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryAdvance(
        heni_iobuf_list_fiter_t * iter,
        size_t delta
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   resDelta = 0;
    while (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&iter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        maxDelta = ionode->iobuf.len - iter->boff;
        if (maxDelta > delta)
        {
            iter->boff += delta;
            return resDelta + delta;
        }
        delta -= maxDelta;
        resDelta += maxDelta;
        iter->boff = 0;
        heniLinkedListFIterAdvance(&iter->liter);
    }
    return resDelta;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryCopyFromOneBufAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        void const * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   numCopiedBytes = 0;
    while (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&iter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        maxDelta = ionode->iobuf.len - iter->boff;
        if (maxDelta > bufLen)
        {
            if (bufLen > 0)
            {
                memcpy(
                        (uint8_t *)ionode->iobuf.base + iter->boff,
                        (uint8_t const *)bufPtr + numCopiedBytes,
                        bufLen
                );
                iter->boff += bufLen;
                numCopiedBytes += bufLen;
            }
            break;
        }
        memcpy(
                (uint8_t *)ionode->iobuf.base + iter->boff,
                (uint8_t const *)bufPtr + numCopiedBytes,
                maxDelta
        );
        bufLen -= maxDelta;
        numCopiedBytes += maxDelta;
        iter->boff = 0;
        heniLinkedListFIterAdvance(&iter->liter);
    }
    return numCopiedBytes;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryCopyIntoOneBufAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        void * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   numCopiedBytes = 0;
    while (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&iter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        maxDelta = ionode->iobuf.len - iter->boff;
        if (maxDelta > bufLen)
        {
            if (bufLen > 0)
            {
                memcpy(
                        (uint8_t *)bufPtr + numCopiedBytes,
                        (uint8_t const *)ionode->iobuf.base + iter->boff,
                        bufLen
                );
                iter->boff += bufLen;
                numCopiedBytes += bufLen;
            }
            break;
        }
        memcpy(
                (uint8_t *)bufPtr + numCopiedBytes,
                (uint8_t const *)ionode->iobuf.base + iter->boff,
                maxDelta
        );
        bufLen -= maxDelta;
        numCopiedBytes += maxDelta;
        iter->boff = 0;
        heniLinkedListFIterAdvance(&iter->liter);
    }
    return numCopiedBytes;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(
        heni_iobuf_list_fiter_t * fromIter,
        heni_iobuf_list_fiter_t * toIter,
        size_t num
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t                      numCopiedBytes = 0;
    while (heniLinkedListFIterIsActive(&fromIter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        size_t                           realDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&fromIter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > fromIter->boff);
        maxDelta = ionode->iobuf.len - fromIter->boff;
        realDelta =
                heniIOBufListFIterTryCopyFromOneBufAndAdvance(
                        toIter,
                        (uint8_t const *)ionode->iobuf.base + fromIter->boff,
                        maxDelta > num ? num : maxDelta
                );
        num -= realDelta;
        numCopiedBytes += realDelta;
        if (realDelta < maxDelta)
        {
            fromIter->boff += realDelta;
            break;
        }
        fromIter->boff = 0;
        heniLinkedListFIterAdvance(&fromIter->liter);
    }
    return numCopiedBytes;
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniIOBufListFIterSplitAt(
        heni_iobuf_list_fiter_t * iter,
        heni_iobuf_list_t * iolTail,
        heni_iobuf_list_node_t * iolExtraNode
) HENI_API_FUNCT_DEF_SUFFIX
{
    int_fast8_t   usedExtra = 0;

    HENI_DASSERT(&iolTail->llist != heniLinkedListFIterGetList(&iter->liter));
    if (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_linked_list_node_t *   lnode;
        heni_iobuf_list_node_t *    ionode;
        
        lnode = heniLinkedListFIterGetNode(&iter->liter);
        ionode = heniVectoredIOGetIONodePtrFromLNodePtr(lnode);
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        if (iter->boff == 0)
        {
            heniLinkedListFIterSplitBeforeWithoutMoving(&iter->liter, &iolTail->llist);
        }
        else
        {
            HENI_DASSERT(iolExtraNode != NULL);
            iolExtraNode->iobuf.base = (void *)((uint8_t *)ionode->iobuf.base + iter->boff);
            iolExtraNode->iobuf.len = ionode->iobuf.len - iter->boff;
            ionode->iobuf.len = iter->boff;
            heniLinkedListFIterSplitAfterAndAdvance(&iter->liter, &iolTail->llist);
            heniLinkedListFIterAddBeforeWithoutMoving(&iter->liter, &iolExtraNode->lnode);
            heniLinkedListFIterInit(&iter->liter, &iolTail->llist);
            iter->boff = 0;
            usedExtra = 1;
        }
    }
    else
    {
        heniLinkedListInit(&iolTail->llist);
        heniLinkedListFIterInit(&iter->liter, &iolTail->llist);
        iter->boff = 0;
    }
    return usedExtra;
}



HENI_API_FUNCT_DEF_PREFIX heni_iobuf_list_node_t * heniIOBufListFIterTryConcatenateWithPrevious(
        heni_iobuf_list_fiter_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_riter_t   tmpLIter;
    heni_iobuf_list_node_t *   ionodeCurr;
    heni_iobuf_list_node_t *   ionodePrev;

    heniLinkedListFIterToRIter(&iter->liter, &tmpLIter);
    HENI_DASSERT(heniLinkedListRIterIsActive(&tmpLIter));
    ionodeCurr =
            heniVectoredIOGetIONodePtrFromLNodePtr(
                    heniLinkedListRIterGetNode(&tmpLIter)
            );
    heniLinkedListRIterAdvance(&tmpLIter);
    if (! heniLinkedListRIterIsActive(&tmpLIter))
    {
        return NULL;
    }
    ionodePrev =
            heniVectoredIOGetIONodePtrFromLNodePtr(
                    heniLinkedListRIterGetNode(&tmpLIter)
            );
    if (((void *)((uint8_t *)ionodePrev->iobuf.base + ionodePrev->iobuf.len)) !=
            ionodeCurr->iobuf.base)
    {
        return NULL;
    }
    iter->boff += ionodePrev->iobuf.len;
    ionodePrev->iobuf.len += ionodeCurr->iobuf.len;
    heniLinkedListFIterRemoveAndAdvance(&iter->liter);
    heniLinkedListRIterToFIter(&tmpLIter, &iter->liter);
    return ionodeCurr;
}

