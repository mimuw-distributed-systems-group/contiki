/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIFrame.h"
#include "HENILinkAddress.h"


HENI_API_FUNCT_DEC_PREFIX void heniFrameAddrReset(
        heni_frame_addr_t * faddr
) HENI_API_FUNCT_DEC_SUFFIX
{
    heniLinkAddrInvalidate(heniFrameAddrGetDstLinkAddrPtr(faddr));
    heniLinkAddrInvalidate(heniFrameAddrGetSrcLinkAddrPtr(faddr));
}

