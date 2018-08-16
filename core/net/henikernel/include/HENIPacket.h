/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_PACKET_H__
#define __HENI_PACKET_H__

#include "HENIBase.h"


/**
 * @file
 * HENI: Types, macros, and functions concerning HENI packets.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


struct heni_packet_s;
/** A packet routed by a HENI kernel. */
typedef struct heni_packet_s   heni_packet_t;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          Packet-addressing API                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Resets a HENI packet address with default values.
 * @param paddr The packet address.
 */
HENI_API_FUNCT_DEC_PREFIX void heniPacketAddrReset(
        heni_packet_addr_t * paddr
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Copies a HENI packet address.
 * @param paddrSrc The source packet address.
 * @param paddrDst The destination packet address.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniPacketAddrCopy(
        heni_packet_addr_t const * paddrSrc,
        heni_packet_addr_t * paddrDst
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a pointer to the source label
 * from a HENI packet address.
 * @param paddr The packet address.
 * @return A pointer to the source label.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t * heniPacketAddrGetSrcLabelPtr(
        heni_packet_addr_t * paddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a constant pointer to the source label
 * from a HENI packet address.
 * @param paddr The packet address.
 * @return A constant pointer to the source label.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t const * heniPacketAddrGetSrcLabelConstPtr(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a pointer to the destination label
 * from a HENI packet address.
 * @param paddr The packet address.
 * @return A pointer to the destination label.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t * heniPacketAddrGetDstLabelPtr(
        heni_packet_addr_t * paddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a constant pointer to the destination label
 * from a HENI packet address.
 * @param paddr The packet address.
 * @return A constant pointer to the destination label.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t const * heniPacketAddrGetDstLabelConstPtr(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the hop limit from a HENI packet address.
 * @param paddr The packet address.
 * @return The hop limit.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_packet_hop_limit_t heniPacketAddrGetHopLimit(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets the hop limit in a HENI packet address.
 * @param paddr The packet address.
 * @param hlim The hop limit.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniPacketAddrSetHopLimit(
        heni_packet_addr_t * paddr,
        heni_packet_hop_limit_t hlim
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the identifier of a HENI instance
 * for a given HENI packet address.
 * @param paddr The packet address.
 * @return The instance identifier.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_instance_id_t heniPacketAddrGetInstanceID(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets the identifier of a HENI instance
 * for a given HENI packet address.
 * @param paddr The packet address.
 * @param iid The instance identifier.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniPacketAddrSetInstanceID(
        heni_packet_addr_t * paddr,
        heni_instance_id_t iid
) HENI_INL_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          Packet operations API                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Resets packet transmission information.
 * @param pinfo The packet tranmission information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniPacketTxInfoReset(
        heni_packet_tx_info_t * pinfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Copies packet transmission information.
 * @param srcPInfo The source information.
 * @param dstPInfo The destination information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniPacketTxInfoCopy(
        heni_packet_tx_info_t const * srcPInfo,
        heni_packet_tx_info_t * dstPInfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Sets the error field within packet
 * transmission information.
 * @param pinfo The packet tranmission information.
 * @param status The status to be set.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniPacketTxInfoSetError(
        heni_packet_tx_info_t * pinfo,
        uint_fast8_t status
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Updates packet transmission information
 * with frame transmission information.
 * @param pinfo The packet transmission information.
 * @param finfo The frame transmission information.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniPacketTxInfoUpdateWithFrameTxInfo(
        heni_packet_tx_info_t * pinfo,
        heni_frame_tx_info_t const * finfo
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * Resets packet reception information.
 * @param pinfo The packet reception information.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniPacketRxInfoReset(
        heni_packet_rx_info_t * pinfo
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Updates packet reception information
 * with frame reception information.
 * @param pinfo The packet reception information.
 * @param finfo The frame reception information.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniPacketRxInfoUpdateWithFrameRxInfo(
        heni_packet_rx_info_t * pinfo,
        heni_frame_rx_info_t const * finfo
) HENI_HID_FUNCT_DEC_SUFFIX;


#include "detail/HENIPacketDetail.h"

#endif /* __HENI_PACKET_H__ */

