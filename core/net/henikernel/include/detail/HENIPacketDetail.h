/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_PACKET_DETAIL_H__
#define __HENI_PACKET_DETAIL_H__

#ifndef __HENI_PACKET_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_PACKET_H__ */

#include <string.h>
#include "HENILinkAddress.h"
#include "HENILinkedList.h"
#include "HENIVectoredIO.h"



struct heni_packet_addr_s
{
    heni_label_container_t    dstLabel;
    heni_label_container_t    srcLabel;
    heni_packet_hop_limit_t   hopLimit;
    heni_instance_id_t        instanceId;
};


struct heni_packet_tx_info_s
{
    uint8_t    status;
    uint8_t    numAckedFrames;
    uint16_t   numUnackedFrames;
};

struct heni_packet_rx_info_s
{
    uint8_t PLACEHOLDER;
};


typedef union heni_packet_op_info_u
{
    heni_packet_tx_info_t   ptx;
    heni_packet_rx_info_t   prx;
} heni_packet_op_info_t;


struct heni_packet_s
{
    heni_linked_list_node_t      activeListNode;
    heni_packet_addr_t           paddr;
    heni_iobuf_list_t *          ppld;
    heni_link_addr_container_t   linkLayerSrcNeighborAddr;
    heni_packet_op_info_t        opInfo;
};



HENI_INL_FUNCT_DEF_PREFIX void heniPacketAddrCopy(
        heni_packet_addr_t const * paddrSrc,
        heni_packet_addr_t * paddrDst
) HENI_INL_FUNCT_DEF_SUFFIX
{
    memcpy(paddrDst, paddrSrc, sizeof(heni_packet_addr_t));
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t * heniPacketAddrGetSrcLabelPtr(
        heni_packet_addr_t * paddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(paddr->srcLabel.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t const * heniPacketAddrGetSrcLabelConstPtr(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(paddr->srcLabel.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t * heniPacketAddrGetDstLabelPtr(
        heni_packet_addr_t * paddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(paddr->dstLabel.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t const * heniPacketAddrGetDstLabelConstPtr(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(paddr->dstLabel.data8[0]);
}



HENI_INL_FUNCT_DEF_PREFIX heni_packet_hop_limit_t heniPacketAddrGetHopLimit(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return paddr->hopLimit;
}



HENI_INL_FUNCT_DEF_PREFIX void heniPacketAddrSetHopLimit(
        heni_packet_addr_t * paddr,
        heni_packet_hop_limit_t hlim
) HENI_INL_FUNCT_DEF_SUFFIX
{
    paddr->hopLimit = hlim;
}



HENI_INL_FUNCT_DEF_PREFIX heni_instance_id_t heniPacketAddrGetInstanceID(
        heni_packet_addr_t const * paddr
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return paddr->instanceId;
}



HENI_INL_FUNCT_DEF_PREFIX void heniPacketAddrSetInstanceID(
        heni_packet_addr_t * paddr,
        heni_instance_id_t iid
) HENI_INL_FUNCT_DEF_SUFFIX
{
    paddr->instanceId = iid;
}



/**
 * This is a private implementation function.
 *
 * Returns an active list node for a given HENI packet.
 */
HENI_INL_FUNCT_DEF_PREFIX heni_linked_list_node_t * heniPacketGetActiveListNodeForPacket(
        heni_packet_t * packet
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &packet->activeListNode;
}

/**
 * This is a private implementation function.
 *
 * Returns a HENI packet for a given active list node.
 */
HENI_INL_FUNCT_DEF_PREFIX heni_packet_t * heniPacketGetPacketForActiveListNode(
        heni_linked_list_node_t * lnode
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (heni_packet_t *)((uint8_t *)lnode - offsetof(struct heni_packet_s, activeListNode));
}

/**
 * This is a private implementation function.
 *
 * Returns the payload of a packet.
 */
HENI_INL_FUNCT_DEF_PREFIX heni_iobuf_list_t * heniPacketGetPayloadIOVectorPtr(
        heni_packet_t * packet
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return packet->ppld;
}

/**
 * This is a private implementation function.
 *
 * Returns a pointer to the link-layer address
 * from which a HENI packet has been received.
 * @param paddr The packet address.
 * @return A pointer to the source label.
 */
HENI_INL_FUNCT_DEF_PREFIX uint8_t * heniPacketGetLinkLayerSrcNeighborAddrPtr(
        heni_packet_t * packet
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(packet->linkLayerSrcNeighborAddr.data8[0]);
}



/**
 * This is a private implementation function.
 *
 * Returns a pointer to the link-layer address
 * from which a HENI packet has been received.
 * The address can only be read.
 * @param paddr The packet address.
 * @return A pointer to the source label.
 */
HENI_INL_FUNCT_DEF_PREFIX uint8_t const * heniPacketGetLinkLayerSrcNeighborAddrConstPtr(
        heni_packet_t const * packet
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &(packet->linkLayerSrcNeighborAddr.data8[0]);
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          Packet operations API                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

HENI_INL_FUNCT_DEF_PREFIX void heniPacketTxInfoReset(
        heni_packet_tx_info_t * pinfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    pinfo->status = HENI_PACKET_ROUTING_ERROR_NONE;
    pinfo->numAckedFrames = 0;
    pinfo->numUnackedFrames = 0;
}



HENI_INL_FUNCT_DEF_PREFIX void heniPacketTxInfoCopy(
        heni_packet_tx_info_t const * srcPInfo,
        heni_packet_tx_info_t * dstPInfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    memcpy(dstPInfo, srcPInfo, sizeof(heni_packet_tx_info_t));
}



HENI_INL_FUNCT_DEF_PREFIX void heniPacketTxInfoSetError(
        heni_packet_tx_info_t * pinfo,
        uint_fast8_t status
) HENI_INL_FUNCT_DEF_SUFFIX
{
    pinfo->status = status;
}



HENI_INL_FUNCT_DEF_PREFIX void heniPacketRxInfoReset(
        heni_packet_rx_info_t * pinfo
) HENI_INL_FUNCT_DEF_SUFFIX
{
    pinfo->PLACEHOLDER = 0;
}



#endif /* __HENI_PACKET_DETAIL_H__ */

