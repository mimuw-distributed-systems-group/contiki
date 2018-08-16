/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENILinkAddress.h"
#include "HENICommonStubLinkAddress.h"


/**
 * @file
 * HENI: Implementations of common stubs for link-layer addressing.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


HENI_EXT_FUNCT_DEC_PREFIX void heniLinkAddrStubFill(
        uint8_t * laddrPtr,
        size_t seed
) HENI_EXT_FUNCT_DEC_SUFFIX
{
    heni_link_addr_container_t *   a =
            (heni_link_addr_container_t *)laddrPtr;
    size_t   i;
    for (i = 0; i < HENI_LINK_ADDR_MAX_BYTE_SIZE; ++i)
    {
        a->data8[i] = (uint8_t)seed;
        seed = seed >> 8;
    }
}



HENI_EXT_FUNCT_DEF_PREFIX int_fast8_t heniLinkAddrCmp(
        uint8_t const * laddrPtr1,
        uint8_t const * laddrPtr2
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    heni_link_addr_container_t const *   a1 =
            (heni_link_addr_container_t const *)laddrPtr1;
    heni_link_addr_container_t const *   a2 =
            (heni_link_addr_container_t const *)laddrPtr2;
    size_t                               i;
    for (i = 0; i < HENI_LINK_ADDR_MAX_BYTE_SIZE; ++i)
    {
        if (a1->data8[i] != a2->data8[i])
        {
            return a1->data8[i] < a2->data8[i] ? (int_fast8_t)-1 : (int_fast8_t)1;
        }
    }
    return (int_fast8_t)0;
}



HENI_EXT_FUNCT_DEF_PREFIX size_t heniLinkAddrHash(
        uint8_t const * laddrPtr
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    heni_link_addr_container_t const *   a1 =
            (heni_link_addr_container_t const *)laddrPtr;
    size_t   res = 0, i;
    for (i = 0; i < HENI_LINK_ADDR_MAX_BYTE_SIZE; ++i)
    {
        res += a1->data8[i];
    }
    return res;
}



HENI_EXT_FUNCT_DEF_PREFIX void heniLinkAddrCopy(
        uint8_t const * laddrSrcPtr,
        uint8_t * laddrDstPtr
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    heni_link_addr_container_t const *   as =
            (heni_link_addr_container_t const *)laddrSrcPtr;
    heni_link_addr_container_t *         ad =
            (heni_link_addr_container_t *)laddrDstPtr;
    size_t                               i;
    for (i = 0; i < HENI_LINK_ADDR_MAX_BYTE_SIZE; ++i)
    {
        ad->data8[i] = as->data8[i];
    }
}



HENI_EXT_FUNCT_DEF_PREFIX void heniLinkAddrInvalidate(
        uint8_t * laddrPtr
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    heni_link_addr_container_t *   a =
            (heni_link_addr_container_t *)laddrPtr;
    size_t   i;
    for (i = 0; i < HENI_LINK_ADDR_MAX_BYTE_SIZE; ++i)
    {
        a->data8[i] = 19;
    }
}



HENI_EXT_FUNCT_DEF_PREFIX int_fast8_t heniLinkAddrIsValid(
        uint8_t const * laddrPtr
) HENI_EXT_FUNCT_DEF_SUFFIX
{
    heni_link_addr_container_t const *   a =
            (heni_link_addr_container_t const *)laddrPtr;
    size_t   i;
    for (i = 0; i < HENI_LINK_ADDR_MAX_BYTE_SIZE; ++i)
    {
        if (a->data8[i] != 19)
        {
            return (int_fast8_t)1;
        }
    }
    return (int_fast8_t)0;
}
