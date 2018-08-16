/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_VECTORED_IO_DETAIL_H__
#define __HENI_VECTORED_IO_DETAIL_H__


#ifndef __HENI_VECTORED_IO_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_VECTORED_IO_H__ */

#include "HENIBase.h"
#include "HENILinkedList.h"



struct heni_iobuf_list_node_s
{
	heni_iobuf_t              iobuf;
    heni_linked_list_node_t   lnode;
};


struct heni_iobuf_list_s
{
    heni_linked_list_t   llist;
};


struct heni_iobuf_list_fiter_s
{
    heni_linked_list_fiter_t   liter;
    size_t                     boff;
};


#define heniVectoredIOGetIONodePtrFromLNodePtr(lnp) \
    (heni_iobuf_list_node_t *)((uint8_t *)(lnp) - offsetof(struct heni_iobuf_list_node_s, lnode))
#define heniVectoredIOGetIONodeCPtrFromLNodeCPtr(lnp) \
    (heni_iobuf_list_node_t const *)((uint8_t const *)(lnp) - offsetof(struct heni_iobuf_list_node_s, lnode))

#define heniVectoredIOGetIOListPtrFromLListPtr(llp) \
    (heni_iobuf_list_t *)((uint8_t *)(llp) - offsetof(struct heni_iobuf_list_s, llist))



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                        Direct list manipulation                        *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_INL_FUNCT_DEF_PREFIX void heniIOBufListInit(
        heni_iobuf_list_t * iol
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListInit(&iol->llist);
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufNodeInitBuf(
        heni_iobuf_list_node_t * ioln,
        heni_iobuf_t iobuf
) HENI_INL_FUNCT_DEF_SUFFIX
{
    ioln->iobuf = iobuf;
    heniLinkedListNodeInit(&ioln->lnode);
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufNodeInitMem(
        heni_iobuf_list_node_t * ioln,
        void * iobufPtr,
        size_t iobufLen
) HENI_INL_FUNCT_DEF_SUFFIX
{
    ioln->iobuf.base = iobufPtr;
    ioln->iobuf.len = iobufLen;
    heniLinkedListNodeInit(&ioln->lnode);
}



HENI_INL_FUNCT_DEF_PREFIX heni_iobuf_t const * heniIOBufNodeGetBuf(
        heni_iobuf_list_node_t const * ioln
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &ioln->iobuf;
}



HENI_INL_FUNCT_DEF_PREFIX void * heniIOBufNodeGetMemPtr(
        heni_iobuf_list_node_t const * ioln
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return ioln->iobuf.base;
}



HENI_INL_FUNCT_DEF_PREFIX size_t heniIOBufNodeGetMemLen(
        heni_iobuf_list_node_t const * ioln
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return ioln->iobuf.len;
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufNodeAddFront(
        heni_iobuf_list_t * iol,
        heni_iobuf_list_node_t * ioln
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListNodeAddFront(&iol->llist, &ioln->lnode);
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufNodeAddBack(
        heni_iobuf_list_t * iol,
        heni_iobuf_list_node_t * ioln
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListNodeAddBack(&iol->llist, &ioln->lnode);
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniIOBufListIsEmpty(
        heni_iobuf_list_t const * iol
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniLinkedListIsEmpty(&iol->llist);
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufListMerge(
        heni_iobuf_list_t * iolHead,
        heni_iobuf_list_t * iolTail
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListMerge(&iolHead->llist, &iolTail->llist);
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                    Iterator-based list manipulation                    *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_INL_FUNCT_DEF_PREFIX void heniIOBufListFIterInit(
        heni_iobuf_list_fiter_t * iter,
        heni_iobuf_list_t * iol
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListFIterInit(&iter->liter, &iol->llist);
    iter->boff = 0U;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniIOBufListFIterIsActive(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniLinkedListFIterIsActive(&iter->liter);
}



HENI_INL_FUNCT_DEF_PREFIX heni_iobuf_list_t * heniIOBufListFIterGetList(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t *   llist;
    llist = heniLinkedListFIterGetList(&iter->liter);
    return heniVectoredIOGetIOListPtrFromLListPtr(llist);
}



HENI_INL_FUNCT_DEF_PREFIX heni_iobuf_list_node_t * heniIOBufListFIterGetNode(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   lnode;
    lnode = heniLinkedListFIterGetNode(&iter->liter);
    return lnode == NULL ? NULL : heniVectoredIOGetIONodePtrFromLNodePtr(lnode);
}



HENI_INL_FUNCT_DEF_PREFIX void * heniIOBufListFiterGetMemPtr(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniIOBufListFIterGetBuf(iter).base;
}



HENI_INL_FUNCT_DEF_PREFIX size_t heniIOBufListFiterGetMemLen(
        heni_iobuf_list_fiter_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniIOBufListFIterGetBuf(iter).len;
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufListFIterFinish(
        heni_iobuf_list_fiter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniLinkedListFIterFinish(&iter->liter);
    iter->boff = 0U;
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufListFIterCopy(
        heni_iobuf_list_fiter_t const * srcIter,
        heni_iobuf_list_fiter_t * dstIter
) HENI_INL_FUNCT_DEF_SUFFIX
{
	heniLinkedListFIterCopy(&srcIter->liter, &dstIter->liter);
    dstIter->boff = srcIter->boff;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniIOBufListFIterEquals(
        heni_iobuf_list_fiter_t const * iter1,
        heni_iobuf_list_fiter_t const * iter2
) HENI_INL_FUNCT_DEF_SUFFIX
{
    if (heniLinkedListFIterGetNode(&iter1->liter) !=
            heniLinkedListFIterGetNode(&iter2->liter))
    {
        return 0;
    }
    return iter1->boff == iter2->boff ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTrySetByteAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        uint8_t b
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heni_iobuf_list_node_t const *   ionode;
	size_t                           res;
    if (!heniIOBufListFIterIsActive(iter))
    {
        return 0;
    }
    ionode =
            heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                    heniLinkedListFIterGetNode(&iter->liter)
            );
    HENI_DASSERT(iter->boff < ionode->iobuf.len);
    *((uint8_t *)ionode->iobuf.base + iter->boff) = b;
    res = heniIOBufListFIterTryAdvance(iter, 1);
    HENI_DASSERT(res == 1);
    return res;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast16_t heniIOBufListFIterTryGetByteAndAdvance(
        heni_iobuf_list_fiter_t * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heni_iobuf_list_node_t const *   ionode;
    uint8_t                     res;

    if (!heniIOBufListFIterIsActive(iter))
    {
        return INT_FAST16_MIN;
    }
    ionode =
            heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                    heniLinkedListFIterGetNode(&iter->liter)
            );
    HENI_DASSERT(iter->boff < ionode->iobuf.len);
    res = *((uint8_t const *)ionode->iobuf.base + iter->boff);
    heniIOBufListFIterTryAdvance(iter, 1);
    return (int_fast16_t)res;
}



HENI_INL_FUNCT_DEF_PREFIX void heniIOBufListFIterSetAfterMerge(
        heni_iobuf_list_fiter_t * iterOrNull,
        heni_iobuf_list_t * iolHead,
        heni_iobuf_list_t * iolTail
) HENI_INL_FUNCT_DEF_SUFFIX
{
    if (iterOrNull != NULL)
    {
        heniLinkedListFIterSetAfterMerge(&iterOrNull->liter, &iolHead->llist, &iolTail->llist);
        if (heniLinkedListFIterIsActive(&iterOrNull->liter))
        {
            heniLinkedListFIterAdvance(&iterOrNull->liter);
        }
        iterOrNull->boff = 0;
    }
    else
    {
        heniLinkedListMerge(&iolHead->llist, &iolTail->llist);
    }
}



#endif /* __HENI_VECTORED_IO_DETAIL_H__ */

