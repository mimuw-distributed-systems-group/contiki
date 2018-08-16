/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_ZONE_DETAIL_H__
#define __HENI_ZONE_DETAIL_H__

#ifndef __HENI_ZONE_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_ZONE_H__ */

#include "HENILinkedList.h"


typedef struct heni_zone_entry_key_s
{
    heni_level_t        level;
    heni_zone_lid_t     lid;
    heni_zone_discr_t   discr;
} heni_zone_entry_key_t;


struct heni_zone_s
{
    heni_linked_list_node_t      bucketListNode;   /* for the zone table */
    heni_zone_entry_key_t        key;
};



/**
 * This is a private implementation function.
 *
 * Returns a bucket list node for a HENI zone.
 * @param zone The zone.
 * @return The bucket list node for the zone.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_linked_list_node_t * heniZoneEntryToBucketListNode(
        heni_zone_t * zone
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns a HENI zone corresponding to a bucket
 * list node.
 * @param bucketListNode The bucket list node.
 * @return The zone corresponding to the list node.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_t * heniZoneEntryFromBucketListNode(
        heni_linked_list_node_t * bucketListNode
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Sets in a HENI zone entry the identifying data,
 * that is, the level, LID, and discriminator.
 * @param zone The zone entry.
 * @param level The level.
 * @param lid The LID.
 * @param discr The discriminator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniZoneEntrySetKey(
        heni_zone_t * zone,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_INL_FUNCT_DEC_SUFFIX;



HENI_INL_FUNCT_DEF_PREFIX heni_zone_lid_t heniZoneEntryGetLID(
        heni_zone_t const * zone
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return zone->key.lid;
}




HENI_INL_FUNCT_DEF_PREFIX heni_zone_discr_t heniZoneEntryGetDiscr(
        heni_zone_t const * zone
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return zone->key.discr;
}



HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniZoneEntryToBucketListNode(
        heni_zone_t * zone
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &zone->bucketListNode;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_t * heniZoneEntryFromBucketListNode(
        heni_linked_list_node_t * bucketListNode
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (heni_zone_t *)(
            (uint8_t *)bucketListNode -
            offsetof(struct heni_zone_s, bucketListNode)
    );
}



HENI_INL_FUNCT_DEF_PREFIX void heniZoneEntrySetKey(
        heni_zone_t * zone,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    zone->key.level = level;
    zone->key.lid = lid;
    zone->key.discr = discr;
}


#endif /* __HENI_ZONE_DETAIL_H__ */
