/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIKernel.h"
#include "HENILabel.h"
#include "HENILinkedList.h"
#include "HENIZoneTable.h"




/**
 * This is a private implementation function.
 *
 * Searches in a HENI zone table row for a next zone
 * starting from the current position and sets the
 * iterator to point at this zone, if it exists.
 * If the iteration process has finished, nothing
 * is done.
 * @param iter The iterator to set.
 * @return Nonzero if the next zone in row has been
 *   found or zero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniZoneTableTIterSeekNextZoneInRow(
        heni_zone_table_titer_t * iter
) HENI_HID_FUNCT_DEC_SUFFIX;



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableInit(
        heni_zone_table_t * zt,
        heni_instance_t * inst,
        heni_zone_table_bucket_t * const * bucketPtrsPerRow,
        size_t bucketCountPerRow
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_level_t   numLevels, levelIdx;

    HENI_DASSERT(inst != NULL);
    numLevels = heniKernelInstanceGetNumLevelsForInstancePtr(inst);
    HENI_PASSERT(numLevels >= 1 && numLevels <= HENI_MAX_LIDS_IN_LABEL);
    zt->bucketPtrsPerRow[0] = &zt->rootZoneBucketBuf;
    heniLinkedListInit(&zt->rootZoneBucketBuf.bucketList);
    for (levelIdx = 1; levelIdx <= numLevels; ++levelIdx)
    {
        size_t   bucketIdx;
        zt->bucketPtrsPerRow[levelIdx] = bucketPtrsPerRow[levelIdx - 1];
        HENI_PASSERT(zt->bucketPtrsPerRow[levelIdx] != NULL);
        for (bucketIdx = 0; bucketIdx < bucketCountPerRow; ++bucketIdx)
        {
            heniLinkedListInit(&(zt->bucketPtrsPerRow[levelIdx][bucketIdx].bucketList));
        }
    }
    for (; levelIdx <= HENI_MAX_LIDS_IN_LABEL; ++levelIdx)
    {
        zt->bucketPtrsPerRow[levelIdx] = NULL;
    }
    zt->bucketCountPerRow = bucketCountPerRow;
    zt->inst = inst;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableAddNonexisting(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr,
        heni_zone_t * zone
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_table_titer_t   iter;
    HENI_DASSERT((level == 0 && lid == heniLabelSpecGetZoneLIDInvalid(heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))) ||
            (level > 0 && heniLabelSpecIsZoneLIDAssignable(lid, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(discr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));
    HENI_PASSERT(! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(&iter, zt, level, lid, discr));
    heniZoneEntrySetKey(zone, level, lid, discr);
    heniLinkedListFIterAddBeforeWithoutMoving(
            &iter.inrowBucketIter,
            heniZoneEntryToBucketListNode(zone)
    );
}



HENI_API_FUNCT_DEF_PREFIX heni_zone_t * heniZoneTableFind(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_table_titer_t   iter;
    /*HENI_DASSERT(heniLabelSpecIsZoneLIDAssignable(lid, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst)));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(discr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));*/
    if (heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(&iter, zt, level, lid, discr))
    {
        return heniZoneTableTIterGetZone(&iter);
    }
    return NULL;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableRemoveExisting(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_table_titer_t   iter;
    HENI_DASSERT((level == 0 && lid == heniLabelSpecGetZoneLIDInvalid(heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))) ||
            (level > 0 && heniLabelSpecIsZoneLIDAssignable(lid, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(discr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));
    HENI_PASSERT(heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(&iter, zt, level, lid, discr));
    heniLinkedListFIterRemoveAndAdvance(&iter.inrowBucketIter);
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableCleanup(
        heni_zone_table_t * zt
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_level_t   levelIdx;
    for (levelIdx = 0; levelIdx <= HENI_MAX_LIDS_IN_LABEL; ++levelIdx)
    {
        zt->bucketPtrsPerRow[levelIdx] = NULL;
    }
    zt->bucketCountPerRow = 0;
    zt->inst = NULL;
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLid(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID
) HENI_HID_FUNCT_DEF_SUFFIX
{
    size_t   bucketIdx;
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    bucketIdx = heniZoneTableGetBucketForLevelAndLID(zt, zoneLevel, zoneLID);
    heniLinkedListFIterInit(
            &iter->inrowBucketIter,
            &(zt->bucketPtrsPerRow[zoneLevel][bucketIdx].bucketList)
    );
    iter->inrowBucketIdx = bucketIdx;
    iter->rowLevel = zoneLevel;
    iter->zt = zt;
    while (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        heni_zone_t const *   zone;
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (zone->key.lid >= zoneLID)
        {
            return zone->key.lid == zoneLID ? (int_fast8_t)1 : (int_fast8_t)0;
        }
        heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    }
    return (int_fast8_t)0;
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID,
        heni_zone_discr_t zoneDiscr
) HENI_HID_FUNCT_DEF_SUFFIX
{
    if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLid(iter, zt, zoneLevel, zoneLID))
    {
        return 0;
    }
    do
    {
        heni_zone_t const *   zone;
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (zone->key.lid > zoneLID)
        {
            return 0;
        }
        else if (zone->key.lid == zoneLID && zone->key.discr >= zoneDiscr)
        {
            return zone->key.discr == zoneDiscr ? (int_fast8_t)1 : (int_fast8_t)0;
        }
        heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    }
    while (heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    return (int_fast8_t)0;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterSetAtLevel(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    heniLinkedListFIterInit(
            &iter->inrowBucketIter,
            &(zt->bucketPtrsPerRow[zoneLevel][0].bucketList)
    );
    iter->inrowBucketIdx = 0;
    iter->rowLevel = zoneLevel;
    iter->zt = zt;
    if (! heniZoneTableTIterSeekNextZoneInRow(iter))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterSetAtLevelAndLID(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    HENI_DASSERT(heniLabelSpecIsZoneLIDAssignable(zoneLID, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst)));
    if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLid(
            iter, zt, zoneLevel, zoneLID))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterSetAtLevelAndLIDAndDiscr(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID,
        heni_zone_discr_t zoneDiscr
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    HENI_DASSERT(heniLabelSpecIsZoneLIDAssignable(zoneLID, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst)));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(zoneDiscr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));
    if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(iter, zt, zoneLevel, zoneLID, zoneDiscr))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterCopy(
        heni_zone_table_titer_t const * iterSrc,
        heni_zone_table_titer_t * iterDst
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniLinkedListFIterCopy(&iterSrc->inrowBucketIter, &iterDst->inrowBucketIter);
    iterDst->inrowBucketIdx = iterSrc->inrowBucketIdx;
    iterDst->rowLevel = iterSrc->rowLevel;
    iterDst->zt = iterSrc->zt;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterFinish(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(iter->zt != NULL);
    HENI_DASSERT(iter->zt->bucketCountPerRow > 0);
    /* NOTICE iwanicki 2017-06-09:                    */
    /* In theory, we do not need the init, but        */
    /* let's leave it here to make the implementation */
    /* more robust.                                   */
    heniLinkedListFIterInit(
            &iter->inrowBucketIter,
            &(iter->zt->bucketPtrsPerRow[0][0].bucketList)
    );
    heniLinkedListFIterFinish(&iter->inrowBucketIter);
    iter->inrowBucketIdx = iter->zt->bucketCountPerRow + 1;
    iter->rowLevel = heniKernelInstanceGetNumLevelsForInstancePtr(iter->zt->inst) + 1;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterAdvancePreservingLevel(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    if (! heniZoneTableTIterSeekNextZoneInRow(iter))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterAdvancePreservingLevelAndLID(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_t const *   zone;
    heni_zone_lid_t       lid;
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    zone =
            heniZoneEntryFromBucketListNode(
                    heniLinkedListFIterGetNode(&iter->inrowBucketIter)
            );
    lid = heniZoneEntryGetLID(zone);
    heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (heniZoneEntryGetLID(zone) == lid)
        {
            return;
        }
    }
    heniZoneTableTIterFinish(iter);
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterRemoveAndAdvancePreservingLevel(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    heniLinkedListFIterRemoveAndAdvance(&iter->inrowBucketIter);
    if (! heniZoneTableTIterSeekNextZoneInRow(iter))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_t const *   zone;
    heni_zone_lid_t       lid;
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    zone =
            heniZoneEntryFromBucketListNode(
                    heniLinkedListFIterGetNode(&iter->inrowBucketIter)
            );
    lid = heniZoneEntryGetLID(zone);
    heniLinkedListFIterRemoveAndAdvance(&iter->inrowBucketIter);
    if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (heniZoneEntryGetLID(zone) == lid)
        {
            return;
        }
    }
    heniZoneTableTIterFinish(iter);
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniZoneTableTIterSeekNextZoneInRow(
        heni_zone_table_titer_t * iter
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_zone_table_bucket_t *   rowPtr;
    size_t                       bucketCount;

    HENI_DASSERT(iter->rowLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(iter->zt->inst));
    bucketCount = iter->rowLevel == 0 ? 1 : iter->zt->bucketCountPerRow;
    if (iter->inrowBucketIdx >= bucketCount)
    {
        return (int_fast8_t)0;
    }
    if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        return (int_fast8_t)1;
    }
    rowPtr = iter->zt->bucketPtrsPerRow[iter->rowLevel];
    for (++iter->inrowBucketIdx; iter->inrowBucketIdx < bucketCount; ++iter->inrowBucketIdx)
    {
        heniLinkedListFIterInit(
                &iter->inrowBucketIter,
                &(rowPtr[iter->inrowBucketIdx].bucketList)
        );
        if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
        {
            return (int_fast8_t)1;
        }
    }
    return (int_fast8_t)0;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterToPIter(
        heni_zone_table_titer_t const * titer,
        heni_zone_table_piter_t * piter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(titer->zt != NULL);
    piter->zt = titer->zt;
    if (heniZoneTableTIterIsActive(titer))
    {
        piter->zoneLevel = heniZoneTableTIterGetLevel(titer);
        piter->zoneLID = heniZoneTableTIterGetLID(titer);
        piter->zoneDiscr = heniZoneTableTIterGetDiscr(titer);
    }
    else
    {
        piter->zoneLevel =
                heniKernelInstanceGetNumLevelsForInstancePtr(
                        titer->zt->inst
                ) + 1;
        piter->zoneLID =
                heniLabelSpecGetZoneLIDInvalid(
                        heniKernelInstanceGetLabelSpecForInstancePtr(
                                titer->zt->inst
                        )
                );
        piter->zoneDiscr =
                heniZoneDiscrSpecGetZoneDiscrInvalid(
                        heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(
                                titer->zt->inst
                        )
                );
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTablePIterToTIterPreservingLevel(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (piter->zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(
                piter->zt->inst))
    {
        HENI_DASSERT((piter->zoneLevel > 0 &&
                heniLabelSpecIsZoneLIDAssignable(
                        piter->zoneLID,
                        heniKernelInstanceGetLabelSpecForInstancePtr(
                                piter->zt->inst))) ||
                (piter->zoneLevel == 0 && piter->zoneLID ==
                        heniLabelSpecGetZoneLIDInvalid(
                                heniKernelInstanceGetLabelSpecForInstancePtr(
                                        piter->zt->inst))));
        if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(
                titer, piter->zt, piter->zoneLevel, piter->zoneLID, piter->zoneDiscr))
        {
            if (! heniZoneTableTIterSeekNextZoneInRow(titer))
            {
                heniZoneTableTIterFinish(titer);
            }
        }
    }
    else
    {
        heniZoneTableTIterSetAtLevel(titer, piter->zt, 0);
        heniZoneTableTIterFinish(titer);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTablePIterToTIterPreservingLevelAndLID(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniZoneTablePIterToTIterPreservingLevel(piter, titer);
    if (heniZoneTableTIterIsActive(titer))
    {
        if (piter->zoneLID != heniZoneTableTIterGetLID(titer))
        {
            heniZoneTableTIterFinish(titer);
        }
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniZoneTablePIterToTIterPreservingLevel(piter, titer);
    if (heniZoneTableTIterIsActive(titer))
    {
        if (piter->zoneLID != heniZoneTableTIterGetLID(titer) ||
                piter->zoneDiscr != heniZoneTableTIterGetDiscr(titer))
        {
            heniZoneTableTIterFinish(titer);
        }
    }
}
