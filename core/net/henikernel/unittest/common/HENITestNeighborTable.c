/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENINeighborTable.h"
#include "HENIUnitTest.h"
#include "HENICommonStubLinkAddress.h"


/**
 * @file
 * Type: heniUnitTestSynchronouslyRunAll
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


enum
{
    UT_DEF_MAX_NBT_SIZE = 13,
};


enum
{
    UT_DEF_LLA_SEED1 = 19,
    UT_DEF_LLA_SEED2 = 42,
    UT_DEF_LLA_SEED3 = 77,
    UT_DEF_LLA_SEED1A = UT_DEF_LLA_SEED1,
    UT_DEF_LLA_SEED1B = ((UT_DEF_LLA_SEED1 / 2) << 8) + ((UT_DEF_LLA_SEED1 + 1) / 2),
    UT_DEF_LLA_SEED1C = ((UT_DEF_LLA_SEED1 / 2) << 16) + (((UT_DEF_LLA_SEED1 + 1) / 2) << 8),
};


struct heni_kernel_s
{
    int PLACEHOLDER;
};

heni_kernel_t                g_utDefKernel;
heni_neighbor_table_elem_t   g_utDefNTBuffer[UT_DEF_MAX_NBT_SIZE];



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Helper functions                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_PRV_FUNCT_DEF_PREFIX uint8_t * doFillAddr(
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
    		heniNeighborTableGetBucketForLinkAddr(nbt, a2))




/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Individual tests                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_UT_FUNCT_DEF_PREFIX void
initNT_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED2)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED3)), NULL);

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOne_ShouldContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_neighbor_t               nb1;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doInitNbr(&nb1));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addTwoToDiffBuckets_ShouldContainTwoElementsInArbitraryOrder(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doFillAddr(&la2, UT_DEF_LLA_SEED2)));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doInitNbr(&nb1));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2), doInitNbr(&nb2));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb1)
    {
        heniNeighborTableTIterAdvance(&nbtti);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    }
    else
    {
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
        heniNeighborTableTIterAdvance(&nbtti);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    }
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addThreeToDiffBuckets_ShouldContainThreeElementsInArbitraryOrder(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doFillAddr(&la2, UT_DEF_LLA_SEED2)));
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2), doFillAddr(&la3, UT_DEF_LLA_SEED3)));
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3), doFillAddr(&la1, UT_DEF_LLA_SEED1)));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doInitNbr(&nb1));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3), doInitNbr(&nb3));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), &nb3);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb1)
    {
        heniNeighborTableTIterAdvance(&nbtti);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb3)
        {
            heniNeighborTableTIterAdvance(&nbtti);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
        }
        else
        {
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
            heniNeighborTableTIterAdvance(&nbtti);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
        }
    }
    else if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb2)
    {
        heniNeighborTableTIterAdvance(&nbtti);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb3)
        {
            heniNeighborTableTIterAdvance(&nbtti);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);

        }
        else
        {
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
            heniNeighborTableTIterAdvance(&nbtti);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
        }
    }
    else
    {
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
        heniNeighborTableTIterAdvance(&nbtti);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb1)
        {
            heniNeighborTableTIterAdvance(&nbtti);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
        }
        else
        {
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
            heniNeighborTableTIterAdvance(&nbtti);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
        }
    }
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addTwoToSameBucket_ShouldContainTwoElementsInComparisonOrder(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), &nb2);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
addThreeToSameBucket_ShouldContainThreeElementsInComparisonOrder(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), &nb3);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterFinish(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeThreeDirectlyFromDiffBuckets_ShouldElementsBeRemoved(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doFillAddr(&la2, UT_DEF_LLA_SEED2)));
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2), doFillAddr(&la3, UT_DEF_LLA_SEED3)));
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3), doFillAddr(&la1, UT_DEF_LLA_SEED1)));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doInitNbr(&nb1));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3), doInitNbr(&nb3));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), &nb3);

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb1)
    {
        heniNeighborTableTIterAdvance(&nbtti);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    }
    else
    {
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
        heniNeighborTableTIterAdvance(&nbtti);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    }
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeThreeDirectlyFromSameBucket_ShouldElementsBeRemoved(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), &nb3);

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), &nb3);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), NULL);
    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeThreeViaTIterFromDiffBuckets_ShouldElementsBeRemovedAndTIterAdvanced(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doFillAddr(&la2, UT_DEF_LLA_SEED2)));
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2), doFillAddr(&la3, UT_DEF_LLA_SEED3)));
    HENI_UT_CHECK(! ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3), doFillAddr(&la1, UT_DEF_LLA_SEED1)));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doInitNbr(&nb1));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3), doInitNbr(&nb3));

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), &nb3);

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb1)
    {
        heniNeighborTableTIterRemoveAndAdvance(&nbtti);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), &nb3);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb3)
        {
            heniNeighborTableTIterRemoveAndAdvance(&nbtti);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
        }
        else
        {
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
            heniNeighborTableTIterRemoveAndAdvance(&nbtti);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), &nb3);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
        }
    }
    else if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb2)
    {
        heniNeighborTableTIterRemoveAndAdvance(&nbtti);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), &nb3);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb3)
        {
            heniNeighborTableTIterRemoveAndAdvance(&nbtti);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
        }
        else
        {
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
            heniNeighborTableTIterRemoveAndAdvance(&nbtti);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), &nb3);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
        }
    }
    else
    {
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
        heniNeighborTableTIterRemoveAndAdvance(&nbtti);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
        HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
        HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
        if (heniNeighborTableTIterGetNeighbor(&nbtti) == &nb1)
        {
            heniNeighborTableTIterRemoveAndAdvance(&nbtti);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), &nb2);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
        }
        else
        {
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
            heniNeighborTableTIterRemoveAndAdvance(&nbtti);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
            HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
            HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
        }
    }
    heniNeighborTableTIterRemoveAndAdvance(&nbtti);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED2)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED3)), NULL);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeThreeViaTIterFromSameBucket_ShouldElementsBeRemovedAndTIterAdvanced(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_titer_t   nbtti2;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), &nb3);

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);

    heniNeighborTableTIterRemoveAndAdvance(&nbtti);

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), &nb1);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), NULL);
    heniNeighborTableTIterCopy(&nbtti, &nbtti2);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti2));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti2), &nb2);
    heniNeighborTableTIterAdvance(&nbtti2);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti2));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti2), &nb1);
    heniNeighborTableTIterAdvance(&nbtti2);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti2));

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterRemoveAndAdvance(&nbtti);

    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A)), NULL);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B)), &nb2);
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1C)), NULL);
    heniNeighborTableTIterCopy(&nbtti, &nbtti2);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti2));

    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithNoElements_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterFinish(&nbtti);

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithOneElement_ShouldWorkProperly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;
    heni_link_addr_container_t    la1;
    heni_neighbor_t               nb1;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1), doInitNbr(&nb1));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableFind(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1)), &nb1);

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterFinish(&nbtti);

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithElementAddedBeforeIterator_ShouldNotVisitNewElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithElementAddedAfterIterator_ShouldVisitNewElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithElementAddedAtIterator_ShouldVisitNewElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;
    heni_neighbor_t               nb4;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterRemoveAndAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb4));

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb4);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb4);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithElementRemovedBeforeIterator_ShouldVisitRemainingElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C));

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithElementRemovedAfterIterator_ShouldVisitRemainingElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableRemoveExisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A));

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



HENI_UT_FUNCT_DEF_PREFIX void
iterConversionsWithElementRemovedAtIterator_ShouldVisitRemainingElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_t         nbt;
    heni_neighbor_table_titer_t   nbtti;
    heni_neighbor_table_piter_t   nbtpi;
    heni_link_addr_container_t    la1;
    heni_link_addr_container_t    la2;
    heni_link_addr_container_t    la3;
    heni_neighbor_t               nb1;
    heni_neighbor_t               nb2;
    heni_neighbor_t               nb3;

    heniNeighborTableInit(&nbt, &g_utDefKernel, g_utDefNTBuffer, UT_DEF_MAX_NBT_SIZE);

    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)));
    HENI_UT_CHECK(ADDR_FALL_TO_SAME_BUCKET(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)));
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la1, UT_DEF_LLA_SEED1A), doFillAddr(&la2, UT_DEF_LLA_SEED1B)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la2, UT_DEF_LLA_SEED1B), doFillAddr(&la3, UT_DEF_LLA_SEED1C)) > 0);
    HENI_UT_CHECK(heniLinkAddrCmp(doFillAddr(&la3, UT_DEF_LLA_SEED1C), doFillAddr(&la1, UT_DEF_LLA_SEED1A)) < 0);

    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la3, UT_DEF_LLA_SEED1C), doInitNbr(&nb3));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la2, UT_DEF_LLA_SEED1B), doInitNbr(&nb2));
    heniNeighborTableAddNonexisting(&nbt, doFillAddr(&la1, UT_DEF_LLA_SEED1A), doInitNbr(&nb1));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb2);

    heniNeighborTableTIterToPIter(&nbtti, &nbtpi);

    heniNeighborTableTIterRemoveAndAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTablePIterToTIter(&nbtpi, &nbtti);

    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableTIterInit(&nbtti, &nbt);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb3);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(heniNeighborTableTIterIsActive(&nbtti));
    HENI_UT_CHECK_PTR_EQ(heniNeighborTableTIterGetNeighbor(&nbtti), &nb1);
    heniNeighborTableTIterAdvance(&nbtti);
    HENI_UT_CHECK(! heniNeighborTableTIterIsActive(&nbtti));

    heniNeighborTableCleanup(&nbt);
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          The main test method                          *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_EXT_FUNCT_DEF_PREFIX void heniUnitTestSynchronouslyRunAll(
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    HENI_UT_RUN_TEST(initNT_ShouldContainNoElements);
    HENI_UT_RUN_TEST(addOne_ShouldContainOneElement);
    HENI_UT_RUN_TEST(addTwoToDiffBuckets_ShouldContainTwoElementsInArbitraryOrder);
    HENI_UT_RUN_TEST(addThreeToDiffBuckets_ShouldContainThreeElementsInArbitraryOrder);
    HENI_UT_RUN_TEST(addTwoToSameBucket_ShouldContainTwoElementsInComparisonOrder);
    HENI_UT_RUN_TEST(addThreeToSameBucket_ShouldContainThreeElementsInComparisonOrder);
    HENI_UT_RUN_TEST(removeThreeDirectlyFromDiffBuckets_ShouldElementsBeRemoved);
    HENI_UT_RUN_TEST(removeThreeDirectlyFromSameBucket_ShouldElementsBeRemoved);
    HENI_UT_RUN_TEST(removeThreeViaTIterFromDiffBuckets_ShouldElementsBeRemovedAndTIterAdvanced);
    HENI_UT_RUN_TEST(removeThreeViaTIterFromSameBucket_ShouldElementsBeRemovedAndTIterAdvanced);
    HENI_UT_RUN_TEST(iterConversionsWithNoElements_ShouldWorkProperly);
    HENI_UT_RUN_TEST(iterConversionsWithOneElement_ShouldWorkProperly);
    HENI_UT_RUN_TEST(iterConversionsWithElementAddedBeforeIterator_ShouldNotVisitNewElement);
    HENI_UT_RUN_TEST(iterConversionsWithElementAddedAfterIterator_ShouldVisitNewElement);
    HENI_UT_RUN_TEST(iterConversionsWithElementAddedAtIterator_ShouldVisitNewElement);
    HENI_UT_RUN_TEST(iterConversionsWithElementRemovedBeforeIterator_ShouldVisitRemainingElements);
    HENI_UT_RUN_TEST(iterConversionsWithElementRemovedAfterIterator_ShouldVisitRemainingElements);
    HENI_UT_RUN_TEST(iterConversionsWithElementRemovedAtIterator_ShouldVisitRemainingElements);
}
