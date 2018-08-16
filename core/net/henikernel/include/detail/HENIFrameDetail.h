/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_FRAME_DETAIL_H__
#define __HENI_FRAME_DETAIL_H__

#ifndef __HENI_FRAME_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_FRAME_H__ */

#include <string.h>



struct heni_frame_addr_s
{
    heni_link_addr_container_t   dstAddr;
    heni_link_addr_container_t   srcAddr;
};



enum
{
    HENI_FRAME_TX_INFO_FLAG_FRAME_COMPOSED = (1 << 0),
    HENI_FRAME_TX_INFO_FLAG_FRAME_ACCEPTED_BY_LOW_LEVEL_STACK = (1 << 1),
    HENI_FRAME_TX_INFO_FLAG_FRAME_PROCESSED_BY_LOW_LEVEL_STACK = (1 << 2),
    HENI_FRAME_TX_INFO_FLAG_FRAME_TRANSMITTED_BY_LOW_LEVEL_STACK = (1 << 3),
    HENI_FRAME_TX_INFO_FLAG_FRAME_TRANSMISSION_ACKED = (1 << 7),
};


struct heni_frame_tx_info_s
{
    uint8_t   flags;
    uint8_t   numAttempts;
    uint8_t   maxAttempts;
};

struct heni_frame_rx_info_s
{
    uint8_t   flags;
};



HENI_INL_FUNCT_DEF_PREFIX void heniFrameAddrCopy(
        heni_frame_addr_t const * faddrSrc,
        heni_frame_addr_t * faddrDst
) HENI_INL_FUNCT_DEF_SUFFIX
{
    memcpy(faddrDst, faddrSrc, sizeof(heni_frame_addr_t));
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t * heniFrameAddrGetSrcLinkAddrPtr(
        heni_frame_addr_t * faddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(faddr->srcAddr.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t const * heniFrameAddrGetSrcLinkAddrConstPtr(
        heni_frame_addr_t const * faddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(faddr->srcAddr.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t * heniFrameAddrGetDstLinkAddrPtr(
        heni_frame_addr_t * faddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(faddr->dstAddr.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t const * heniFrameAddrGetDstLinkAddrConstPtr(
        heni_frame_addr_t const * faddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(faddr->dstAddr.data8[0]);
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                           Frame operations API                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoReset(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    finfo->flags = 0;
    finfo->numAttempts = 0;
    finfo->maxAttempts = 1;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoCopy(
        heni_frame_tx_info_t const * srcFInfo,
        heni_frame_tx_info_t * dstFInfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    memcpy(dstFInfo, srcFInfo, sizeof(heni_frame_tx_info_t));
}



HENI_INL_FUNCT_DEF_PREFIX uint_fast8_t heniFrameTxInfoGetNumAttempts(
        heni_frame_tx_info_t const * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (uint_fast8_t)finfo->numAttempts;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoIncNumAttempts(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    ++finfo->numAttempts;
}



HENI_INL_FUNCT_DEF_PREFIX uint_fast8_t heniFrameTxInfoGetMaxAttempts(
        heni_frame_tx_info_t const * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (uint_fast8_t)finfo->maxAttempts;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoMarkCompositionSuccess(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    finfo->flags |= HENI_FRAME_TX_INFO_FLAG_FRAME_COMPOSED;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoMarkPassingToLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    finfo->flags |= HENI_FRAME_TX_INFO_FLAG_FRAME_ACCEPTED_BY_LOW_LEVEL_STACK;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoMarkProcessingByLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    finfo->flags |= HENI_FRAME_TX_INFO_FLAG_FRAME_PROCESSED_BY_LOW_LEVEL_STACK;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoMarkTransmissionByLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    finfo->flags |= HENI_FRAME_TX_INFO_FLAG_FRAME_TRANSMITTED_BY_LOW_LEVEL_STACK;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameTxInfoMarkAcknowledgmentByLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    finfo->flags |= HENI_FRAME_TX_INFO_FLAG_FRAME_TRANSMISSION_ACKED;
}



HENI_INL_FUNCT_DEF_PREFIX void heniFrameRxInfoReset(
        heni_frame_rx_info_t * finfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    finfo->flags = 0;
}



#endif /* __HENI_FRAME_DETAIL_H__ */

