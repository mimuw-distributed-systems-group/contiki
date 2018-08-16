/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_UT_COMMON_STUB_LINK_ADDRESS_H__
#define __HENI_UT_COMMON_STUB_LINK_ADDRESS_H__


#include "HENILinkAddress.h"


/**
 * @file
 * HENI: Interfaces of common stubs for link-layer addressing.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/**
 * Fills out a link-layer address stub with
 * an address generated from a given seed.
 * @param laddrPtr A pointer to the address.
 * @param seed The seed.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniLinkAddrStubFill(
        uint8_t * laddrPtr,
        size_t seed
) HENI_EXT_FUNCT_DEC_SUFFIX;



#endif /* __HENI_UT_COMMON_STUB_LINK_ADDRESS_H__ */
