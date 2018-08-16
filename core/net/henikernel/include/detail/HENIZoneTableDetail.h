/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_ZONE_TABLE_DETAIL_H__
#define __HENI_ZONE_TABLE_DETAIL_H__

#ifndef __HENI_ZONE_TABLE_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_ZONE_TABLE_H__ */

/* NOTICE iwanicki 2017-06-19:                        */
/* If needed, the kernel include may be removed, but  */
/* then heniZoneTableTIterIsActive should be moved to */
/* the C file.                                        */
#include "HENIKernel.h"
#include "HENILinkedList.h"



struct heni_zone_table_bucket_s
{
    heni_linked_list_t   bucketList;
};

struct heni_zone_table_s
{
    heni_zone_table_bucket_t *   bucketPtrsPerRow[HENI_MAX_LIDS_IN_LABEL + 1];
    size_t                       bucketCountPerRow; // the number of buckets per row
                                                    // apart from row 0, which
                                                    // always has 1 bucket
    heni_instance_t *            inst;
    heni_zone_table_bucket_t     rootZoneBucketBuf;
};

struct heni_zone_table_titer_s
{
    heni_linked_list_fiter_t   inrowBucketIter;
    size_t                     inrowBucketIdx;
    heni_level_t               rowLevel;
    heni_zone_table_t *        zt;
};

struct heni_zone_table_piter_s
{
    heni_zone_table_t *        zt;
    heni_level_t               zoneLevel;
    heni_zone_lid_t            zoneLID;
    heni_zone_discr_t          zoneDiscr;
};


/**
 * This is a private implementation function.
 *
 * Returns a bucket index in a HENI zone table
 * for a given zone level and LID.
 * @param zt The zone table.
 * @param level The zone level.
 * @param lid The zone LID.
 * @return The bucket index.
 */
HENI_INL_FUNCT_DEC_PREFIX size_t heniZoneTableGetBucketForLevelAndLID(
        heni_zone_table_t const * zt,
        heni_level_t level,
        heni_zone_lid_t lid
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Searches in a row of a HENI zone table for a zone
 * with a given LID and discriminator. If such a zone
 * exists, then sets a given iterator to point at the
 * zone. Otherwise, sets the iterator to the position
 * before which the zone should be inserted if one
 * wishes to add it into the table.
 * @param iter The iterator to set.
 * @param zt The zone table.
 * @param zoneLevel The level of the row in the table.
 * @param zoneLID The LID.
 * @param zoneDiscr The discriminator.
 * @return Nonzero if the zone exists in the table
 *   or zero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID,
        heni_zone_discr_t zoneDiscr
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Searches in a row of a HENI zone table for a zone
 * with a given LID and any discriminator. If such a zone
 * exists, then sets a given iterator to point at the
 * zone. Otherwise, sets the iterator to the position
 * before which the zone should be inserted if one
 * wishes to add it into the table.
 * @param iter The iterator to set.
 * @param zt The zone table.
 * @param zoneLevel The level of the row in the table.
 * @param zoneLID The LID.
 * @return Nonzero if the zone exists in the table
 *   or zero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLid(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID
) HENI_HID_FUNCT_DEC_SUFFIX;



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniZoneTableTIterIsActive(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(iter->zt != NULL);
    return iter->rowLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(iter->zt->inst) ?
            (int_fast8_t)1 : (int_fast8_t)0;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_t * heniZoneTableTIterGetZone(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    return heniZoneEntryFromBucketListNode(
            heniLinkedListFIterGetNode(&iter->inrowBucketIter)
    );
}



HENI_INL_FUNCT_DEF_PREFIX heni_level_t heniZoneTableTIterGetLevel(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    return iter->rowLevel;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_lid_t heniZoneTableTIterGetLID(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniZoneEntryGetLID(heniZoneTableTIterGetZone(iter));
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_discr_t heniZoneTableTIterGetDiscr(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniZoneEntryGetDiscr(heniZoneTableTIterGetZone(iter));
}



HENI_INL_FUNCT_DEF_PREFIX size_t heniZoneTableGetBucketForLevelAndLID(
        heni_zone_table_t const * zt,
        heni_level_t level,
        heni_zone_lid_t lid
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return level == 0 ? 0 : (size_t)(lid % zt->bucketCountPerRow);
}


#endif /* __HENI_ZONE_TABLE_DETAIL_H__ */
