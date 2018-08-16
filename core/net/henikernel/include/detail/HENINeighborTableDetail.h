/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_NEIGHBOR_TABLE_DETAIL_H__
#define __HENI_NEIGHBOR_TABLE_DETAIL_H__

#ifndef __HENI_NEIGHBOR_TABLE_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_NEIGHBOR_TABLE_H__ */

#include "HENILinkedList.h"




struct heni_neighbor_table_elem_s
{
    heni_linked_list_t   bucketList;
};


struct heni_neighbor_table_s
{
    size_t                         bucketsCount;
    heni_neighbor_table_elem_t *   bucketsPtr;
    heni_kernel_t *                ker;
};


struct heni_neighbor_table_titer_s
{
    heni_neighbor_table_t *    nbt;
    size_t                     bucketIdx;
    heni_linked_list_fiter_t   bucketIter;
};


struct heni_neighbor_table_piter_s
{
    heni_neighbor_table_t *      nbt;
    heni_link_addr_container_t   currNeighborLinkAddr;
};

/**
 * This is a private implementation function.
 *
 * Returns a bucket index in a HENI neighbor table
 * for a given link-layer address.
 * @param nbt The neighbor table.
 * @param laddrPtr A pointer to the link-layer address.
 * @return The bucket index.
 */
HENI_INL_FUNCT_DEC_PREFIX size_t heniNeighborTableGetBucketForLinkAddr(
        heni_neighbor_table_t const * nbt,
        uint8_t const * laddrPtr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Searches in a HENI neighbor table for a neighbor
 * with a given link-layer address. If such a neighbor
 * exists, then sets a given iterator to point at the
 * neighbor. Otherwise, sets the iterator to the position
 * before which the neighbor should be inserted if one
 * wishes to add it into the table.
 * @param nbtti The iterator to set.
 * @param nbt The neighbor table.
 * @param laddrPtr A pointer to the link-layer address.
 * @return Nonzero if the neighbor exists in the table
 *   or zero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniNeighborTableTIterInitAtOrBeforeNeighbor(
        heni_neighbor_table_titer_t * nbtti,
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Searches in a HENI neighbor table for a next neighbor
 * starting from the current position and sets the
 * iterator to point at this neighbor, if it exists.
 * If the iteration process has finished, nothing
 * is done.
 * @param nbtti The iterator to set.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniNeighborTableTIterSeekNextNeighbor(
        heni_neighbor_table_titer_t * nbtti
) HENI_HID_FUNCT_DEC_SUFFIX;



HENI_INL_FUNCT_DEC_PREFIX size_t heniNeighborTableGetBucketForLinkAddr(
        heni_neighbor_table_t const * nbt,
        uint8_t const * laddrPtr
) HENI_INL_FUNCT_DEC_SUFFIX
{
    return heniLinkAddrHash(laddrPtr) % nbt->bucketsCount;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniNeighborTableTIterIsActive(
        heni_neighbor_table_titer_t const * nbtti
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(nbtti->nbt != NULL);
    return nbtti->bucketIdx < nbtti->nbt->bucketsCount ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX heni_neighbor_t * heniNeighborTableTIterGetNeighbor(
        heni_neighbor_table_titer_t const * nbtti
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniNeighborTableTIterIsActive(nbtti));
    return heniNeighborEntryFromBucketListNode(
            heniLinkedListFIterGetNode(&nbtti->bucketIter)
    );
}



HENI_INL_FUNCT_DEF_PREFIX void heniNeighborTableTIterFinish(
        heni_neighbor_table_titer_t * nbtti
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(nbtti->nbt != NULL);
    nbtti->bucketIdx = nbtti->nbt->bucketsCount;
}



#endif /* __HENI_NEIGHBOR_TABLE_DETAIL_H__ */

