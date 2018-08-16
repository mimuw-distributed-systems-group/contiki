/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_FRAME_H__
#define __HENI_FRAME_H__

#include "HENIBase.h"


/**
 * @file
 * HENI: Types, macros, and functions concerning HENI frames.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          Frame-addressing API                          *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Resets a HENI frame address with default values.
 * @param faddr The frame address.
 */
HENI_API_FUNCT_DEC_PREFIX void heniFrameAddrReset(
        heni_frame_addr_t * faddr
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Copies a HENI frame address.
 * @param faddrSrc The source frame address.
 * @param faddrDst The destination frame address.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameAddrCopy(
        heni_frame_addr_t const * faddrSrc,
        heni_frame_addr_t * faddrDst
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a pointer to the source link-layer
 * address from a HENI packet address.
 * @param faddr The frame address.
 * @return A pointer to the source address.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t * heniFrameAddrGetSrcLinkAddrPtr(
        heni_frame_addr_t * faddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a constant pointer to the source
 * link-layer address from a HENI packet address.
 * @param faddr The frame address.
 * @return A constant pointer to the source address.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t const * heniFrameAddrGetSrcLinkAddrConstPtr(
        heni_frame_addr_t const * faddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a pointer to the destination link-layer
 * address from a HENI packet address.
 * @param faddr The frame address.
 * @return A pointer to the destination address.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t * heniFrameAddrGetDstLinkAddrPtr(
        heni_frame_addr_t * faddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a constant pointer to the destination
 * link-layer address from a HENI packet address.
 * @param faddr The frame address.
 * @return A constant pointer to the destination address.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t const * heniFrameAddrGetDstLinkAddrConstPtr(
        heni_frame_addr_t const * faddr
) HENI_INL_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                           Frame operations API                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Resets frame transmission information.
 * @param finfo The frame tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoReset(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Copies frame transmission information.
 * @param srcFInfo The source information.
 * @param dstFInfo The destination information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoCopy(
        heni_frame_tx_info_t const * srcFInfo,
        heni_frame_tx_info_t * dstFInfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns from frame transmission information
 * the number of transmission attempts.
 * @param finfo The frame tranmission information.
 * @return The number of transmission attempts.
 */
HENI_INL_FUNCT_DEC_PREFIX uint_fast8_t heniFrameTxInfoGetNumAttempts(
        heni_frame_tx_info_t const * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Increments in frame transmission information
 * the number of transmission attempts.
 * @param finfo The frame tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoIncNumAttempts(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns from frame transmission information
 * the maximal number of transmission attempts.
 * @param finfo The frame tranmission information.
 * @return The maximal number of transmission attempts.
 */
HENI_INL_FUNCT_DEC_PREFIX uint_fast8_t heniFrameTxInfoGetMaxAttempts(
        heni_frame_tx_info_t const * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets a flag indicating that frame composition
 * has been successful.
 * @param finfo The frame tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoMarkCompositionSuccess(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets a flag indicating that the corresponding
 * frame has been accepted by the low-level stack
 * for transmission.
 * @param finfo The frame tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoMarkPassingToLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets a flag indicating that the corresponding
 * frame has been processed by the low-level stack
 * at least once but not necessarily transmitted.
 * @param finfo The frame tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoMarkProcessingByLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets a flag indicating that the corresponding
 * frame has been transmitted by the low-level stack
 * at least once.
 * @param finfo The frame tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoMarkTransmissionByLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets a flag indicating that the corresponding
 * frame has been delivered by the low-level stack
 * at least once, that is, that the stack has
 * received an acknowledgment for the frame
 * from its recipient.
 * @param finfo The frame tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameTxInfoMarkAcknowledgmentByLowLevelStack(
        heni_frame_tx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;


/**
 * Resets frame reception information.
 * @param finfo The frame reception information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniFrameRxInfoReset(
        heni_frame_rx_info_t * finfo
) HENI_INL_FUNCT_DEC_SUFFIX;


#include "detail/HENIFrameDetail.h"

#endif /* __HENI_FRAME_H__ */

