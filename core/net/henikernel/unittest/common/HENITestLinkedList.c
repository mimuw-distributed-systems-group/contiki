/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENILinkedList.h"
#include "HENIUnitTest.h"

/**
 * @file
 * Type: heniUnitTestSynchronouslyRunAll
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


typedef struct ut_list_node_s
{
    int                       val;
    heni_linked_list_node_t   lnode;
} ut_list_node_t;


#define UT_PTR_UT2LN(utn) (&((utn)->lnode))
#define UT_PTR_LN2UT(ln) ((ut_list_node_t *)((uint8_t *)(ln) - offsetof(ut_list_node_t, lnode)))
#define UT_PTR_INI(utn, v) do { (utn)->val = v; heniLinkedListNodeInit(UT_PTR_UT2LN(utn)); } while (0)


/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Individual tests                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_UT_FUNCT_DEF_PREFIX void
initList_ShouldBeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);

    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneFront_ShouldContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneBack_ShouldContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
addTwoFront_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 41);
    UT_PTR_INI(&(lns[1]), 42);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );

    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
addTwoBack_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 51);
    UT_PTR_INI(&(lns[1]), 52);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );

    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneFrontOneBack_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );

    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListRIterAdvance(&ri);


    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneBackOneFront_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 31);
    UT_PTR_INI(&(lns[1]), 32);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[1]) );
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAdvance(&ri);


    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddBeforeToEmpty_ShouldContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterAddBeforeWithoutMoving(&fi, UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));    

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddBeforeToEmpty_ShouldContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterAddBeforeAndAdvance(&ri, UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));    
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddAfterToEmpty_ShouldContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterAddAfterAndAdvance(&fi, UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));    
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddAfterToEmpty_ShouldContainOneElement(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 31);
    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterAddAfterWithoutMoving(&ri, UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));    

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddBeforeNothingToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);
    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterAddBeforeWithoutMoving(&fi, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );

    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddBeforeNothingToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri, &ll);
    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterAddBeforeAndAdvance(&ri, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[1]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddBeforeElementToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAddBeforeWithoutMoving(&fi, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[1]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddBeforeElementToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAddBeforeAndAdvance(&ri, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[1]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddAfterNothingToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);
    heniLinkedListFIterAdvance(&fi);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterAddAfterAndAdvance(&fi, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddAfterElementToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAddAfterAndAdvance(&fi, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddAfterNothingToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri, &ll);
    heniLinkedListRIterAdvance(&ri);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterAddAfterWithoutMoving(&ri, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[1]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );

    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));


    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddAfterElementToOneElement_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAddAfterWithoutMoving(&ri, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddBetweenTwoElementsWithBefore_ShouldContainThreeElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );

    heniLinkedListFIterAddBeforeWithoutMoving(&fi, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddBetweenTwoElementsWithBefore_ShouldContainThreeElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );

    heniLinkedListRIterAddBeforeAndAdvance(&ri, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAddBetweenTwoElementsWithAfter_ShouldContainThreeElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    UT_PTR_INI(&(lns[2]), 23);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterAddAfterAndAdvance(&fi, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterAddBetweenTwoElementsWithAfter_ShouldContainThreeElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 31);
    UT_PTR_INI(&(lns[1]), 32);
    UT_PTR_INI(&(lns[2]), 33);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterAddAfterWithoutMoving(&ri, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterFinishIterationEarly_ShouldIterationFinish(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterFinish(&fi);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );
}



HENI_UT_FUNCT_DEF_PREFIX void
riterFinishIterationEarly_ShouldIterationFinish(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );

    heniLinkedListRIterFinish(&ri);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyInactive_ShouldCopyBeInactive(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi1;
    heni_linked_list_fiter_t   fi2;

    heniLinkedListInit(&ll);
    heniLinkedListFIterInit(&fi1, &ll);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi1), &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi1), NULL );

    heniLinkedListFIterCopy(&fi1, &fi2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi1), &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi1), NULL );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetList(&fi1), heniLinkedListFIterGetList(&fi2) );
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi2));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi1), heniLinkedListFIterGetNode(&fi2) );
}



HENI_UT_FUNCT_DEF_PREFIX void
riterCopyInactive_ShouldCopyBeInactive(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t         ll;
    heni_linked_list_riter_t   ri1;
    heni_linked_list_riter_t   ri2;

    heniLinkedListInit(&ll);
    heniLinkedListRIterInit(&ri1, &ll);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri1), &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri1), NULL );

    heniLinkedListRIterCopy(&ri1, &ri2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri1), &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri1), NULL );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetList(&ri1), heniLinkedListRIterGetList(&ri2) );
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri2));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri1), heniLinkedListRIterGetNode(&ri2) );
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyActive_ShouldCopyBeEqual(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi1;
    heni_linked_list_fiter_t   fi2;

    UT_PTR_INI(&(lns[0]), 21);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi1, &ll);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi1), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi1), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterCopy(&fi1, &fi2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi1), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi1), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetList(&fi1), heniLinkedListFIterGetList(&fi2) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi2));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi1), heniLinkedListFIterGetNode(&fi2) );
}



HENI_UT_FUNCT_DEF_PREFIX void
riterCopyActive_ShouldCopyBeEqual(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_riter_t   ri1;
    heni_linked_list_riter_t   ri2;

    UT_PTR_INI(&(lns[0]), 31);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri1, &ll);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri1), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri1), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterCopy(&ri1, &ri2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri1), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri1));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri1), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetList(&ri1), heniLinkedListRIterGetList(&ri2) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri2));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri1), heniLinkedListRIterGetNode(&ri2) );
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterInactiveToRIter_ShouldRIterBeInactive(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);
    heniLinkedListFIterFinish(&fi);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterToRIter(&fi, &ri);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetList(&ri), heniLinkedListFIterGetList(&fi) );
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), heniLinkedListFIterGetNode(&fi) );
}



HENI_UT_FUNCT_DEF_PREFIX void
riterInactiveToFIter_ShouldFIterBeInactive(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri, &ll);
    heniLinkedListRIterFinish(&ri);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterToFIter(&ri, &fi);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetList(&fi), heniLinkedListRIterGetList(&ri) );
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), heniLinkedListRIterGetNode(&ri) );
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterActiveToRIter_ShouldRIterBeActiveAndPointSameNodeAndList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterToRIter(&fi, &ri);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetList(&ri), heniLinkedListFIterGetList(&fi) );
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), heniLinkedListFIterGetNode(&fi) );
}



HENI_UT_FUNCT_DEF_PREFIX void
riterActiveToFIter_ShouldFIterBeInactiveAndPointSameNodeAndList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterToFIter(&ri, &fi);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetList(&fi), heniLinkedListRIterGetList(&ri) );
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), heniLinkedListRIterGetNode(&ri) );
}



HENI_UT_FUNCT_DEF_PREFIX void
tryRemoveFromFrontAndBackOfEmptyList_ShouldDoNothing(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveFront(&ll), NULL );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveBack(&ll), NULL );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
removeFrontOfOne_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveFront(&ll), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
removeBackOfOne_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_NE(heniLinkedListNodeGetBack(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveBack(&ll), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
removeFrontOfTwo_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveFront(&ll), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[1]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveFront(&ll), UT_PTR_UT2LN(&(lns[1])) );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
removeBackOfTwo_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveBack(&ll), UT_PTR_UT2LN(&(lns[1])) );

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveBack(&ll), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
removeFrontBackOfTwo_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveFront(&ll), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[1]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveBack(&ll), UT_PTR_UT2LN(&(lns[1])) );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
removeBackFrontOfTwo_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveBack(&ll), UT_PTR_UT2LN(&(lns[1])) );

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    HENI_UT_CHECK_PTR_EQ( heniLinkedListNodeTryRemoveFront(&ll), UT_PTR_UT2LN(&(lns[0])) );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[0]))));
    HENI_UT_CHECK(! heniLinkedListContainsNode(&ll, UT_PTR_UT2LN(&(lns[1]))));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterRemoveFromOneElement_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListNodeAddFront(&ll, UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterRemoveAndAdvance(&fi);

    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterRemoveFromOneElement_ShouldContainNoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[0]) );
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterRemoveAndAdvance(&ri);

    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), NULL);
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterRemoveFrontFromThreeElements_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    UT_PTR_INI(&(lns[2]), 23);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListFIterRemoveAndAdvance(&fi);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterRemoveFrontFromThreeElements_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 31);
    UT_PTR_INI(&(lns[1]), 32);
    UT_PTR_INI(&(lns[2]), 33);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );

    heniLinkedListRIterRemoveAndAdvance(&ri);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterRemoveMiddleFromThreeElements_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 41);
    UT_PTR_INI(&(lns[1]), 42);
    UT_PTR_INI(&(lns[2]), 43);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListFIterRemoveAndAdvance(&fi);

    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterRemoveMiddleFromThreeElements_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 51);
    UT_PTR_INI(&(lns[1]), 52);
    UT_PTR_INI(&(lns[2]), 53);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );

    heniLinkedListRIterRemoveAndAdvance(&ri);

    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterRemoveBackFromThreeElements_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 61);
    UT_PTR_INI(&(lns[1]), 62);
    UT_PTR_INI(&(lns[2]), 63);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    heniLinkedListRIterInit(&ri, &ll);
    heniLinkedListRIterToFIter(&ri, &fi);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );

    heniLinkedListFIterRemoveAndAdvance(&fi);

    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterRemoveBackFromThreeElements_ShouldContainTwoElements(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 71);
    UT_PTR_INI(&(lns[1]), 72);
    UT_PTR_INI(&(lns[2]), 73);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[2]) );
    heniLinkedListFIterInit(&fi, &ll);
    heniLinkedListFIterToRIter(&fi, &ri);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );

    heniLinkedListRIterRemoveAndAdvance(&ri);

    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeTwoEmpty_ShouldBothBecomeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListMerge(&ll1, &ll2);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeEmptyAndOneElement_ShouldFirstBecomeOneElementAndSecondBecomeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[0])));

    heniLinkedListMerge(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeOneElementAndEmpty_ShouldFirstStayOneElementAndSecondStayEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListInit(&ll2);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListMerge(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeTwoOneElementLists_ShouldFirstBecomeTwoElementsAndSecondBecomeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[1])));

    heniLinkedListMerge(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeEmptyAndTwoElement_ShouldFirstBecomeTwoElementAndSecondBecomeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 31);
    UT_PTR_INI(&(lns[1]), 32);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[1])));

    heniLinkedListMerge(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeTwoElementAndEmpty_ShouldFirstStayTwoElementAndSecondStayEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 41);
    UT_PTR_INI(&(lns[1]), 42);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListInit(&ll2);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListMerge(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);


    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeTwoTwoElementLists_ShouldFirstBecomeFourElementsAndSecondBecomeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 51);
    UT_PTR_INI(&(lns[1]), 52);
    UT_PTR_INI(&(lns[2]), 53);
    UT_PTR_INI(&(lns[3]), 54);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    heniLinkedListMerge(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeWithItself_ShouldDoNothing(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[1];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));

    heniLinkedListMerge(&ll, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[0])));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeTwoEmpty_ShouldIterationFinish(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;

    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterSetAfterMerge(&fi, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterMergeTwoEmpty_ShouldIterationFinish(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListRIterSetAfterMerge(&ri, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeEmptyAndTwoElement_ShouldIterationFinish(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[1])));

    heniLinkedListFIterSetAfterMerge(&fi, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterMergeEmptyAndTwoElement_ShouldIteratorPointToFrontElementOfOldSecondOnNewFirst(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[1])));

    heniLinkedListRIterSetAfterMerge(&ri, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeTwoElementAndEmpty_ShouldIteratorPointToBackElementOfOldFirstOnNewFirst(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;

    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListFIterSetAfterMerge(&fi, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
}



HENI_UT_FUNCT_DEF_PREFIX void
riterMergeTwoElementAndEmpty_ShouldIterationFinish(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 31);
    UT_PTR_INI(&(lns[1]), 32);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    heniLinkedListRIterSetAfterMerge(&ri, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeTwoElementAndTwoElement_ShouldIteratorPointToBackElementOfOldFirstOnNewFirst(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;

    UT_PTR_INI(&(lns[0]), 41);
    UT_PTR_INI(&(lns[1]), 42);
    UT_PTR_INI(&(lns[2]), 43);
    UT_PTR_INI(&(lns[3]), 44);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    heniLinkedListFIterSetAfterMerge(&fi, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterMergeTwoElementAndTwoElement_ShouldIteratorPointToFrontElementOfOldSecondOnNewFirst(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 51);
    UT_PTR_INI(&(lns[1]), 52);
    UT_PTR_INI(&(lns[2]), 53);
    UT_PTR_INI(&(lns[3]), 54);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    heniLinkedListRIterSetAfterMerge(&ri, &ll1, &ll2);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeTwoElementAndTwoElementWithNullFitter_ShouldOnlyMerge(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;

    UT_PTR_INI(&(lns[0]), 61);
    UT_PTR_INI(&(lns[1]), 62);
    UT_PTR_INI(&(lns[2]), 63);
    UT_PTR_INI(&(lns[3]), 64);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    heniLinkedListFIterSetAfterMerge(NULL, &ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterMergeTwoElementAndTwoElementWithNullFitter_ShouldOnlyMerge(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;

    UT_PTR_INI(&(lns[0]), 61);
    UT_PTR_INI(&(lns[1]), 62);
    UT_PTR_INI(&(lns[2]), 63);
    UT_PTR_INI(&(lns[3]), 64);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    heniLinkedListRIterSetAfterMerge(NULL, &ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitBeforeOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    UT_PTR_INI(&(lns[3]), 4);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    heniLinkedListFIterFinish(&fi);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );
    
    heniLinkedListFIterSplitBeforeWithoutMoving(&fi, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitBeforeOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    UT_PTR_INI(&(lns[3]), 14);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    heniLinkedListRIterFinish(&ri);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );
    
    heniLinkedListRIterSplitBeforeWithoutMoving(&ri, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitAfterOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;

    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    UT_PTR_INI(&(lns[2]), 23);
    UT_PTR_INI(&(lns[3]), 24);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    heniLinkedListFIterFinish(&fi);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );
    
    heniLinkedListFIterSplitAfterAndAdvance(&fi, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitAfterOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 31);
    UT_PTR_INI(&(lns[1]), 32);
    UT_PTR_INI(&(lns[2]), 33);
    UT_PTR_INI(&(lns[3]), 34);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    heniLinkedListRIterFinish(&ri);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );
    
    heniLinkedListRIterSplitAfterAndAdvance(&ri, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitBeforeOnFrontElement_ShouldListBecomeEmptyTailListBecomeFullAndIteratorReferToFrontOfTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    UT_PTR_INI(&(lns[3]), 4);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    
    heniLinkedListFIterSplitBeforeWithoutMoving(&fi, &ll2);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitBeforeOnFrontElement_ShouldListBecomeNearlyFullHeadListBecomeOneElementAndIteratorReferToBackOfHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    UT_PTR_INI(&(lns[3]), 14);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    heniLinkedListFIterToRIter(&fi, &ri);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    
    heniLinkedListRIterSplitBeforeWithoutMoving(&ri, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[0])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitAfterOnFrontElement_ShouldListBecomeOneElementTailListBecomeNearlyFullAndIteratorReferToFrontOfTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 21);
    UT_PTR_INI(&(lns[1]), 22);
    UT_PTR_INI(&(lns[2]), 23);
    UT_PTR_INI(&(lns[3]), 24);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    
    heniLinkedListFIterSplitAfterAndAdvance(&fi, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitAfterOnFrontElement_ShouldListStayFullHeadListBecomeEmptyAndIterationFinishOnHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 31);
    UT_PTR_INI(&(lns[1]), 32);
    UT_PTR_INI(&(lns[2]), 33);
    UT_PTR_INI(&(lns[3]), 34);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    heniLinkedListFIterToRIter(&fi, &ri);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    
    heniLinkedListRIterSplitAfterAndAdvance(&ri, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitBeforeOnBackElement_ShouldListBecomeNearlyFullTailListBecomeOneElementAndIteratorReferToFrontOfTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    UT_PTR_INI(&(lns[3]), 4);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    heniLinkedListRIterToFIter(&ri, &fi);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])) );
    
    heniLinkedListFIterSplitBeforeWithoutMoving(&fi, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitBeforeOnBackElement_ShouldListBecomeEmptyHeadListBecomeFullAndIteratorReferToBackOfHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    UT_PTR_INI(&(lns[3]), 14);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])) );
    
    heniLinkedListRIterSplitBeforeWithoutMoving(&ri, &ll2);

    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), NULL);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitAfterOnBackElement_ShouldListRemainFullTailListBecomeEmptyAndIterationFinishOnTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    UT_PTR_INI(&(lns[3]), 4);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    heniLinkedListRIterToFIter(&ri, &fi);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])) );
    
    heniLinkedListFIterSplitAfterAndAdvance(&fi, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), NULL);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), NULL);

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitAfterOnBackElement_ShouldListBecomeOneElementHeadListBecomeNearlyFullAndIteratorReferToBackOfHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    UT_PTR_INI(&(lns[3]), 14);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])) );
    
    heniLinkedListRIterSplitAfterAndAdvance(&ri, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[2])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitBeforeOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToFrontOfTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    UT_PTR_INI(&(lns[3]), 4);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    
    heniLinkedListFIterSplitBeforeWithoutMoving(&fi, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitBeforeOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToBackOfHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    UT_PTR_INI(&(lns[3]), 14);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    
    heniLinkedListRIterSplitBeforeWithoutMoving(&ri, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitAfterOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToFrontOfTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    UT_PTR_INI(&(lns[3]), 4);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    
    heniLinkedListFIterSplitAfterAndAdvance(&fi, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(! heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), NULL );

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
riterSplitAfterOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToBackOfHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    UT_PTR_INI(&(lns[3]), 14);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])) );
    
    heniLinkedListRIterSplitAfterAndAdvance(&ri, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[1])));

    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(! heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), NULL );

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
copyTwoLists_ShouldBeEqual(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    heniLinkedListInit(&ll1);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[2])));

    heniLinkedListCopyShallow(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[2])));

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
copySameList_ShouldNothingHappen(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[3];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    UT_PTR_INI(&(lns[2]), 13);
    heniLinkedListInit(&ll);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[2])));

    heniLinkedListCopyShallow(&ll, &ll);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll), UT_PTR_UT2LN(&(lns[2])));

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
swapTwoLists_ShouldBeSwapped(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[4];
    heni_linked_list_t         ll1;
    heni_linked_list_t         ll2;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    UT_PTR_INI(&(lns[0]), 1);
    UT_PTR_INI(&(lns[1]), 2);
    UT_PTR_INI(&(lns[2]), 3);
    UT_PTR_INI(&(lns[3]), 4);
    heniLinkedListInit(&ll1);
    heniLinkedListInit(&ll2);
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll1, UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListNodeAddBack(&ll2, UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[3])));

    heniLinkedListSwap(&ll1, &ll2);

    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll1));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll1), UT_PTR_UT2LN(&(lns[2])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll1), UT_PTR_UT2LN(&(lns[3])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll2));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetFront(&ll2), UT_PTR_UT2LN(&(lns[0])));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListNodeGetBack(&ll2), UT_PTR_UT2LN(&(lns[1])));

    heniLinkedListFIterInit(&fi, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll1);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListFIterInit(&fi, &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll2);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetNode(&fi), NULL);

    heniLinkedListRIterInit(&ri, &ll1);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll1);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[3])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[2])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);

    heniLinkedListRIterInit(&ri, &ll2);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll2);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetNode(&ri), NULL);
}



HENI_UT_FUNCT_DEF_PREFIX void
swapSameList_ShouldNothingHappen(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    ut_list_node_t             lns[2];
    heni_linked_list_t         ll;
    heni_linked_list_fiter_t   fi;
    heni_linked_list_riter_t   ri;

    heniLinkedListInit(&ll);
    UT_PTR_INI(&(lns[0]), 11);
    UT_PTR_INI(&(lns[1]), 12);
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[0])));
    heniLinkedListNodeAddBack(&ll, UT_PTR_UT2LN(&(lns[1])));
    HENI_UT_CHECK(! heniLinkedListIsEmpty(&ll));
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetFront(&ll) ), &(lns[0]) );
    HENI_UT_CHECK_PTR_EQ( UT_PTR_LN2UT( heniLinkedListNodeGetBack(&ll) ), &(lns[1]) );

    heniLinkedListSwap(&ll, &ll);

    heniLinkedListFIterInit(&fi, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListFIterGetList(&fi), &ll);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(heniLinkedListFIterIsActive(&fi));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListFIterGetNode(&fi), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListFIterAdvance(&fi);
    HENI_UT_CHECK(!heniLinkedListFIterIsActive(&fi));

    heniLinkedListRIterInit(&ri, &ll);
    HENI_UT_CHECK_PTR_EQ(heniLinkedListRIterGetList(&ri), &ll);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[1])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(heniLinkedListRIterIsActive(&ri));
    HENI_UT_CHECK_PTR_EQ( heniLinkedListRIterGetNode(&ri), UT_PTR_UT2LN(&(lns[0])) );
    heniLinkedListRIterAdvance(&ri);
    HENI_UT_CHECK(!heniLinkedListRIterIsActive(&ri));
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          The main test method                          *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_EXT_FUNCT_DEF_PREFIX void heniUnitTestSynchronouslyRunAll(
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    HENI_UT_RUN_TEST(initList_ShouldBeEmpty);
    HENI_UT_RUN_TEST(addOneFront_ShouldContainOneElement);
    HENI_UT_RUN_TEST(addOneBack_ShouldContainOneElement);
    HENI_UT_RUN_TEST(addTwoFront_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(addTwoBack_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(addOneFrontOneBack_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(addOneBackOneFront_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(fiterAddBeforeToEmpty_ShouldContainOneElement);
    HENI_UT_RUN_TEST(riterAddBeforeToEmpty_ShouldContainOneElement);
    HENI_UT_RUN_TEST(fiterAddAfterToEmpty_ShouldContainOneElement);
    HENI_UT_RUN_TEST(riterAddAfterToEmpty_ShouldContainOneElement);
    HENI_UT_RUN_TEST(fiterAddBeforeNothingToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(riterAddBeforeNothingToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(fiterAddBeforeElementToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(riterAddBeforeElementToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(fiterAddAfterNothingToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(riterAddAfterNothingToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(fiterAddAfterElementToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(riterAddAfterElementToOneElement_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(fiterAddBetweenTwoElementsWithBefore_ShouldContainThreeElements);
    HENI_UT_RUN_TEST(riterAddBetweenTwoElementsWithBefore_ShouldContainThreeElements);
    HENI_UT_RUN_TEST(fiterAddBetweenTwoElementsWithAfter_ShouldContainThreeElements);
    HENI_UT_RUN_TEST(riterAddBetweenTwoElementsWithAfter_ShouldContainThreeElements);
    HENI_UT_RUN_TEST(fiterFinishIterationEarly_ShouldIterationFinish);
    HENI_UT_RUN_TEST(riterFinishIterationEarly_ShouldIterationFinish);
    HENI_UT_RUN_TEST(fiterCopyInactive_ShouldCopyBeInactive);
    HENI_UT_RUN_TEST(riterCopyInactive_ShouldCopyBeInactive);
    HENI_UT_RUN_TEST(fiterCopyActive_ShouldCopyBeEqual);
    HENI_UT_RUN_TEST(riterCopyActive_ShouldCopyBeEqual);
    HENI_UT_RUN_TEST(fiterInactiveToRIter_ShouldRIterBeInactive);
    HENI_UT_RUN_TEST(riterInactiveToFIter_ShouldFIterBeInactive);
    HENI_UT_RUN_TEST(fiterActiveToRIter_ShouldRIterBeActiveAndPointSameNodeAndList);
    HENI_UT_RUN_TEST(riterActiveToFIter_ShouldFIterBeInactiveAndPointSameNodeAndList);
    HENI_UT_RUN_TEST(tryRemoveFromFrontAndBackOfEmptyList_ShouldDoNothing);
    HENI_UT_RUN_TEST(removeFrontOfOne_ShouldContainNoElements);
    HENI_UT_RUN_TEST(removeBackOfOne_ShouldContainNoElements);
    HENI_UT_RUN_TEST(removeFrontOfTwo_ShouldContainNoElements);
    HENI_UT_RUN_TEST(removeBackOfTwo_ShouldContainNoElements);
    HENI_UT_RUN_TEST(removeFrontBackOfTwo_ShouldContainNoElements);
    HENI_UT_RUN_TEST(removeBackFrontOfTwo_ShouldContainNoElements);
    HENI_UT_RUN_TEST(fiterRemoveFromOneElement_ShouldContainNoElements);
    HENI_UT_RUN_TEST(riterRemoveFromOneElement_ShouldContainNoElements);
    HENI_UT_RUN_TEST(fiterRemoveFrontFromThreeElements_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(riterRemoveFrontFromThreeElements_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(fiterRemoveMiddleFromThreeElements_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(riterRemoveMiddleFromThreeElements_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(fiterRemoveBackFromThreeElements_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(riterRemoveBackFromThreeElements_ShouldContainTwoElements);
    HENI_UT_RUN_TEST(mergeTwoEmpty_ShouldBothBecomeEmpty);
    HENI_UT_RUN_TEST(mergeEmptyAndOneElement_ShouldFirstBecomeOneElementAndSecondBecomeEmpty);
    HENI_UT_RUN_TEST(mergeOneElementAndEmpty_ShouldFirstStayOneElementAndSecondStayEmpty);
    HENI_UT_RUN_TEST(mergeTwoOneElementLists_ShouldFirstBecomeTwoElementsAndSecondBecomeEmpty);
    HENI_UT_RUN_TEST(mergeEmptyAndTwoElement_ShouldFirstBecomeTwoElementAndSecondBecomeEmpty);
    HENI_UT_RUN_TEST(mergeTwoElementAndEmpty_ShouldFirstStayTwoElementAndSecondStayEmpty);
    HENI_UT_RUN_TEST(mergeTwoTwoElementLists_ShouldFirstBecomeFourElementsAndSecondBecomeEmpty);
    HENI_UT_RUN_TEST(mergeWithItself_ShouldDoNothing);
    HENI_UT_RUN_TEST(fiterMergeTwoEmpty_ShouldIterationFinish);
    HENI_UT_RUN_TEST(riterMergeTwoEmpty_ShouldIterationFinish);
    HENI_UT_RUN_TEST(fiterMergeEmptyAndTwoElement_ShouldIterationFinish);
    HENI_UT_RUN_TEST(riterMergeEmptyAndTwoElement_ShouldIteratorPointToFrontElementOfOldSecondOnNewFirst);
    HENI_UT_RUN_TEST(fiterMergeTwoElementAndEmpty_ShouldIteratorPointToBackElementOfOldFirstOnNewFirst);
    HENI_UT_RUN_TEST(riterMergeTwoElementAndEmpty_ShouldIterationFinish);
    HENI_UT_RUN_TEST(fiterMergeTwoElementAndTwoElement_ShouldIteratorPointToBackElementOfOldFirstOnNewFirst);
    HENI_UT_RUN_TEST(riterMergeTwoElementAndTwoElement_ShouldIteratorPointToFrontElementOfOldSecondOnNewFirst);
    HENI_UT_RUN_TEST(fiterMergeTwoElementAndTwoElementWithNullFitter_ShouldOnlyMerge);
    HENI_UT_RUN_TEST(riterMergeTwoElementAndTwoElementWithNullFitter_ShouldOnlyMerge);
    HENI_UT_RUN_TEST(fiterSplitBeforeOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList);
    HENI_UT_RUN_TEST(riterSplitBeforeOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToHeadList);
    HENI_UT_RUN_TEST(fiterSplitAfterOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList);
    HENI_UT_RUN_TEST(riterSplitAfterOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList);
    HENI_UT_RUN_TEST(fiterSplitBeforeOnFrontElement_ShouldListBecomeEmptyTailListBecomeFullAndIteratorReferToFrontOfTailList);
    HENI_UT_RUN_TEST(riterSplitBeforeOnFrontElement_ShouldListBecomeNearlyFullHeadListBecomeOneElementAndIteratorReferToBackOfHeadList);
    HENI_UT_RUN_TEST(fiterSplitAfterOnFrontElement_ShouldListBecomeOneElementTailListBecomeNearlyFullAndIteratorReferToFrontOfTailList);
    HENI_UT_RUN_TEST(riterSplitAfterOnFrontElement_ShouldListStayFullHeadListBecomeEmptyAndIterationFinishOnHeadList);
    HENI_UT_RUN_TEST(fiterSplitBeforeOnBackElement_ShouldListBecomeNearlyFullTailListBecomeOneElementAndIteratorReferToFrontOfTailList);
    HENI_UT_RUN_TEST(riterSplitBeforeOnBackElement_ShouldListBecomeEmptyHeadListBecomeFullAndIteratorReferToBackOfHeadList);
    HENI_UT_RUN_TEST(fiterSplitAfterOnBackElement_ShouldListRemainFullTailListBecomeEmptyAndIterationFinishOnTailList);
    HENI_UT_RUN_TEST(riterSplitAfterOnBackElement_ShouldListBecomeOneElementHeadListBecomeNearlyFullAndIteratorReferToBackOfHeadList);
    HENI_UT_RUN_TEST(fiterSplitBeforeOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToFrontOfTailList);
    HENI_UT_RUN_TEST(riterSplitBeforeOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToBackOfHeadList);
    HENI_UT_RUN_TEST(fiterSplitAfterOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToFrontOfTailList);
    HENI_UT_RUN_TEST(riterSplitAfterOnMiddleElement_ShouldEachListGetHalfElementsAndIteratorReferToBackOfHeadList);
    HENI_UT_RUN_TEST(copyTwoLists_ShouldBeEqual);
    HENI_UT_RUN_TEST(copySameList_ShouldNothingHappen);
    HENI_UT_RUN_TEST(swapTwoLists_ShouldBeSwapped);
    HENI_UT_RUN_TEST(swapSameList_ShouldNothingHappen);
}

