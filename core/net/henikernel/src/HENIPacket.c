/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIError.h"
#include "HENIFrame.h"
#include "HENIPacket.h"



enum
{
    HENI_PACKET_ADDR_HOP_LIMIT_MAX = (sizeof(heni_packet_hop_limit_t) << 8) - 1,
};


HENI_API_FUNCT_DEF_PREFIX void heniPacketAddrReset(
        heni_packet_addr_t * paddr
) HENI_API_FUNCT_DEF_SUFFIX
{
    memset(paddr, 0x00, sizeof(heni_packet_addr_t));
    heniPacketAddrSetHopLimit(paddr, HENI_PACKET_ADDR_HOP_LIMIT_MAX);
    heniPacketAddrSetInstanceID(paddr, HENI_INSTANCE_ID_INVALID);
}



HENI_HID_FUNCT_DEF_PREFIX void heniPacketTxInfoUpdateWithFrameTxInfo(
        heni_packet_tx_info_t * pinfo,
        heni_frame_tx_info_t const * finfo
) HENI_HID_FUNCT_DEF_SUFFIX
{
    // FIXME: here a different approach, taking into account whether the frame was actually transmitted
    // FIXME: not all packets have to be acked
    if ((finfo->flags & HENI_FRAME_TX_INFO_FLAG_FRAME_TRANSMISSION_ACKED) != 0)
    {
        HENI_DASSERT(finfo->numAttempts > 0);
        ++pinfo->numAckedFrames;
        pinfo->numUnackedFrames += finfo->numAttempts - 1;
    }
    else
    {
        pinfo->status = HENI_PACKET_ROUTING_ERROR_HOP_BY_HOP_ACK_FAILED;
        pinfo->numUnackedFrames += finfo->numAttempts;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniPacketRxInfoUpdateWithFrameRxInfo(
        heni_packet_rx_info_t * pinfo,
        heni_frame_rx_info_t const * finfo
) HENI_HID_FUNCT_DEF_SUFFIX
{
    /* There is nothing to be done here as of now. */
}

