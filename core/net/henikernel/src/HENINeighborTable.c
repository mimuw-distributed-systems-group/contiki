/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENINeighborTable.h"



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniNeighborTableTIterInitAtOrBeforeNeighbor(
        heni_neighbor_table_titer_t * nbtti,
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_HID_FUNCT_DEF_SUFFIX
{
    int_fast8_t   cmpRes = 1;
    nbtti->nbt = nbt;
    nbtti->bucketIdx = heniNeighborTableGetBucketForLinkAddr(nbt, laddrPtr);
    heniLinkedListFIterInit(
            &nbtti->bucketIter,
            &(nbt->bucketsPtr[nbtti->bucketIdx].bucketList)
    );
    while (heniLinkedListFIterIsActive(&nbtti->bucketIter))
    {
        heni_neighbor_t const *   nbrTmp;
        uint8_t const *           laddrTmp;
        nbrTmp =
                heniNeighborEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&nbtti->bucketIter)
                );
        laddrTmp = heniNeighborEntryGetLinkAddrConstPtr(nbrTmp);
        cmpRes = heniLinkAddrCmp(laddrTmp, laddrPtr);
        if (cmpRes >= 0)
        {
            break;
        }
        heniLinkedListFIterAdvance(&nbtti->bucketIter);
    }
    return !cmpRes;
}



HENI_HID_FUNCT_DEC_PREFIX void heniNeighborTableTIterSeekNextNeighbor(
        heni_neighbor_table_titer_t * nbtti
) HENI_HID_FUNCT_DEC_SUFFIX
{
    /* All buckets have been tested. */
    if (nbtti->bucketIdx >= nbtti->nbt->bucketsCount)
    {
        return;
    }
    if (heniLinkedListFIterIsActive(&nbtti->bucketIter))
    {
        return;
    }
    for (++nbtti->bucketIdx; nbtti->bucketIdx < nbtti->nbt->bucketsCount; ++nbtti->bucketIdx)
    {
        heniLinkedListFIterInit(
                &nbtti->bucketIter,
                &(nbtti->nbt->bucketsPtr[nbtti->bucketIdx].bucketList)
        );
        if (heniLinkedListFIterIsActive(&nbtti->bucketIter))
        {
            return;
        }
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableInit(
        heni_neighbor_table_t * nbt,
        heni_kernel_t * ker,
        heni_neighbor_table_elem_t * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   bucketIdx;
    HENI_PASSERT(bufPtr != NULL && bufLen > 0);
    nbt->bucketsCount = bufLen;
    nbt->bucketsPtr = bufPtr;
    nbt->ker = ker;
    for (bucketIdx = 0; bucketIdx < bufLen; ++bucketIdx)
    {
        heniLinkedListInit(&(nbt->bucketsPtr[bucketIdx].bucketList));
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableCleanup(
        heni_neighbor_table_t * nbt
) HENI_API_FUNCT_DEF_SUFFIX
{
    /* NOTICE iwanicki 2017-05-23:       */
    /* We do no additional checks here.  */
    nbt->ker = NULL;
    nbt->bucketsPtr = NULL;
    nbt->bucketsCount = 0;
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableAddNonexisting(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr,
        heni_neighbor_t * nbr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_titer_t   nbtti;

    heniLinkAddrCopy(laddrPtr, heniNeighborEntryGetLinkAddrPtr(nbr));
    HENI_PASSERT(! heniNeighborTableTIterInitAtOrBeforeNeighbor(&nbtti, nbt, laddrPtr));
    heniLinkedListFIterAddBeforeWithoutMoving(
            &nbtti.bucketIter,
            heniNeighborEntryToBucketListNode(nbr)
    );
}



HENI_API_FUNCT_DEF_PREFIX heni_neighbor_t * heniNeighborTableFind(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_titer_t   nbtti;
    if (heniNeighborTableTIterInitAtOrBeforeNeighbor(&nbtti, nbt, laddrPtr))
    {
        return heniNeighborTableTIterGetNeighbor(&nbtti);
    }
    return NULL;
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableRemoveExisting(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_titer_t   nbtti;
    HENI_PASSERT(heniNeighborTableTIterInitAtOrBeforeNeighbor(&nbtti, nbt, laddrPtr));
    heniLinkedListFIterRemoveAndAdvance(&nbtti.bucketIter);
}


HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterInit(
        heni_neighbor_table_titer_t * nbtti,
        heni_neighbor_table_t * nbt
) HENI_API_FUNCT_DEF_SUFFIX
{
    nbtti->nbt = nbt;
    nbtti->bucketIdx = 0;
    if (nbtti->bucketIdx < nbtti->nbt->bucketsCount)
    {
        heniLinkedListFIterInit(
                &nbtti->bucketIter,
                &(nbtti->nbt->bucketsPtr[nbtti->bucketIdx].bucketList)
        );
        heniNeighborTableTIterSeekNextNeighbor(nbtti);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterCopy(
        heni_neighbor_table_titer_t const * nbttiSrc,
        heni_neighbor_table_titer_t * nbttiDst
) HENI_API_FUNCT_DEF_SUFFIX
{
    nbttiDst->nbt = nbttiSrc->nbt;
    nbttiDst->bucketIdx = nbttiSrc->bucketIdx;
    heniLinkedListFIterCopy(&nbttiSrc->bucketIter, &nbttiDst->bucketIter);
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterToPIter(
        heni_neighbor_table_titer_t const * nbtti,
        heni_neighbor_table_piter_t * nbtpi
) HENI_API_FUNCT_DEF_SUFFIX
{
    nbtpi->nbt = nbtti->nbt;
    if (heniNeighborTableTIterIsActive(nbtti))
    {
        heniLinkAddrCopy(
                heniNeighborEntryGetLinkAddrConstPtr(
                        heniNeighborTableTIterGetNeighbor(nbtti)
                ),
                &(nbtpi->currNeighborLinkAddr.data8[0])
        );
    }
    else
    {
        heniLinkAddrInvalidate(&(nbtpi->currNeighborLinkAddr.data8[0]));
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTablePIterToTIter(
        heni_neighbor_table_piter_t const * nbtpi,
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (heniLinkAddrIsValid(&(nbtpi->currNeighborLinkAddr.data8[0])))
    {
        heniNeighborTableTIterInitAtOrBeforeNeighbor(
                nbtti,
                nbtpi->nbt,
                &(nbtpi->currNeighborLinkAddr.data8[0])
        );
    }
    else
    {
        nbtti->nbt = nbtpi->nbt;
        nbtti->bucketIdx = nbtti->nbt->bucketsCount;
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterAdvance(
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniLinkedListFIterIsActive(&nbtti->bucketIter));
    heniLinkedListFIterAdvance(&nbtti->bucketIter);
    heniNeighborTableTIterSeekNextNeighbor(nbtti);
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterRemoveAndAdvance(
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniLinkedListFIterIsActive(&nbtti->bucketIter));
    heniLinkedListFIterRemoveAndAdvance(&nbtti->bucketIter);
    heniNeighborTableTIterSeekNextNeighbor(nbtti);
}
