/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIZoneTable.h"
#include "HENIUnitTest.h"


/**
 * @file
 * Type: heniUnitTestSynchronouslyRunAll
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

enum
{
    UT_MAX_ZT_ROWS = 16,
    UT_DEF_ZT_BUCKETS = 3,
    UT_DEF_LSPEC = 4,
    UT_DEF_DISCRBITS = 4,
};

enum
{
    UT_SLID1 = 3,
    UT_SLID2 = 4,
    UT_SLID3 = 5,
};

heni_instance_t                g_utDefInstance;
heni_zone_table_bucket_t       g_utDefZoneTableAllBuckets[UT_MAX_ZT_ROWS * UT_DEF_ZT_BUCKETS];
heni_zone_table_bucket_t *     g_utDefZoneTableBuckets[UT_MAX_ZT_ROWS];



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Helper functions                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_PRV_FUNCT_DEF_PREFIX heni_level_t utMaxLevel() HENI_PRV_FUNCT_DEF_SUFFIX
{
    heni_lspec_t      lspec = heniKernelInstanceGetLabelSpecForInstancePtr(&g_utDefInstance);
    return heniLabelSpecGetNumLevels(lspec);
}


HENI_PRV_FUNCT_DEF_PREFIX heni_instance_t * doInitInst() HENI_PRV_FUNCT_DEF_SUFFIX
{
    g_utDefInstance.ker = NULL;
    g_utDefInstance.iid = 1;
    g_utDefInstance.lspec = UT_DEF_LSPEC;
    g_utDefInstance.logNumZoneDiscrBitsPlusOne = UT_DEF_DISCRBITS;
    return &g_utDefInstance;
}

HENI_PRV_FUNCT_DEF_PREFIX heni_zone_table_bucket_t * * doInitRows(
        heni_level_t numRows,
        size_t bucketsPerRow
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    heni_level_t   level;
    HENI_PASSERT(numRows > 0);
    HENI_PASSERT(bucketsPerRow > 0);
    for (level = 0; level < numRows; ++level)
    {
        g_utDefZoneTableBuckets[level] = &(g_utDefZoneTableAllBuckets[level * UT_DEF_ZT_BUCKETS]);
    }
    return &(g_utDefZoneTableBuckets[0]);
}

HENI_PRV_FUNCT_DEF_PREFIX heni_zone_t * doInitZone(
        heni_zone_t * zone
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    return zone;
}

HENI_PRV_FUNCT_DEF_PREFIX heni_zone_lid_t sampleLID(
        uint8_t seed
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    heni_lspec_t      lspec = heniKernelInstanceGetLabelSpecForInstancePtr(&g_utDefInstance);
    heni_zone_lid_t   lid = heniLabelSpecGetZoneLIDMinAssignable(lspec) + seed;
    HENI_PASSERT(heniLabelSpecIsZoneLIDAssignable(lid, lspec));
    return lid;
}

HENI_PRV_FUNCT_DEF_PREFIX heni_zone_lid_t invalidLID() HENI_PRV_FUNCT_DEF_SUFFIX
{
    heni_lspec_t   lspec = heniKernelInstanceGetLabelSpecForInstancePtr(&g_utDefInstance);
    return heniLabelSpecGetZoneLIDInvalid(lspec);
}

HENI_PRV_FUNCT_DEF_PREFIX heni_zone_discr_t sampleDiscr(
        uint8_t seed
) HENI_PRV_FUNCT_DEF_SUFFIX
{

    uint8_t             numBits = heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(&g_utDefInstance);
    heni_zone_discr_t   discr = heniZoneDiscrSpecGetZoneDiscrMinAssignable(numBits) + seed;
    HENI_PASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(discr, numBits));
    return discr;
}

/*HENI_PRV_FUNCT_DEF_PREFIX uint8_t * doFillAddr(
        heni_link_addr_container_t * cont,
        size_t seed
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    heniLinkAddrStubFill(&(cont->data8[0]), seed);
    return &(cont->data8[0]);
}



HENI_PRV_FUNCT_DEF_PREFIX heni_neighbor_t * doInitNbr(
        heni_neighbor_t * nbr
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    return nbr;
}



#define ADDR_FALL_TO_SAME_BUCKET(nbt, a1, a2) \
    (heniNeighborTableGetBucketForLinkAddr(nbt, a1) == \
    		heniNeighborTableGetBucketForLinkAddr(nbt, a2))*/




