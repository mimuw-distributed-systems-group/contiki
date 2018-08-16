/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIVectoredIO.h"
#include "HENIUnitTest.h"

/**
 * @file
 * Type: heniUnitTestSynchronouslyRunAll
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

enum
{
    UT_DEF_IOBUF_NODE_LEN = 16,
};

typedef struct ut_iobuf_node_s
{
    uint8_t                  mem[UT_DEF_IOBUF_NODE_LEN];
    heni_iobuf_list_node_t   rnode;
} ut_iobuf_node_t;

#define UT_PTR_UT2LN(utn) (&((utn)->rnode))
#define UT_PTR_LN2UT(ln) ((ut_iobuf_node_t *)((uint8_t *)(ln) - offsetof(ut_iobuf_node_t, rnode)))
#define UT_PTR_INI(utn, len) do { HENI_PASSERT((size_t)(len) <= (size_t)UT_DEF_IOBUF_NODE_LEN); heniIOBufNodeInitMem(UT_PTR_UT2LN(utn), &((utn)->mem[0]), (len)); } while (0)
#define UT_PTR_OVERRIDE(utn, lenp, ptr) do { HENI_PASSERT((size_t)(lenp) <= (size_t)UT_DEF_IOBUF_NODE_LEN); (UT_PTR_UT2LN(utn))->iobuf.base = (ptr); (UT_PTR_UT2LN(utn))->iobuf.len = (lenp); } while (0)
#define UT_IOBUF_LIST_INI(iol, utns, lens, num, seed) doCreateIOBufListWithPattern(iol, utns, lens, num, seed, __LINE__, __FUNCTION__)
#define UT_MEM_INI(mptr, mlen, seed) doFillMemWithPattern(mptr, mlen, seed)

#define HENI_UT_CHECK_IOBUF_NODE_EQ(what, exp) do { \
    heni_iobuf_list_node_t * iolnp = (what); \
    HENI_UT_CHECK_PTR_EQ(iolnp, doNodeMangleToDisableWarning(exp)); \
    if (doNodeMangleToDisableWarning(exp) != NULL && iolnp != NULL) { \
        HENI_UT_CHECK_PTR_EQ(heniIOBufNodeGetBuf(iolnp), heniIOBufNodeGetBuf(exp)); \
        HENI_UT_CHECK_PTR_EQ(heniIOBufNodeGetMemPtr(iolnp), heniIOBufNodeGetMemPtr(exp)); \
        HENI_UT_CHECK_SZ_EQ(heniIOBufNodeGetMemLen(iolnp), heniIOBufNodeGetMemLen(exp)); \
    } \
} while (0)    

#define HENI_UT_CHECK_IOBUF_MEMPTR_EQ(iter, utn, off) do { \
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFiterGetMemPtr(iter), (void *)((uint8_t *)(heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(utn))) + (off))); \
} while (0)

#define HENI_UT_CHECK_IOBUF_MEMLEN_EQ(iter, utn, off) do { \
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFiterGetMemLen(iter), (heniIOBufNodeGetMemLen(UT_PTR_UT2LN(utn))) - (off)); \
} while (0)

#define HENI_UT_CHECK_IOBUF_MEMPTR_NULL(iter) HENI_UT_CHECK_PTR_EQ(heniIOBufListFiterGetMemPtr(iter), NULL)

#define HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(iter) HENI_UT_CHECK_SZ_EQ(heniIOBufListFiterGetMemLen(iter), 0)

#define HENI_UT_CHECK_IOBUF_CONTENTS(iter, off, len, seed) doCheckIOBufListPattern(iter, len, off, seed, __LINE__, __FUNCTION__)

#define HENI_UT_CHECK_MEMBUF_CONTENTS(mptr, moff, mlen, seed) doCheckMemWithPattern(mptr, mlen, moff, seed, __LINE__, __FUNCTION__)

/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Helper functions                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_PRV_FUNCT_DEF_PREFIX void doFillMemWithPattern(
        uint8_t * memPtr,
        size_t memLen,
        size_t seed
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    size_t   i;
    for (i = 0; i < memLen; ++i)
    {
        memPtr[i] = (uint8_t)(seed + i);
    }
}



HENI_PRV_FUNCT_DEF_PREFIX void doCheckMemWithPattern(
        uint8_t const * memPtr,
        size_t memLen,
        size_t memOff,
        size_t seed,
        unsigned line,
        char const * funct
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    size_t   i;
    for (i = 0; i < memLen; ++i)
    {
        if (memPtr[i] != (uint8_t)(seed + i + memOff))
        {
            HENI_UT_LOG("ERROR: At position %u of a memory buffer there is "
                    "element %u whereas element %u was expected in function %s "
                    "(file %s, line %u)!\n", (unsigned)i, (unsigned)memPtr[i],
                    (unsigned)(seed + i + memOff), funct, __FILE__, line);
            heniUnitTestFail();
            return;
        }
    }
}



HENI_PRV_FUNCT_DEF_PREFIX void doCreateIOBufListWithPattern(
        heni_iobuf_list_t * iol,
        ut_iobuf_node_t * ions,
        size_t const * lengths,
        size_t num,
        size_t seed,
        unsigned line,
        char const * funct
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    heni_iobuf_list_fiter_t   iofi;
    size_t                    i, s = 0;

    heniIOBufListInit(iol);
    for (i = 0; i < num; ++i)
    {
        UT_PTR_INI(&(ions[i]), lengths[i]);
        s += lengths[i];
        heniIOBufNodeAddBack(iol, UT_PTR_UT2LN(&(ions[i])));
    }
    heniIOBufListFIterInit(&iofi, iol);
    for (i = 0; i < s; ++i)
    {
        HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTrySetByteAndAdvance(&iofi, (uint8_t)(i + seed)), 1);
    }
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
}



HENI_PRV_FUNCT_DEF_PREFIX void doCheckIOBufListPattern(
        heni_iobuf_list_fiter_t * iofi,
        size_t len,
        size_t offset,
        size_t seed,
        unsigned line,
        char const * funct
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    for (; offset < len; ++offset)
    {
        int_fast16_t   tmp = heniIOBufListFIterTryGetByteAndAdvance(iofi);
        if (tmp < 0 || tmp > 0xff)
        {
            HENI_UT_LOG("ERROR: An I/O buffer list has only %u elements "
                    "whereas %u elements were expected in function %s "
                    "(file %s, line %u)!\n", (unsigned)offset,
                    (unsigned)len, funct, __FILE__, line);
            heniUnitTestFail();
            return;
        }
        if ((uint8_t)tmp != (uint8_t)(offset + seed))
        {
            HENI_UT_LOG("ERROR: At position %u, an I/O buffer list has "
                    "element %u whereas element %u was expected in function %s "
                    "(file %s, line %u)!\n", (unsigned)offset, (unsigned)tmp,
                    (unsigned)(offset + seed), funct, __FILE__, line);
            heniUnitTestFail();
            return;
        }
    }
}



HENI_PRV_FUNCT_DEF_PREFIX heni_iobuf_list_node_t * doNodeMangleToDisableWarning(
        heni_iobuf_list_node_t * utn
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    return utn;
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Individual tests                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_UT_FUNCT_DEF_PREFIX void
initList_ShouldBeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    heniIOBufListInit(&iol);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneFront_ShouldContainOneBuf(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 13,
    };
    ut_iobuf_node_t           ions[1];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufNodeAddFront(&iol, UT_PTR_UT2LN(&(ions[0])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 6), 6);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 1+6);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 1+6);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1-(1+6)), UT_BUF_LEN_1-(1+6));

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneBack_ShouldContainOneBuf(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 8,
    };
    ut_iobuf_node_t           ions[1];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[0])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 4), 4);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 4);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 4);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 2), 2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 4+2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 4+2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1-(4+2)+1), UT_BUF_LEN_1-(4+2));

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
addTwoFront_ShouldContainTwoBufs(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 13,
        UT_BUF_LEN_2 = 3,
    };
    ut_iobuf_node_t           ions[2];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    UT_PTR_INI(&(ions[1]), UT_BUF_LEN_2);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufNodeAddFront(&iol, UT_PTR_UT2LN(&(ions[1])));
    heniIOBufNodeAddFront(&iol, UT_PTR_UT2LN(&(ions[0])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1+UT_BUF_LEN_2);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1-1), UT_BUF_LEN_1-1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1-1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1-1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2-1), UT_BUF_LEN_2-1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2-1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2-1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
addTwoBack_ShouldContainTwoBufs(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 8,
        UT_BUF_LEN_2 = 4,
    };
    ut_iobuf_node_t           ions[2];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    UT_PTR_INI(&(ions[1]), UT_BUF_LEN_2);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[0])));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[1])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1+UT_BUF_LEN_2);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1-1), UT_BUF_LEN_1-1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1-1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1-1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2-1), UT_BUF_LEN_2-1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2-2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2-2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2-1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2-1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 3), 1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneFrontOneBack_ShouldContainTwoBufs(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 13,
        UT_BUF_LEN_2 = 3,
    };
    ut_iobuf_node_t           ions[2];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    UT_PTR_INI(&(ions[1]), UT_BUF_LEN_2);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufNodeAddFront(&iol, UT_PTR_UT2LN(&(ions[0])));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[1])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1+UT_BUF_LEN_2);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1-1), UT_BUF_LEN_1-1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1-1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1-1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 2*UT_BUF_LEN_2), UT_BUF_LEN_2+1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
addOneBackOneFront_ShouldContainTwoBufs(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 13,
        UT_BUF_LEN_2 = 3,
    };
    ut_iobuf_node_t           ions[2];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    UT_PTR_INI(&(ions[1]), UT_BUF_LEN_2);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[1])));
    heniIOBufNodeAddFront(&iol, UT_PTR_UT2LN(&(ions[0])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1+UT_BUF_LEN_2);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1+1), UT_BUF_LEN_1+1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2-1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 2), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterAlignedIteration_ShouldCorrectlyManipulateOffsets(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 1,
        UT_BUF_LEN_3 = 5,
    };
    ut_iobuf_node_t           ions[3];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    UT_PTR_INI(&(ions[1]), UT_BUF_LEN_2);
    UT_PTR_INI(&(ions[2]), UT_BUF_LEN_3);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[0])));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[1])));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1+UT_BUF_LEN_2+UT_BUF_LEN_3);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterUnalignedIteration_ShouldCorrectlyManipulateOffsets(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 7,
        UT_BUF_LEN_3 = 5,
    };
    ut_iobuf_node_t           ions[3];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    UT_PTR_INI(&(ions[1]), UT_BUF_LEN_2);
    UT_PTR_INI(&(ions[2]), UT_BUF_LEN_3);
    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[0])));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[1])));
    heniIOBufNodeAddBack(&iol, UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1+UT_BUF_LEN_2+UT_BUF_LEN_3);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1 / 2), UT_BUF_LEN_1 / 2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 / 2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 / 2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1 - UT_BUF_LEN_1 / 2 + 1), UT_BUF_LEN_1 - UT_BUF_LEN_1 / 2 + 1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, (UT_BUF_LEN_2 - 1) / 2), (UT_BUF_LEN_2 - 1) / 2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), (UT_BUF_LEN_2 - 1) / 2 + 1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), (UT_BUF_LEN_2 - 1) / 2 + 1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2 - (UT_BUF_LEN_2 - 1) / 2), UT_BUF_LEN_2 - (UT_BUF_LEN_2 - 1) / 2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, (UT_BUF_LEN_3 - 1) / 2), (UT_BUF_LEN_3 - 1) / 2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), (UT_BUF_LEN_3 - 1) / 2 + 1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), (UT_BUF_LEN_3 - 1) / 2 + 1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3 - (UT_BUF_LEN_3 - 1) / 2 - 1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 1), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterNonprogressIteration_ShouldNotChangeOffset(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 11,
    };
    ut_iobuf_node_t           ions[1];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    heniIOBufListInit(&iol);
    heniIOBufNodeAddFront(&iol, UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1);

    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 0), 0);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1 / 2), UT_BUF_LEN_1 / 2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 / 2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 / 2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 0), 0);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 / 2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 / 2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1 - (UT_BUF_LEN_1 / 2)), UT_BUF_LEN_1 - (UT_BUF_LEN_1 / 2));

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, 0), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSetAndGetOnEmptyList_ShouldReturnError(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    heniIOBufListInit(&iol);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTrySetByteAndAdvance(&iofi, 0x17), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTrySetByteAndAdvance(&iofi, 0x19), 0);

    HENI_UT_CHECK(heniIOBufListFIterTryGetByteAndAdvance(&iofi) < 0);

    HENI_UT_CHECK(heniIOBufListFIterTryGetByteAndAdvance(&iofi) < 0);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSetAndGetOnFinishedIteration_ShouldReturnError(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_LEN_1 = 7,
    };
    ut_iobuf_node_t           ions[1];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_PTR_INI(&(ions[0]), UT_BUF_LEN_1);
    heniIOBufListInit(&iol);
    heniIOBufNodeAddFront(&iol, UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_1);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    heniIOBufListFIterFinish(&iofi);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTrySetByteAndAdvance(&iofi, 0x45), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTrySetByteAndAdvance(&iofi, 0x77), 0);

    HENI_UT_CHECK(heniIOBufListFIterTryGetByteAndAdvance(&iofi) < 0);

    HENI_UT_CHECK(heniIOBufListFIterTryGetByteAndAdvance(&iofi) < 0);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSetAndGetOnOneBufList_ShouldValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x99,
        UT_BUF_LEN_1 = 15,
    };
    ut_iobuf_node_t           ions[1];
    size_t                    iols[1] = {UT_BUF_LEN_1};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 1, UT_BUF_SEED);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSetAndGetOnTwoBufList_ShouldValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x33,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 9,
    };
    ut_iobuf_node_t           ions[2];
    size_t                    iols[2] = {UT_BUF_LEN_1, UT_BUF_LEN_2};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 2, UT_BUF_SEED);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSetAndGetOnThreeBufList_ShouldValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 1,
        UT_BUF_LEN_3 = UT_DEF_IOBUF_NODE_LEN,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSetAndGetWithSingleOverwrite_ShouldValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x42,
        UT_BUF_LEN_1 = 4,
        UT_BUF_LEN_2 = 3,
        UT_BUF_LEN_3 = 7,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2), UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTrySetByteAndAdvance(&iofi, UT_BUF_SEED + UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3), 1);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2, UT_BUF_SEED);
    HENI_UT_CHECK(heniIOBufListFIterTryGetByteAndAdvance(&iofi) == UT_BUF_SEED + UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2 + 1, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSetAndGetWithCompleteOverwrite_ShouldValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x98,
        UT_BUF_SEED2 = 0x13,
        UT_BUF_LEN_1 = 9,
        UT_BUF_LEN_2 = 2,
        UT_BUF_LEN_3 = 11,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;
    size_t                    i;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED1);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED1);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    for (i = 0; i < UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3; ++i)
    {
        HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTrySetByteAndAdvance(&iofi, (uint8_t)(i + UT_BUF_SEED2)), 1);
    }
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED2);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyToEmptyBuffer_ShouldNothingChange(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_CLN_SEED = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, NULL, 0), 0);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyToNonEmptyBufferFromEmptyList_ShouldNothingChange(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_CLN_SEED = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    heniIOBufListFIterFinish(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), 0);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_TOT, UT_CLN_SEED);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyToMatchingBuffer_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_CLN_SEED = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromOffsetZeroToShorterBuffer_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x44,
        UT_CLN_SEED = 0x80 + 0x12,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_SHR = UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_SHR];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_SHR, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_SHR), UT_BUF_LEN_SHR);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_SHR, UT_BUF_SEED);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2 / 2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2 / 2);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromCloseToEndToShorterBuffer_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x22,
        UT_CLN_SEED = 0x80 + 0x39,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2,
        UT_BUF_LEN_SHR = UT_BUF_LEN_TOT - UT_BUF_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_SHR];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_SHR, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_SHR), UT_BUF_LEN_SHR);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, UT_BUF_OFF, UT_BUF_LEN_SHR, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromCloseToEndToOverlengthBuffer_ShouldValuesInBufferMatchIfInBuffer(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x07,
        UT_CLN_SEED = 0x80 + 0x42,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2,
        UT_BUF_LEN_SHR = UT_BUF_LEN_TOT - UT_BUF_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_SHR);

    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[0]), UT_BUF_OFF, UT_BUF_LEN_SHR, UT_BUF_SEED);
    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[UT_BUF_LEN_SHR]), UT_BUF_LEN_SHR, UT_BUF_LEN_TOT - UT_BUF_LEN_SHR, UT_CLN_SEED);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromFrontNodeToMatchingBuffer_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x033,
        UT_CLN_SEED = 0x80 + 0x11,
        UT_BUF_LEN_1 = 13,
        UT_BUF_LEN_2 = 11,
        UT_BUF_LEN_3 = 15,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1,
        UT_BUF_OFF = 0,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[0]), UT_BUF_OFF, UT_BUF_LEN_TOT, UT_BUF_SEED);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromBackNodeToMatchingBuffer_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x033,
        UT_CLN_SEED = 0x80 + 0x11,
        UT_BUF_LEN_1 = 13,
        UT_BUF_LEN_2 = 11,
        UT_BUF_LEN_3 = 15,
        UT_BUF_LEN_TOT = UT_BUF_LEN_3,
        UT_BUF_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[0]), UT_BUF_OFF, UT_BUF_LEN_TOT, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromMiddleNodeToMatchingBuffer_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x033,
        UT_CLN_SEED = 0x80 + 0x11,
        UT_BUF_LEN_1 = 13,
        UT_BUF_LEN_2 = 11,
        UT_BUF_LEN_3 = 15,
        UT_BUF_LEN_TOT = UT_BUF_LEN_2,
        UT_BUF_OFF = UT_BUF_LEN_1,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyIntoOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[0]), UT_BUF_OFF, UT_BUF_LEN_TOT, UT_BUF_SEED);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromEmptyBuffer_ShouldNothingChange(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_CLN_SEED = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, NULL, 0), 0);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromNonEmptyBufferToEmptyList_ShouldNothingChange(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_CLN_SEED = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_CLN_SEED);
    heniIOBufListFIterInit(&iofi, &iol);
    heniIOBufListFIterFinish(&iofi);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), 0);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_TOT, UT_CLN_SEED);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromMatchingBuffer_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_CLN_SEED = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_CLN_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_BUF_SEED);
    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_TOT, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromShorterBufferToOffsetZero_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x44,
        UT_CLN_SEED = 0x80 + 0x12,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 5,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_SHR = UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_SHR];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_CLN_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_SHR, UT_BUF_SEED);
    heniIOBufListFIterInit(&iofi, &iol);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_SHR), UT_BUF_LEN_SHR);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2 / 2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2 / 2);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_SHR, UT_BUF_SEED);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_LEN_SHR, UT_BUF_LEN_TOT, UT_CLN_SEED);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_SHR, UT_BUF_SEED);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromShorterBufferToCloseToEnd_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x22,
        UT_CLN_SEED = 0x80 + 0x39,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2,
        UT_BUF_LEN_SHR = UT_BUF_LEN_TOT - UT_BUF_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_SHR];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_CLN_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_SHR, UT_BUF_SEED + UT_BUF_OFF);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_SHR), UT_BUF_LEN_SHR);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_OFF, UT_CLN_SEED);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_OFF, UT_BUF_LEN_TOT, UT_BUF_SEED);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_SHR, UT_BUF_SEED + UT_BUF_OFF);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromOverlengthBufferToCloseToEnd_ShouldValuesInBufferMatchIfInBuffer(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x07,
        UT_CLN_SEED = 0x80 + 0x42,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2 / 2,
        UT_BUF_LEN_SHR = UT_BUF_LEN_TOT - UT_BUF_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_CLN_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_OFF - UT_BUF_LEN_1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_SHR);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_OFF, UT_CLN_SEED);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_OFF, UT_BUF_LEN_TOT, UT_BUF_SEED);

    HENI_UT_CHECK_MEMBUF_CONTENTS(membuf, 0, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromMatchingBufferToFrontNode_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x033,
        UT_CLN_SEED = 0x80 + 0x11,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1,
        UT_BUF_OFF = 0,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_CLN_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1, UT_BUF_SEED);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_LEN_1, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_CLN_SEED);

    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[0]), 0, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromMatchingBufferToBackNode_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x033,
        UT_CLN_SEED = 0x80 + 0x11,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_3,
        UT_BUF_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_CLN_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1 + UT_BUF_LEN_2, UT_CLN_SEED);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_LEN_1 + UT_BUF_LEN_2, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_BUF_SEED);

    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[0]), 0, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromMatchingBufferToMiddleNode_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x033,
        UT_CLN_SEED = 0x80 + 0x11,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_2,
        UT_BUF_OFF = UT_BUF_LEN_1,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    uint8_t                   membuf[UT_BUF_LEN_TOT];
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_CLN_SEED);
    UT_MEM_INI(membuf, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_OFF), UT_BUF_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyFromOneBufAndAdvance(&iofi, membuf, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, 0, UT_BUF_LEN_1, UT_CLN_SEED);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_LEN_1, UT_BUF_LEN_1 + UT_BUF_LEN_2, UT_BUF_SEED);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi, UT_BUF_LEN_1 + UT_BUF_LEN_2, UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3, UT_CLN_SEED);

    HENI_UT_CHECK_MEMBUF_CONTENTS(&(membuf[0]), 0, UT_BUF_LEN_TOT, UT_BUF_SEED + UT_BUF_OFF);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromFinishedToAnother_ShouldNothingChange(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x11,
        UT_BUF_SEED2 = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[3];
    size_t                    iols1[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    size_t                    iols2[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 3, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    heniIOBufListFIterFinish(&iofi1);
    heniIOBufListFIterInit(&iofi2, &iol2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_LEN_TOT), 0);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi1));
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi2));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi2), UT_PTR_UT2LN(&(ions2[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi2, &(ions2[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi2, &(ions2[0]), 0);
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_TOT, UT_BUF_SEED2);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyToAnotherFinished_ShouldNothingChange(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x11,
        UT_BUF_SEED2 = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[3];
    size_t                    iols1[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    size_t                    iols2[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 3, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    heniIOBufListFIterInit(&iofi2, &iol2);
    heniIOBufListFIterFinish(&iofi2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_LEN_TOT), 0);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi1));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi1), UT_PTR_UT2LN(&(ions1[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi1, &(ions1[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi1, &(ions1[0]), 0);
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi2));
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_TOT, UT_BUF_SEED2);

}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyAllToAnotherAligned_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x11,
        UT_BUF_SEED2 = 0x80,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[3];
    size_t                    iols1[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    size_t                    iols2[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 3, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    heniIOBufListFIterInit(&iofi2, &iol2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi1));
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi2));
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyAllToAnotherUnaligned_ShouldValuesInBufferMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x55,
        UT_BUF_SEED2 = 0x80 + 0x33,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 6,
        UT_BUF_LEN_13 = 8,
        UT_BUF_LEN_21 = 1,
        UT_BUF_LEN_22 = 3,
        UT_BUF_LEN_23 = 5,
        UT_BUF_LEN_24 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_11 + UT_BUF_LEN_12 + UT_BUF_LEN_13,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[4];
    size_t                    iols1[3] = {UT_BUF_LEN_11, UT_BUF_LEN_12, UT_BUF_LEN_13};
    size_t                    iols2[4] = {UT_BUF_LEN_21, UT_BUF_LEN_22, UT_BUF_LEN_23, UT_BUF_LEN_24};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;

    HENI_UT_CHECK_SZ_EQ((size_t)UT_BUF_LEN_TOT, (size_t)(UT_BUF_LEN_21 + UT_BUF_LEN_22 + UT_BUF_LEN_23 + UT_BUF_LEN_24));

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 4, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    heniIOBufListFIterInit(&iofi2, &iol2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi1));
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi2));
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyLongerToShorter_ShouldAllValuesInShorterMatchLonger(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x44,
        UT_BUF_SEED2 = 0x80 + 0x11,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 6,
        UT_BUF_LEN_13 = 8,
        UT_BUF_LEN_21 = 15,
        UT_BUF_LEN_22 = 3,
        UT_BUF_LEN_23 = 5,
        UT_BUF_LEN_24 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_11 + UT_BUF_LEN_12 + UT_BUF_LEN_13,
        UT_BUF_TO_COPY = UT_BUF_LEN_21 + UT_BUF_LEN_22 + UT_BUF_LEN_23 + UT_BUF_LEN_24,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[4];
    size_t                    iols1[3] = {UT_BUF_LEN_11, UT_BUF_LEN_12, UT_BUF_LEN_13};
    size_t                    iols2[4] = {UT_BUF_LEN_21, UT_BUF_LEN_22, UT_BUF_LEN_23, UT_BUF_LEN_24};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;
    heni_iobuf_list_fiter_t   iofi3;

    HENI_UT_CHECK((size_t)UT_BUF_LEN_TOT < (size_t)UT_BUF_TO_COPY);

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 4, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    heniIOBufListFIterInit(&iofi2, &iol2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi2, &iofi1, UT_BUF_TO_COPY), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi1));
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi2));
    heniIOBufListFIterInit(&iofi3, &iol2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi3, UT_BUF_LEN_TOT), UT_BUF_LEN_TOT);
    HENI_UT_CHECK(heniIOBufListFIterEquals(&iofi3, &iofi2));
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_TOT, UT_BUF_SEED2);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyShorterToLonger_ShouldAllAppropriateValuesInLongerMatchShorter(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x22,
        UT_BUF_SEED2 = 0x80 + 0x33,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 6,
        UT_BUF_LEN_13 = 8,
        UT_BUF_LEN_21 = 15,
        UT_BUF_LEN_22 = 3,
        UT_BUF_LEN_23 = 5,
        UT_BUF_LEN_24 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_21 + UT_BUF_LEN_22 + UT_BUF_LEN_23 + UT_BUF_LEN_24,
        UT_BUF_TO_COPY = UT_BUF_LEN_11 + UT_BUF_LEN_12 + UT_BUF_LEN_13,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[4];
    size_t                    iols1[3] = {UT_BUF_LEN_11, UT_BUF_LEN_12, UT_BUF_LEN_13};
    size_t                    iols2[4] = {UT_BUF_LEN_21, UT_BUF_LEN_22, UT_BUF_LEN_23, UT_BUF_LEN_24};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;
    heni_iobuf_list_fiter_t   iofi3;

    HENI_UT_CHECK((size_t)UT_BUF_LEN_TOT > (size_t)UT_BUF_TO_COPY);

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 4, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    heniIOBufListFIterInit(&iofi2, &iol2);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_TO_COPY), UT_BUF_TO_COPY);

    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi1));
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_TO_COPY, UT_BUF_SEED1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi2));
    heniIOBufListFIterInit(&iofi3, &iol2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi3, UT_BUF_TO_COPY), UT_BUF_TO_COPY);
    HENI_UT_CHECK(heniIOBufListFIterEquals(&iofi3, &iofi2));
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_TO_COPY, UT_BUF_SEED1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, UT_BUF_TO_COPY, UT_BUF_LEN_TOT, UT_BUF_SEED2);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyFromEntireNode_ShouldAllAppropriateValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x22,
        UT_BUF_SEED2 = 0x80 + 0x33,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 6,
        UT_BUF_LEN_13 = 8,
        UT_BUF_LEN_21 = 1,
        UT_BUF_LEN_22 = 3,
        UT_BUF_LEN_23 = 5,
        UT_BUF_LEN_24 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_11 + UT_BUF_LEN_12 + UT_BUF_LEN_13,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[4];
    size_t                    iols1[3] = {UT_BUF_LEN_11, UT_BUF_LEN_12, UT_BUF_LEN_13};
    size_t                    iols2[4] = {UT_BUF_LEN_21, UT_BUF_LEN_22, UT_BUF_LEN_23, UT_BUF_LEN_24};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;
    heni_iobuf_list_fiter_t   iofi3;

    HENI_UT_CHECK((size_t)UT_BUF_LEN_TOT == (size_t)(UT_BUF_LEN_21 + UT_BUF_LEN_22 + UT_BUF_LEN_23 + UT_BUF_LEN_24));

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 4, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi1, UT_BUF_LEN_11), UT_BUF_LEN_11);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi1), UT_PTR_UT2LN(&(ions1[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi1, &(ions1[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi1, &(ions1[1]), 0);
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi2, UT_BUF_LEN_21), UT_BUF_LEN_21);
    heniIOBufListFIterCopy(&iofi2, &iofi3);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_LEN_12), UT_BUF_LEN_12);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi1));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi1), UT_PTR_UT2LN(&(ions1[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi1, &(ions1[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi1, &(ions1[2]), 0);
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi2));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi3, UT_BUF_LEN_12), UT_BUF_LEN_12);
    HENI_UT_CHECK(heniIOBufListFIterEquals(&iofi3, &iofi2));
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_21, UT_BUF_SEED2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, UT_BUF_LEN_21, UT_BUF_LEN_21 + UT_BUF_LEN_12, UT_BUF_SEED1 + UT_BUF_LEN_11 - UT_BUF_LEN_21);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, UT_BUF_LEN_21 + UT_BUF_LEN_12, UT_BUF_LEN_TOT, UT_BUF_SEED2);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyToEntireNode_ShouldAllAppropriateValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x44,
        UT_BUF_SEED2 = 0x80 + 0x11,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 6,
        UT_BUF_LEN_13 = 8,
        UT_BUF_LEN_21 = 1,
        UT_BUF_LEN_22 = 3,
        UT_BUF_LEN_23 = 5,
        UT_BUF_LEN_24 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_11 + UT_BUF_LEN_12 + UT_BUF_LEN_13,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[4];
    size_t                    iols1[3] = {UT_BUF_LEN_11, UT_BUF_LEN_12, UT_BUF_LEN_13};
    size_t                    iols2[4] = {UT_BUF_LEN_21, UT_BUF_LEN_22, UT_BUF_LEN_23, UT_BUF_LEN_24};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;
    heni_iobuf_list_fiter_t   iofi3;

    HENI_UT_CHECK((size_t)UT_BUF_LEN_TOT == (size_t)(UT_BUF_LEN_21 + UT_BUF_LEN_22 + UT_BUF_LEN_23 + UT_BUF_LEN_24));

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 4, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi1, UT_BUF_LEN_11 + UT_BUF_LEN_12 / 2), UT_BUF_LEN_11 + UT_BUF_LEN_12 / 2);
    heniIOBufListFIterCopy(&iofi1, &iofi3);
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi2, UT_BUF_LEN_21 + UT_BUF_LEN_22 + 1), UT_BUF_LEN_21 + UT_BUF_LEN_22 + 1);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi2), UT_PTR_UT2LN(&(ions2[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi2, &(ions2[2]), 1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi2, &(ions2[2]), 1);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_LEN_23 - 1), UT_BUF_LEN_23 - 1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi1));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi3, UT_BUF_LEN_23 - 1), UT_BUF_LEN_23 - 1);
    HENI_UT_CHECK(heniIOBufListFIterEquals(&iofi3, &iofi1));
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);


    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi2));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi2), UT_PTR_UT2LN(&(ions2[3])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi2, &(ions2[3]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi2, &(ions2[3]), 0);
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_21 + UT_BUF_LEN_22 + 1, UT_BUF_SEED2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, UT_BUF_LEN_21 + UT_BUF_LEN_22 + 1, UT_BUF_LEN_21 + UT_BUF_LEN_22 + UT_BUF_LEN_23, UT_BUF_SEED1 + (UT_BUF_LEN_11 + UT_BUF_LEN_12 / 2) - (UT_BUF_LEN_21 + UT_BUF_LEN_22 + 1));
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, UT_BUF_LEN_21 + UT_BUF_LEN_22 + UT_BUF_LEN_23, UT_BUF_LEN_TOT, UT_BUF_SEED2);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterCopyBetweenEntireNodes_ShouldAllAppropriateValuesMatch(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x11,
        UT_BUF_SEED2 = 0x80 + 0x55,
        UT_BUF_LEN_1 = 2,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions1[3];
    ut_iobuf_node_t           ions2[3];
    size_t                    iols1[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    size_t                    iols2[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol1;
    heni_iobuf_list_t         iol2;
    heni_iobuf_list_fiter_t   iofi1;
    heni_iobuf_list_fiter_t   iofi2;

    UT_IOBUF_LIST_INI(&iol1, ions1, iols1, 3, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iol2, ions2, iols2, 3, UT_BUF_SEED2);
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi1, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi1), UT_PTR_UT2LN(&(ions1[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi1, &(ions1[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi1, &(ions1[1]), 0);
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi2, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi2), UT_PTR_UT2LN(&(ions2[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi2, &(ions2[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi2, &(ions2[1]), 0);

    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(&iofi1, &iofi2, UT_BUF_LEN_2), UT_BUF_LEN_2);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi1));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi1), UT_PTR_UT2LN(&(ions1[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi1, &(ions1[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi1, &(ions1[2]), 0);
    heniIOBufListFIterInit(&iofi1, &iol1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi1, 0, UT_BUF_LEN_TOT, UT_BUF_SEED1);

    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi2));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi2), UT_PTR_UT2LN(&(ions2[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi2, &(ions2[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi2, &(ions2[2]), 0);
    heniIOBufListFIterInit(&iofi2, &iol2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, 0, UT_BUF_LEN_1, UT_BUF_SEED2);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, UT_BUF_LEN_1, UT_BUF_LEN_1 + UT_BUF_LEN_2, UT_BUF_SEED1);
    HENI_UT_CHECK_IOBUF_CONTENTS(&iofi2, UT_BUF_LEN_1 + UT_BUF_LEN_2, UT_BUF_LEN_TOT, UT_BUF_SEED2);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnEmptyList_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    heniIOBufListInit(&iolHead);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), 0);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    HENI_UT_CHECK(! heniIOBufListFIterSplitAt(&iofi, &iolTail, NULL));

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), 0);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x011,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 7,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    heniIOBufListFIterFinish(&iofi);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    HENI_UT_CHECK(! heniIOBufListFIterSplitAt(&iofi, &iolTail, NULL));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnFrontNodeAligned_ShouldAllNodesBeMovedToTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x33,
        UT_BUF_LEN_1 = 4,
        UT_BUF_LEN_2 = 2,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;
    heni_iobuf_list_node_t    xioln;

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    HENI_UT_CHECK(! heniIOBufListFIterSplitAt(&iofi, &iolTail, &xioln));

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), 0);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnBackNodeAligned_ShouldBackNodeBeMovedToTailListAndOtherNodesRemainOnHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x22,
        UT_BUF_LEN_1 = 1,
        UT_BUF_LEN_2 = 2,
        UT_BUF_LEN_3 = 3,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;
    heni_iobuf_list_node_t    xioln;

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1 + UT_BUF_LEN_2), UT_BUF_LEN_1 + UT_BUF_LEN_2);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    HENI_UT_CHECK(! heniIOBufListFIterSplitAt(&iofi, &iolTail, &xioln));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_1 + UT_BUF_LEN_2);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_3);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnMiddleNodeAligned_ShouldNodesFromMiddleNodeBeMovedToTailListAndOtherNodesRemainOnHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x22,
        UT_BUF_LEN_1 = 1,
        UT_BUF_LEN_2 = 2,
        UT_BUF_LEN_3 = 3,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;
    heni_iobuf_list_node_t    xioln;

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);

    HENI_UT_CHECK(! heniIOBufListFIterSplitAt(&iofi, &iolTail, &xioln));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_1);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_2 + UT_BUF_LEN_3);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnFrontNodeUnaligned_ShouldFrontNodeRemainOnHeadListAndAllNodesBeMovedToTailListWithNewExtraFrontNode(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x55,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 6,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = (UT_BUF_LEN_1 + 1) / 2,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;
    ut_iobuf_node_t           xioln;

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK(heniIOBufListFIterSplitAt(&iofi, &iolTail, UT_PTR_UT2LN(&xioln)));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), (UT_BUF_LEN_1 - UT_BUF_LEN_INNODE_OFF) + UT_BUF_LEN_2 + UT_BUF_LEN_3);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&xioln));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(xioln)));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_1 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnBackNodeUnaligned_ShouldBackAndPreviousNodesRemainOnHeadListAndNewExtraFrontNodeBeAddedToTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 2,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = (UT_BUF_LEN_3 + 1) / 2,
        UT_BUF_LEN_TOTAL_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_INNODE_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;
    ut_iobuf_node_t           xioln;

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK(heniIOBufListFIterSplitAt(&iofi, &iolTail, UT_PTR_UT2LN(&xioln)));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOTAL_OFF);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_3 - UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&xioln));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(xioln)));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_3 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterSplitOnMiddleNodeUnaligned_ShouldMiddleAndPreviousNodesRemainOnHeadListAndNextNodesBeMovedToTailListWitNewExtraFrontNode(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x77,
        UT_BUF_LEN_1 = 7,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 9,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = UT_BUF_LEN_2 / 2,
        UT_BUF_LEN_TOTAL_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_INNODE_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;
    ut_iobuf_node_t           xioln;

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK(heniIOBufListFIterSplitAt(&iofi, &iolTail, UT_PTR_UT2LN(&xioln)));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOTAL_OFF);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_TOT - UT_BUF_LEN_TOTAL_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&xioln));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(xioln)));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeTwoEmptyLists_ShouldBothListsRemainEmptyAndIterationBeFinishedForHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    heniIOBufListInit(&iolHead);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), 0);
    heniIOBufListInit(&iolTail);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), 0);

    heniIOBufListFIterSetAfterMerge(&iofi, &iolHead, &iolTail);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), 0);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetNode(&iofi), NULL);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeNonemptyAndEmptyLists_ShouldListsRemainAsBeforeAndIterationBeFinishedForHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x33,
        UT_BUF_LEN_11 = 7,
        UT_BUF_LEN_12 = 3,
        UT_BUF_LEN_H = UT_BUF_LEN_11 + UT_BUF_LEN_12,
    };
    ut_iobuf_node_t           ions1[2];
    size_t                    iols1[2] = {UT_BUF_LEN_11, UT_BUF_LEN_12};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iolHead, ions1, iols1, 2, UT_BUF_SEED1);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_H);
    heniIOBufListInit(&iolTail);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    heniIOBufListFIterSetAfterMerge(&iofi, &iolHead, &iolTail);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_H);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_11), UT_BUF_LEN_11);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_12), UT_BUF_LEN_12);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeEmptyAndNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmptyAndIterationBeFinishedForHeadList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED2 = 0x80 + 0x55,
        UT_BUF_LEN_21 = 11,
        UT_BUF_LEN_22 = 3,
        UT_BUF_LEN_T = UT_BUF_LEN_21 + UT_BUF_LEN_22,
    };
    ut_iobuf_node_t           ions2[2];
    size_t                    iols2[2] = {UT_BUF_LEN_21, UT_BUF_LEN_22};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    heniIOBufListInit(&iolHead);
    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), 0);
    UT_IOBUF_LIST_INI(&iolTail, ions2, iols2, 2, UT_BUF_SEED2);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_T);

    heniIOBufListFIterSetAfterMerge(&iofi, &iolHead, &iolTail);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_T);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_21), UT_BUF_LEN_21);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_22), UT_BUF_LEN_22);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterMergeTwoNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmptyAndIterationBeAtFrontOfFormerTailList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x11,
        UT_BUF_SEED2 = 0x80 + 0x55,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 4,
        UT_BUF_LEN_21 = 6,
        UT_BUF_LEN_22 = 8,
        UT_BUF_LEN_H = UT_BUF_LEN_11 + UT_BUF_LEN_12,
        UT_BUF_LEN_T = UT_BUF_LEN_21 + UT_BUF_LEN_22,
        UT_BUF_LEN_TOT = UT_BUF_LEN_H + UT_BUF_LEN_T
    };
    ut_iobuf_node_t           ions1[2];
    ut_iobuf_node_t           ions2[2];
    size_t                    iols1[2] = {UT_BUF_LEN_11, UT_BUF_LEN_12};
    size_t                    iols2[2] = {UT_BUF_LEN_21, UT_BUF_LEN_22};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iolHead, ions1, iols1, 2, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iolTail, ions2, iols2, 2, UT_BUF_SEED2);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_H);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_T);

    heniIOBufListFIterSetAfterMerge(&iofi, &iolHead, &iolTail);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 4);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[0]), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_11), UT_BUF_LEN_11);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_12), UT_BUF_LEN_12);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_21), UT_BUF_LEN_21);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_22), UT_BUF_LEN_22);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterNullMergeTwoNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x11,
        UT_BUF_SEED2 = 0x80 + 0x55,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 4,
        UT_BUF_LEN_21 = 6,
        UT_BUF_LEN_22 = 8,
        UT_BUF_LEN_H = UT_BUF_LEN_11 + UT_BUF_LEN_12,
        UT_BUF_LEN_T = UT_BUF_LEN_21 + UT_BUF_LEN_22,
        UT_BUF_LEN_TOT = UT_BUF_LEN_H + UT_BUF_LEN_T
    };
    ut_iobuf_node_t           ions1[2];
    ut_iobuf_node_t           ions2[2];
    size_t                    iols1[2] = {UT_BUF_LEN_11, UT_BUF_LEN_12};
    size_t                    iols2[2] = {UT_BUF_LEN_21, UT_BUF_LEN_22};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iolHead, ions1, iols1, 2, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iolTail, ions2, iols2, 2, UT_BUF_SEED2);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_H);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_T);

    heniIOBufListFIterSetAfterMerge(NULL, &iolHead, &iolTail);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 4);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_11), UT_BUF_LEN_11);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_12), UT_BUF_LEN_12);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_21), UT_BUF_LEN_21);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_22), UT_BUF_LEN_22);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
mergeTwoNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmpty(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED1 = 0x11,
        UT_BUF_SEED2 = 0x80 + 0x55,
        UT_BUF_LEN_11 = 2,
        UT_BUF_LEN_12 = 4,
        UT_BUF_LEN_21 = 6,
        UT_BUF_LEN_22 = 8,
        UT_BUF_LEN_H = UT_BUF_LEN_11 + UT_BUF_LEN_12,
        UT_BUF_LEN_T = UT_BUF_LEN_21 + UT_BUF_LEN_22,
        UT_BUF_LEN_TOT = UT_BUF_LEN_H + UT_BUF_LEN_T
    };
    ut_iobuf_node_t           ions1[2];
    ut_iobuf_node_t           ions2[2];
    size_t                    iols1[2] = {UT_BUF_LEN_11, UT_BUF_LEN_12};
    size_t                    iols2[2] = {UT_BUF_LEN_21, UT_BUF_LEN_22};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iolHead, ions1, iols1, 2, UT_BUF_SEED1);
    UT_IOBUF_LIST_INI(&iolTail, ions2, iols2, 2, UT_BUF_SEED2);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_H);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_T);

    heniIOBufListMerge(&iolHead, &iolTail);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 4);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_11), UT_BUF_LEN_11);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions1[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions1[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_12), UT_BUF_LEN_12);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_21), UT_BUF_LEN_21);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions2[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions2[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_22), UT_BUF_LEN_22);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterTryConcatenateOnFrontNode_ShouldNothingHappen(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x33,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 6,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = (UT_BUF_LEN_1 + 1) / 2,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), NULL);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_INNODE_OFF);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterTryConcatenateDiscontiguousNodes_ShouldNothingHappen(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x11,
        UT_BUF_LEN_1 = 4,
        UT_BUF_LEN_2 = 2,
        UT_BUF_LEN_3 = 8,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = (UT_BUF_LEN_2 + 1) / 2,
        UT_BUF_LEN_TOTAL_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_INNODE_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK((uint8_t const *)&(ions[0].mem[0]) + UT_BUF_LEN_1 != (uint8_t const *)&(ions[1].mem[0]));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), NULL);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterTryConcatenateOnMiddleNode_ShouldRemoveMiddleNodeAndMoveIterToPreviousNodeModifyingList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x55,
        UT_BUF_LEN_1 = 9,
        UT_BUF_LEN_2 = 7,
        UT_BUF_LEN_3 = 5,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = (UT_BUF_LEN_2 + 1) / 2,
        UT_BUF_LEN_TOTAL_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_INNODE_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    HENI_UT_CHECK(UT_BUF_LEN_1 + UT_BUF_LEN_2 <= UT_DEF_IOBUF_NODE_LEN);
    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_PTR_OVERRIDE(&(ions[1]), iols[1], &(ions[0].mem[iols[0]]));
    HENI_UT_CHECK((uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[0]))) + \
            heniIOBufNodeGetMemLen(UT_PTR_UT2LN(&(ions[0]))) == \
                    (uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[1]))));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), UT_PTR_UT2LN(&(ions[1])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 + UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 + UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterTryConcatenateOnBackNode_ShouldRemoveBackNodeAndMoveIterToPreviousNodeModifyingList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x22,
        UT_BUF_LEN_1 = 9,
        UT_BUF_LEN_2 = 7,
        UT_BUF_LEN_3 = 5,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = (UT_BUF_LEN_3 - 1),
        UT_BUF_LEN_TOTAL_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_INNODE_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    HENI_UT_CHECK(UT_BUF_LEN_2 + UT_BUF_LEN_3 <= UT_DEF_IOBUF_NODE_LEN);
    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_PTR_OVERRIDE(&(ions[2]), iols[2], &(ions[1].mem[iols[1]]));
    HENI_UT_CHECK((uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[1]))) + \
            heniIOBufNodeGetMemLen(UT_PTR_UT2LN(&(ions[1]))) == \
                    (uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[2]))));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), UT_PTR_UT2LN(&(ions[2])));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2 + UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2 + UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_3 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
fiterTryConcatenateTwiceNode_ShouldModifyListAndIterAccordingly(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x22,
        UT_BUF_LEN_1 = 3,
        UT_BUF_LEN_2 = 4,
        UT_BUF_LEN_3 = 5,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = 0,
        UT_BUF_LEN_TOTAL_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_INNODE_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iol;
    heni_iobuf_list_fiter_t   iofi;

    HENI_UT_CHECK((size_t)UT_BUF_LEN_TOT <= (size_t)UT_DEF_IOBUF_NODE_LEN);
    UT_IOBUF_LIST_INI(&iol, ions, iols, 3, UT_BUF_SEED);
    UT_PTR_OVERRIDE(&(ions[1]), iols[1], &(ions[0].mem[iols[0]]));
    UT_PTR_OVERRIDE(&(ions[2]), iols[2], &(ions[0].mem[iols[0] + iols[1]]));
    HENI_UT_CHECK((uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[0]))) + \
            heniIOBufNodeGetMemLen(UT_PTR_UT2LN(&(ions[0]))) == \
                    (uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[1]))));
    HENI_UT_CHECK((uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[1]))) + \
            heniIOBufNodeGetMemLen(UT_PTR_UT2LN(&(ions[1]))) == \
                    (uint8_t const *)heniIOBufNodeGetMemPtr(UT_PTR_UT2LN(&(ions[2]))));
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), UT_BUF_LEN_INNODE_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), NULL);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iol));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iol), 1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iol), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOT - UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOT - UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iol);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 + UT_BUF_LEN_2);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), UT_BUF_LEN_1 + UT_BUF_LEN_2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
}



HENI_UT_FUNCT_DEF_PREFIX void
splitUnalignedMergeAndConcatenate_ShouldRestoreOriginalList(
) HENI_UT_FUNCT_DEF_SUFFIX
{
    enum
    {
        UT_BUF_SEED = 0x44,
        UT_BUF_LEN_1 = 7,
        UT_BUF_LEN_2 = 5,
        UT_BUF_LEN_3 = 9,
        UT_BUF_LEN_TOT = UT_BUF_LEN_1 + UT_BUF_LEN_2 + UT_BUF_LEN_3,
        UT_BUF_LEN_INNODE_OFF = UT_BUF_LEN_2 / 2,
        UT_BUF_LEN_TOTAL_OFF = UT_BUF_LEN_1 + UT_BUF_LEN_INNODE_OFF,
    };
    ut_iobuf_node_t           ions[3];
    size_t                    iols[3] = {UT_BUF_LEN_1, UT_BUF_LEN_2, UT_BUF_LEN_3};
    heni_iobuf_list_t         iolHead;
    heni_iobuf_list_t         iolTail;
    heni_iobuf_list_fiter_t   iofi;
    heni_iobuf_list_fiter_t   iofiBackup;
    ut_iobuf_node_t           xioln;

    /* preparation */

    UT_IOBUF_LIST_INI(&iolHead, ions, iols, 3, UT_BUF_SEED);
    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);
    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_TOTAL_OFF), UT_BUF_LEN_TOTAL_OFF);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);

    /* split + verification */

    HENI_UT_CHECK(heniIOBufListFIterSplitAt(&iofi, &iolTail, UT_PTR_UT2LN(&xioln)));

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOTAL_OFF);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 2);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), UT_BUF_LEN_TOT - UT_BUF_LEN_TOTAL_OFF);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&xioln));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(xioln)));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    /* merge plus verification */

    heniIOBufListFIterSetAfterMerge(&iofi, &iolHead, &iolTail);

    heniIOBufListFIterCopy(&iofi, &iofiBackup);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 4);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);

    HENI_UT_CHECK(heniIOBufListIsEmpty(&iolTail));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolTail), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolTail), 0);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(xioln)));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(xioln)));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(xioln), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterInit(&iofi, &iolTail);
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolTail);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);

    heniIOBufListFIterCopy(&iofiBackup, &iofi);

    /* concatenation plus verification */
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), UT_PTR_UT2LN(&(xioln)));
    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterTryConcatenateWithPrevious(&iofi), NULL);

    HENI_UT_CHECK(! heniIOBufListIsEmpty(&iolHead));
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCount(&iolHead), 3);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListGetCapacity(&iolHead), UT_BUF_LEN_TOT);

    HENI_UT_CHECK_PTR_EQ(heniIOBufListFIterGetList(&iofi), &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF), UT_BUF_LEN_2 - UT_BUF_LEN_INNODE_OFF);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);

    heniIOBufListFIterInit(&iofi, &iolHead);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[0])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[0]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_1), UT_BUF_LEN_1);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[1])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[1]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_2), UT_BUF_LEN_2);
    HENI_UT_CHECK(heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), UT_PTR_UT2LN(&(ions[2])));
    HENI_UT_CHECK_IOBUF_MEMPTR_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_IOBUF_MEMLEN_EQ(&iofi, &(ions[2]), 0);
    HENI_UT_CHECK_SZ_EQ(heniIOBufListFIterTryAdvance(&iofi, UT_BUF_LEN_3), UT_BUF_LEN_3);
    HENI_UT_CHECK(! heniIOBufListFIterIsActive(&iofi));
    HENI_UT_CHECK_IOBUF_NODE_EQ(heniIOBufListFIterGetNode(&iofi), NULL);
    HENI_UT_CHECK_IOBUF_MEMPTR_NULL(&iofi);
    HENI_UT_CHECK_IOBUF_MEMLEN_ZERO(&iofi);
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
    HENI_UT_RUN_TEST(addOneFront_ShouldContainOneBuf);
    HENI_UT_RUN_TEST(addOneBack_ShouldContainOneBuf);
    HENI_UT_RUN_TEST(addTwoFront_ShouldContainTwoBufs);
    HENI_UT_RUN_TEST(addTwoBack_ShouldContainTwoBufs);
    HENI_UT_RUN_TEST(addOneFrontOneBack_ShouldContainTwoBufs);
    HENI_UT_RUN_TEST(addOneBackOneFront_ShouldContainTwoBufs);
    HENI_UT_RUN_TEST(fiterAlignedIteration_ShouldCorrectlyManipulateOffsets);
    HENI_UT_RUN_TEST(fiterUnalignedIteration_ShouldCorrectlyManipulateOffsets);
    HENI_UT_RUN_TEST(fiterNonprogressIteration_ShouldNotChangeOffset);
    HENI_UT_RUN_TEST(fiterSetAndGetOnEmptyList_ShouldReturnError);
    HENI_UT_RUN_TEST(fiterSetAndGetOnFinishedIteration_ShouldReturnError);
    HENI_UT_RUN_TEST(fiterSetAndGetOnOneBufList_ShouldValuesMatch);
    HENI_UT_RUN_TEST(fiterSetAndGetOnTwoBufList_ShouldValuesMatch);
    HENI_UT_RUN_TEST(fiterSetAndGetOnThreeBufList_ShouldValuesMatch);
    HENI_UT_RUN_TEST(fiterSetAndGetWithSingleOverwrite_ShouldValuesMatch);
    HENI_UT_RUN_TEST(fiterSetAndGetWithCompleteOverwrite_ShouldValuesMatch);
    HENI_UT_RUN_TEST(fiterCopyToEmptyBuffer_ShouldNothingChange);
    HENI_UT_RUN_TEST(fiterCopyToNonEmptyBufferFromEmptyList_ShouldNothingChange);
    HENI_UT_RUN_TEST(fiterCopyToMatchingBuffer_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromOffsetZeroToShorterBuffer_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromCloseToEndToShorterBuffer_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromCloseToEndToOverlengthBuffer_ShouldValuesInBufferMatchIfInBuffer);
    HENI_UT_RUN_TEST(fiterCopyFromFrontNodeToMatchingBuffer_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromBackNodeToMatchingBuffer_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromMiddleNodeToMatchingBuffer_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromEmptyBuffer_ShouldNothingChange);
    HENI_UT_RUN_TEST(fiterCopyFromNonEmptyBufferToEmptyList_ShouldNothingChange);
    HENI_UT_RUN_TEST(fiterCopyFromMatchingBuffer_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromShorterBufferToOffsetZero_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromShorterBufferToCloseToEnd_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromOverlengthBufferToCloseToEnd_ShouldValuesInBufferMatchIfInBuffer);
    HENI_UT_RUN_TEST(fiterCopyFromMatchingBufferToFrontNode_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromMatchingBufferToBackNode_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromMatchingBufferToMiddleNode_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyFromFinishedToAnother_ShouldNothingChange);
    HENI_UT_RUN_TEST(fiterCopyToAnotherFinished_ShouldNothingChange);
    HENI_UT_RUN_TEST(fiterCopyAllToAnotherAligned_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyAllToAnotherUnaligned_ShouldValuesInBufferMatch);
    HENI_UT_RUN_TEST(fiterCopyLongerToShorter_ShouldAllValuesInShorterMatchLonger);
    HENI_UT_RUN_TEST(fiterCopyShorterToLonger_ShouldAllAppropriateValuesInLongerMatchShorter);
    HENI_UT_RUN_TEST(fiterCopyFromEntireNode_ShouldAllAppropriateValuesMatch);
    HENI_UT_RUN_TEST(fiterCopyToEntireNode_ShouldAllAppropriateValuesMatch);
    HENI_UT_RUN_TEST(fiterCopyBetweenEntireNodes_ShouldAllAppropriateValuesMatch);
    HENI_UT_RUN_TEST(fiterSplitOnEmptyList_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList);
    HENI_UT_RUN_TEST(fiterSplitOnFinishedIteration_ShouldNoSplitBePerformedAndIterationFinishButReferToTailList);
    HENI_UT_RUN_TEST(fiterSplitOnFrontNodeAligned_ShouldAllNodesBeMovedToTailList);
    HENI_UT_RUN_TEST(fiterSplitOnBackNodeAligned_ShouldBackNodeBeMovedToTailListAndOtherNodesRemainOnHeadList);
    HENI_UT_RUN_TEST(fiterSplitOnMiddleNodeAligned_ShouldNodesFromMiddleNodeBeMovedToTailListAndOtherNodesRemainOnHeadList);
    HENI_UT_RUN_TEST(fiterSplitOnFrontNodeUnaligned_ShouldFrontNodeRemainOnHeadListAndAllNodesBeMovedToTailListWithNewExtraFrontNode);
    HENI_UT_RUN_TEST(fiterSplitOnBackNodeUnaligned_ShouldBackAndPreviousNodesRemainOnHeadListAndNewExtraFrontNodeBeAddedToTailList);
    HENI_UT_RUN_TEST(fiterSplitOnMiddleNodeUnaligned_ShouldMiddleAndPreviousNodesRemainOnHeadListAndNextNodesBeMovedToTailListWitNewExtraFrontNode);
    HENI_UT_RUN_TEST(fiterMergeTwoEmptyLists_ShouldBothListsRemainEmptyAndIterationBeFinishedForHeadList);
    HENI_UT_RUN_TEST(fiterMergeNonemptyAndEmptyLists_ShouldListsRemainAsBeforeAndIterationBeFinishedForHeadList);
    HENI_UT_RUN_TEST(fiterMergeEmptyAndNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmptyAndIterationBeFinishedForHeadList);
    HENI_UT_RUN_TEST(fiterMergeTwoNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmptyAndIterationBeAtFrontOfFormerTailList);
    HENI_UT_RUN_TEST(fiterNullMergeTwoNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmpty);
    HENI_UT_RUN_TEST(mergeTwoNonemptyLists_ShouldHeadListGetAllElementsAndTailListBecomeEmpty);
    HENI_UT_RUN_TEST(fiterTryConcatenateOnFrontNode_ShouldNothingHappen);
    HENI_UT_RUN_TEST(fiterTryConcatenateDiscontiguousNodes_ShouldNothingHappen);
    HENI_UT_RUN_TEST(fiterTryConcatenateOnMiddleNode_ShouldRemoveMiddleNodeAndMoveIterToPreviousNodeModifyingList);
    HENI_UT_RUN_TEST(fiterTryConcatenateOnBackNode_ShouldRemoveBackNodeAndMoveIterToPreviousNodeModifyingList);
    HENI_UT_RUN_TEST(fiterTryConcatenateTwiceNode_ShouldModifyListAndIterAccordingly);
    HENI_UT_RUN_TEST(splitUnalignedMergeAndConcatenate_ShouldRestoreOriginalList);
}

