/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_NEIGHBOR_DETAIL_H__
#define __HENI_NEIGHBOR_DETAIL_H__

#ifndef __HENI_NEIGHBOR_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_NEIGHBOR_H__ */

#include "HENILinkedList.h"


struct heni_neighbor_s
{
    heni_linked_list_node_t      bucketListNode;   /* for the neighbor table */
    heni_link_addr_container_t   linkAddr;         /* for the neighbor table and others */
};


/**
 * This is a private implementation function.
 *
 * Returns a writable pointer to the link-layer
 * address of a HENI neighbor.
 * @param nbr The neighbor.
 * @return A pointer to the neighbor's link-layer
 *   address. The contents remain valid as long
 *   as the neighbor remains valid.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t * heniNeighborEntryGetLinkAddrPtr(
        heni_neighbor_t * nbr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns a bucket list node for a HENI neighbor.
 * @param nbr The neighbor.
 * @return The bucket list node for the neighbor.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniNeighborEntryToBucketListNode(
        heni_neighbor_t * nbr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns a HENI neighbor corresponding to a bucket
 * list node.
 * @param bucketListNode The bucket list node.
 * @return The neighbor corresponding to the list node.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_neighbor_t * heniNeighborEntryFromBucketListNode(
        heni_linked_list_node_t * bucketListNode
) HENI_INL_FUNCT_DEC_SUFFIX;




HENI_INL_FUNCT_DEF_PREFIX uint8_t const * heniNeighborEntryGetLinkAddrConstPtr(
        heni_neighbor_t const * nbr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(nbr->linkAddr.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t * heniNeighborEntryGetLinkAddrPtr(
        heni_neighbor_t * nbr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(nbr->linkAddr.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniNeighborEntryToBucketListNode(
        heni_neighbor_t * nbr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &nbr->bucketListNode;
}



HENI_INL_FUNCT_DEF_PREFIX heni_neighbor_t * heniNeighborEntryFromBucketListNode(
        heni_linked_list_node_t * bucketListNode
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (heni_neighbor_t *)(
            (uint8_t *)bucketListNode -
            offsetof(struct heni_neighbor_s, bucketListNode)
    );
}

#endif /* __HENI_NEIGHBOR_DETAIL_H__ */