/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Individual tests                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_UT_FUNCT_DEF_PREFIX void
init_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_level_t              level, maxLevel;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);

    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 0, sampleLID(0), sampleDiscr(0)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 1, sampleLID(1), sampleDiscr(1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(2), sampleDiscr(2)), NULL);

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneAtLevelZero_ShouldContainOneElementAtLevelZero(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_table_titer_t   ztib;
    heni_zone_t               ze1;
    heni_level_t              level, maxLevel;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 0, sampleLID(0), sampleDiscr(0)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 1, sampleLID(1), sampleDiscr(1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(2), sampleDiscr(2)), NULL);
    for (level = 0, maxLevel = utMaxLevel(); level < maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }

    heniZoneTableAddNonexisting(&zt, 0, invalidLID(), sampleDiscr(0), doInitZone(&ze1));

    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 0, sampleLID(0), sampleDiscr(0)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 1, sampleLID(1), sampleDiscr(1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(2), sampleDiscr(2)), NULL);

    heniZoneTableTIterSetAtLevel(&zti, &zt, 0);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
    HENI_UT_CHECK_EQ(heniZoneTableTIterGetLevel(&zti), 0, HENI_PRI_LEVEL);
    HENI_UT_CHECK_EQ(heniZoneTableTIterGetLID(&zti), invalidLID(), HENI_PRI_ZONE_LID);
    HENI_UT_CHECK_EQ(heniZoneTableTIterGetDiscr(&zti), sampleDiscr(0), HENI_PRI_ZONE_DISCR);

    heniZoneTableTIterCopy(&zti, &ztib);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&ztib), &ze1);
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterCopy(&ztib, &zti);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    for (level = 1, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneAtLevelNonzero_ShouldContainOneElementAtLevelNonzero(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    /*   level == 2   */
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_table_titer_t   ztib;
    heni_zone_t               ze1;
    heni_level_t              level, maxLevel;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 0, sampleLID(1), sampleDiscr(3)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 1, sampleLID(1), sampleDiscr(3)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1), sampleDiscr(3)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 3, sampleLID(1), sampleDiscr(3)), NULL);
    for (level = 0, maxLevel = utMaxLevel(); level < maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&ze1));

    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 0, sampleLID(1), sampleDiscr(3)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 1, sampleLID(1), sampleDiscr(3)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1), sampleDiscr(3)), &ze1);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 3, sampleLID(1), sampleDiscr(3)), NULL);

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        if (level != 2)
        {
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            HENI_UT_CHECK_EQ(heniZoneTableTIterGetLevel(&zti), 2, HENI_PRI_LEVEL);
            HENI_UT_CHECK_EQ(heniZoneTableTIterGetLID(&zti), sampleLID(1), HENI_PRI_ZONE_LID);
            HENI_UT_CHECK_EQ(heniZoneTableTIterGetDiscr(&zti), sampleDiscr(3), HENI_PRI_ZONE_DISCR);

            heniZoneTableTIterCopy(&zti, &ztib);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&ztib), &ze1);
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

            heniZoneTableTIterCopy(&ztib, &zti);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addFourAtDifferentLevels_ShouldEachLevelContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_table_titer_t   ztib;
    heni_zone_t               ze[4];
    heni_level_t              level, maxLevel;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    for (level = 0, maxLevel = utMaxLevel(); level < maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }

    heniZoneTableAddNonexisting(&zt, 0, invalidLID(), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 1, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 3, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));

    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 0, invalidLID(), sampleDiscr(3)), &(ze[0]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 1, sampleLID(1), sampleDiscr(3)), &(ze[1]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1), sampleDiscr(3)), &(ze[2]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 3, sampleLID(2), sampleDiscr(3)), &(ze[3]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 3, sampleLID(1), sampleDiscr(3)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 3, sampleLID(2), sampleDiscr(2)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 4, sampleLID(1), sampleDiscr(3)), NULL);

    for (level = 0; level <= 3; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
        HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[level]));
        heniZoneTableTIterCopy(&zti, &ztib);
        HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&ztib), &(ze[level]));
        heniZoneTableTIterAdvancePreservingLevel(&zti);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        heniZoneTableTIterCopy(&ztib, &zti);
        HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[level]));
        heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }
    for (maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addThreeAtSameLevelButDifferentBuckets_ShouldLevelContainAll(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_table_titer_t   ztib;
    heni_zone_t               ze[4];
    heni_level_t              level, maxLevel;


    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(3)));
    for (level = 0, maxLevel = utMaxLevel(); level < maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(3), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 3, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));

    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1), sampleDiscr(3)), &(ze[0]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(2), sampleDiscr(3)), &(ze[1]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(3), sampleDiscr(3)), &(ze[2]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 3, sampleLID(2), sampleDiscr(3)), &(ze[3]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(4), sampleDiscr(3)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 3, sampleLID(2), sampleDiscr(2)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 4, sampleLID(1), sampleDiscr(3)), NULL);

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        if (level == 2)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterCopy(&zti, &ztib);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&ztib), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            heniZoneTableTIterCopy(&ztib, &zti);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else if (level == 3)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
            heniZoneTableTIterCopy(&zti, &ztib);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&ztib), &(ze[3]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            heniZoneTableTIterCopy(&ztib, &zti);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
            heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addThreeToSameBucket_ShouldLevelContainAllInOrder(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_table_titer_t   ztib;
    heni_zone_t               ze[4];
    heni_level_t              level, maxLevel;


    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    for (level = 0, maxLevel = utMaxLevel(); level < maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));

    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS), sampleDiscr(3)), &(ze[0]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(3)), &(ze[1]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS), sampleDiscr(3)), &(ze[2]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(2), sampleDiscr(3)), &(ze[3]));

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        if (level == 2)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterCopy(&zti, &ztib);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&ztib), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            heniZoneTableTIterCopy(&ztib, &zti);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addThreeWithSameLID_ShouldLevelContainAllInOrder(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_table_titer_t   ztib;
    heni_zone_t               ze[4];
    heni_level_t              level, maxLevel;


    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    for (level = 0, maxLevel = utMaxLevel(); level < maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS), sampleDiscr(1), doInitZone(&(ze[3])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(2), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(4), doInitZone(&(ze[2])));

    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1), sampleDiscr(2)), &(ze[0]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1), sampleDiscr(3)), &(ze[1]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1), sampleDiscr(4)), &(ze[2]));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableFind(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS), sampleDiscr(1)), &(ze[3]));

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        if (level == 2)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterCopy(&zti, &ztib);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&ztib), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            heniZoneTableTIterCopy(&ztib, &zti);
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
            heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
            heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeDirectlySoleFromBucket_ShouldRemovedElementBeAbsent(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_t               ze[4];
    heni_level_t              level, maxLevel;


    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(3)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(3), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 3, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));
    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        if (level == 2)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else if (level == 3)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }
    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(2));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableRemoveExisting(&zt, 2, sampleLID(2), sampleDiscr(3));

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        if (level == 2)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else if (level == 3)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }
    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(2));
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeDirectlyFromLongBucket_ShouldRemovedElementsBeAbsent(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_t               ze[4];
    heni_level_t              level, maxLevel;


    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));
    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        if (level == 2)
        {
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }

    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(3));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS));
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));


    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS), sampleDiscr(3));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS));
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeDirectlyWithSameLID_ShouldRemovedElementsBeAbsent(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_t               ze[4];

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(2), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(4), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS), sampleDiscr(1), doInitZone(&(ze[3])));
    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1), sampleDiscr(3));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(2));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(3));
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(4));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1), sampleDiscr(4));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(2));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(4));
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeViaTIterSoleFromBucket_ShouldRemovedElementBeAbsent(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_t               ze[4];

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(3)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(3), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 3, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));
    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableTIterSetAtLevel(&zti, &zt, 3);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(2));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);

    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableTIterSetAtLevel(&zti, &zt, 3);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(4), doInitZone(&(ze[1])));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(2), sampleDiscr(4));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(&zti);

    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeViaTIterFromLongBucket_ShouldRemovedElementsBeAbsent(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_t               ze[4];

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));
    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeViaTIterWithSameLID_ShouldRemovedElementsBeAbsent(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_t               ze[4];

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(2), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(4), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS), sampleDiscr(1), doInitZone(&(ze[3])));
    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(3));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(&zti);

    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithNoEntries_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_table_titer_t   zti;
    heni_zone_table_piter_t   ztpi;
    heni_level_t              level, maxLevel;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        heniZoneTableTIterSetAtLevel(&zti, &zt, level);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        heniZoneTableTIterToPIter(&zti, &ztpi);
        memset(&zti, 0x77, sizeof(zti));
        heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
        HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithOneEntryAtLevelZeroAndRemovals_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_t               ze1;
    heni_zone_table_titer_t   zti;
    heni_zone_table_piter_t   ztpi;
    heni_level_t              level, maxLevel;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    heniZoneTableAddNonexisting(&zt, 0, invalidLID(), sampleDiscr(0), doInitZone(&ze1));

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        if (level == 0)
        {
            heniZoneTableTIterSetAtLevel(&zti, &zt, level);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterToPIter(&zti, &ztpi);
            memset(&zti, 0x77, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            memset(&zti, 0x33, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            memset(&zti, 0x55, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);

            heniZoneTableTIterAdvancePreservingLevel(&zti);
            memset(&ztpi, 0x11, sizeof(ztpi));
            heniZoneTableTIterToPIter(&zti, &ztpi);
            memset(&zti, 0x22, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x44, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x44, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));

            heniZoneTableTIterSetAtLevel(&zti, &zt, level);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterToPIter(&zti, &ztpi);

            heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x66, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));

            heniZoneTableAddNonexisting(&zt, 0, invalidLID(), sampleDiscr(0), doInitZone(&ze1));
            memset(&zti, 0x88, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
        else
        {
            heniZoneTableTIterSetAtLevel(&zti, &zt, level);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            heniZoneTableTIterToPIter(&zti, &ztpi);
            memset(&zti, 0x77, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithOneEntryAtLevelNonzeroAndRemovals_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_t               ze1;
    heni_zone_table_titer_t   zti;
    heni_zone_table_piter_t   ztpi;
    heni_level_t              level, maxLevel;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    heniZoneTableAddNonexisting(&zt, 3, sampleLID(2), sampleDiscr(1), doInitZone(&ze1));

    for (level = 0, maxLevel = utMaxLevel(); level <= maxLevel; ++level)
    {
        if (level == 3)
        {
            heniZoneTableTIterSetAtLevel(&zti, &zt, level);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterToPIter(&zti, &ztpi);
            memset(&zti, 0x77, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            memset(&zti, 0x33, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            memset(&zti, 0x55, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);

            heniZoneTableTIterAdvancePreservingLevel(&zti);
            memset(&ztpi, 0x11, sizeof(ztpi));
            heniZoneTableTIterToPIter(&zti, &ztpi);
            memset(&zti, 0x22, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x44, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x44, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));

            heniZoneTableTIterSetAtLevel(&zti, &zt, level);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterToPIter(&zti, &ztpi);

            heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x66, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(!heniZoneTableTIterIsActive(&zti));

            heniZoneTableAddNonexisting(&zt, level, sampleLID(2), sampleDiscr(1), doInitZone(&ze1));
            memset(&zti, 0x88, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

            heniZoneTableAddNonexisting(&zt, level, sampleLID(2), sampleDiscr(2), doInitZone(&ze1));
            memset(&zti, 0x11, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x22, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

            heniZoneTableAddNonexisting(&zt, level, sampleLID(2), sampleDiscr(0), doInitZone(&ze1));
            memset(&zti, 0x11, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x22, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            heniZoneTableRemoveExisting(&zt, level, sampleLID(2), sampleDiscr(0));

            heniZoneTableAddNonexisting(&zt, level, sampleLID(3), sampleDiscr(1), doInitZone(&ze1));
            memset(&zti, 0x33, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x44, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x55, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

            heniZoneTableAddNonexisting(&zt, level + 1, sampleLID(2), sampleDiscr(1), doInitZone(&ze1));
            memset(&zti, 0x66, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x77, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            memset(&zti, 0x88, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

            heniZoneTableTIterSetAtLevel(&zti, &zt, level + 1);
            HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
            HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &ze1);
            heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
            heniZoneTableAddNonexisting(&zt, level, sampleLID(2), sampleDiscr(1), doInitZone(&ze1));
        }
        else
        {
            heniZoneTableTIterSetAtLevel(&zti, &zt, level);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
            heniZoneTableTIterToPIter(&zti, &ztpi);
            memset(&zti, 0x77, sizeof(zti));
            heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
            HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
        }
    }

    heniZoneTableCleanup(&zt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithEntriesInDifferentBucketsAndRemovals_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_t               ze[4];
    heni_zone_table_titer_t   zti;
    heni_zone_table_piter_t   ztpi;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 1, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 1, sampleLID(2)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 1, sampleLID(2)) < heniZoneTableGetBucketForLevelAndLID(&zt, 1, sampleLID(3)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 1, sampleLID(1)) == heniZoneTableGetBucketForLevelAndLID(&zt, 1, sampleLID(1 + UT_DEF_ZT_BUCKETS)));
    heniZoneTableAddNonexisting(&zt, 1, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 1, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 1, sampleLID(3), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 3, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 1, sampleLID(2));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterToPIter(&zti, &ztpi);
    memset(&zti, 0x11, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x22, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x33, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableTIterAdvancePreservingLevel(&zti);
    memset(&ztpi, 0x44, sizeof(ztpi));
    heniZoneTableTIterToPIter(&zti, &ztpi);
    memset(&zti, 0x55, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x66, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x77, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 1, sampleLID(2));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterToPIter(&zti, &ztpi);

    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x88, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x99, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    memset(&zti, 0xaa, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableAddNonexisting(&zt, 1, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[1])));
    memset(&zti, 0xbb, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xcc, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xdd, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));

    heniZoneTableAddNonexisting(&zt, 1, sampleLID(2), sampleDiscr(4), doInitZone(&(ze[1])));
    memset(&zti, 0xee, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xff, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x11, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableRemoveExisting(&zt, 1, sampleLID(2), sampleDiscr(4));

    heniZoneTableAddNonexisting(&zt, 1, sampleLID(2), sampleDiscr(2), doInitZone(&(ze[1])));
    memset(&zti, 0x22, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x33, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    memset(&zti, 0x44, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableRemoveExisting(&zt, 1, sampleLID(2), sampleDiscr(2));

    memset(&zti, 0x55, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x66, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    memset(&zti, 0x77, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableRemoveExisting(&zt, 1, sampleLID(3), sampleDiscr(3));
    memset(&zti, 0x88, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableTIterSetAtLevel(&zti, &zt, 1);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithEntriesInLongBucketAndRemovals_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_t               ze[4];
    heni_zone_table_titer_t   zti;
    heni_zone_table_piter_t   ztpi;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 0 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(2), sampleDiscr(3), doInitZone(&(ze[3])));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterToPIter(&zti, &ztpi);
    memset(&zti, 0x11, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x22, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x33, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableTIterAdvancePreservingLevel(&zti);
    memset(&ztpi, 0x44, sizeof(ztpi));
    heniZoneTableTIterToPIter(&zti, &ztpi);
    memset(&zti, 0x55, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x66, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x77, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));

    heniZoneTableTIterSetAtLevelAndLID(&zti, &zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterToPIter(&zti, &ztpi);

    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x88, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x99, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    memset(&zti, 0xaa, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(3), doInitZone(&(ze[1])));
    memset(&zti, 0xbb, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xcc, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xdd, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(4), doInitZone(&(ze[1])));
    memset(&zti, 0xee, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xff, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x11, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(4));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(2), doInitZone(&(ze[1])));
    memset(&zti, 0x22, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x33, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    memset(&zti, 0x44, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1 + 1 * UT_DEF_ZT_BUCKETS), sampleDiscr(2));

    memset(&zti, 0x55, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x66, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    memset(&zti, 0x77, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1 + 2 * UT_DEF_ZT_BUCKETS), sampleDiscr(3));
    memset(&zti, 0x88, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    memset(&zti, 0x99, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithEntriesWithSameLIDAndRemovals_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_zone_table_t         zt;
    heni_zone_t               ze[4];
    heni_zone_table_titer_t   zti;
    heni_zone_table_piter_t   ztpi;

    heniZoneTableInit(&zt, doInitInst(), doInitRows(UT_MAX_ZT_ROWS, UT_DEF_ZT_BUCKETS), UT_DEF_ZT_BUCKETS);
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) == heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS)));
    HENI_UT_CHECK(heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(1)) < heniZoneTableGetBucketForLevelAndLID(&zt, 2, sampleLID(2)));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(1), doInitZone(&(ze[0])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[1])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(5), doInitZone(&(ze[2])));
    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1 + UT_DEF_ZT_BUCKETS), sampleDiscr(1), doInitZone(&(ze[3])));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(3));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterToPIter(&zti, &ztpi);
    memset(&zti, 0x11, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x22, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0x33, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));

    heniZoneTableTIterAdvancePreservingLevel(&zti);
    memset(&ztpi, 0x44, sizeof(ztpi));
    heniZoneTableTIterToPIter(&zti, &ztpi);
    memset(&zti, 0x55, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x66, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x77, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));

    heniZoneTableTIterSetAtLevelAndLIDAndDiscr(&zti, &zt, 2, sampleLID(1), sampleDiscr(3));
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterToPIter(&zti, &ztpi);

    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x88, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x99, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0xaa, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(3), doInitZone(&(ze[1])));
    memset(&zti, 0xbb, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xcc, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xdd, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(6), doInitZone(&(ze[1])));
    memset(&zti, 0xee, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0xff, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x11, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1), sampleDiscr(6));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(4), doInitZone(&(ze[1])));
    memset(&zti, 0xbb, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xcc, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[1]));
    memset(&zti, 0xdd, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1), sampleDiscr(4));

    heniZoneTableAddNonexisting(&zt, 2, sampleLID(1), sampleDiscr(0), doInitZone(&(ze[1])));
    memset(&zti, 0x22, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x33, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x44, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1), sampleDiscr(0));

    memset(&zti, 0x55, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x66, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLID(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[2]));
    memset(&zti, 0x77, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    heniZoneTableRemoveExisting(&zt, 2, sampleLID(1), sampleDiscr(5));
    memset(&zti, 0x88, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));

    heniZoneTableTIterSetAtLevel(&zti, &zt, 2);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[0]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(heniZoneTableTIterIsActive(&zti));
    HENI_UT_CHECK_PTR_EQ(heniZoneTableTIterGetZone(&zti), &(ze[3]));
    heniZoneTableTIterRemoveAndAdvancePreservingLevel(&zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));

    memset(&zti, 0x99, sizeof(zti));
    heniZoneTablePIterToTIterPreservingLevel(&ztpi, &zti);
    HENI_UT_CHECK(! heniZoneTableTIterIsActive(&zti));
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          The main test method                          *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_EXT_FUNCT_DEF_PREFIX void heniUnitTestSynchronouslyRunAll(
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    HENI_UT_RUN_TEST(init_ShouldContainNoElements);
    HENI_UT_RUN_TEST(addOneAtLevelZero_ShouldContainOneElementAtLevelZero);
    HENI_UT_RUN_TEST(addOneAtLevelNonzero_ShouldContainOneElementAtLevelNonzero);
    HENI_UT_RUN_TEST(addFourAtDifferentLevels_ShouldEachLevelContainOneElement);
    HENI_UT_RUN_TEST(addThreeAtSameLevelButDifferentBuckets_ShouldLevelContainAll);
    HENI_UT_RUN_TEST(addThreeToSameBucket_ShouldLevelContainAllInOrder);
    HENI_UT_RUN_TEST(addThreeWithSameLID_ShouldLevelContainAllInOrder);
    HENI_UT_RUN_TEST(removeDirectlySoleFromBucket_ShouldRemovedElementBeAbsent);
    HENI_UT_RUN_TEST(removeDirectlyFromLongBucket_ShouldRemovedElementsBeAbsent);
    HENI_UT_RUN_TEST(removeDirectlyWithSameLID_ShouldRemovedElementsBeAbsent);
    HENI_UT_RUN_TEST(removeViaTIterSoleFromBucket_ShouldRemovedElementBeAbsent);
    HENI_UT_RUN_TEST(removeViaTIterFromLongBucket_ShouldRemovedElementsBeAbsent);
    HENI_UT_RUN_TEST(removeViaTIterWithSameLID_ShouldRemovedElementsBeAbsent);
    HENI_UT_RUN_TEST(iterConversionsWithNoEntries_ShouldWorkProperly);
    HENI_UT_RUN_TEST(iterConversionsWithOneEntryAtLevelZeroAndRemovals_ShouldWorkProperly);
    HENI_UT_RUN_TEST(iterConversionsWithOneEntryAtLevelNonzeroAndRemovals_ShouldWorkProperly);
    HENI_UT_RUN_TEST(iterConversionsWithEntriesInDifferentBucketsAndRemovals_ShouldWorkProperly);
    HENI_UT_RUN_TEST(iterConversionsWithEntriesInLongBucketAndRemovals_ShouldWorkProperly);
    HENI_UT_RUN_TEST(iterConversionsWithEntriesWithSameLIDAndRemovals_ShouldWorkProperly);
}
