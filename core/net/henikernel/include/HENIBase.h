/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_BASE_H__
#define __HENI_BASE_H__

/**
 * @file
 * HENI: Basic types, macros, and functions.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>


/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                      Debug mode switching macros                       *
 *                                                                        *
 * ---------------------------------------------------------------------- */

#ifndef HENI_DEBUG
#if (defined(DEBUG))
#define HENI_DEBUG
#endif
#endif /* HENI_DEBUG */



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                        Function linkage macros                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

#ifdef __cplusplus

#define HENI_PRV_DATA_DEC_PREFIX extern "C"
#define HENI_PRV_DATA_DEF_PREFIX

#define HENI_EXT_FUNCT_DEC_PREFIX extern "C"
#define HENI_EXT_FUNCT_DEF_PREFIX
#define HENI_EXT_FUNCT_DEC_SUFFIX
#define HENI_EXT_FUNCT_DEF_SUFFIX

#define HENI_API_FUNCT_DEC_PREFIX extern "C"
#define HENI_API_FUNCT_DEF_PREFIX
#define HENI_API_FUNCT_DEC_SUFFIX
#define HENI_API_FUNCT_DEF_SUFFIX

#define HENI_HID_FUNCT_DEC_PREFIX extern "C"
#define HENI_HID_FUNCT_DEF_PREFIX
#define HENI_HID_FUNCT_DEC_SUFFIX
#define HENI_HID_FUNCT_DEF_SUFFIX

#define HENI_INL_FUNCT_DEC_PREFIX static inline
#define HENI_INL_FUNCT_DEF_PREFIX static inline
#define HENI_INL_FUNCT_DEC_SUFFIX
#define HENI_INL_FUNCT_DEF_SUFFIX

#define HENI_PRV_FUNCT_DEC_PREFIX static
#define HENI_PRV_FUNCT_DEF_PREFIX static
#define HENI_PRV_FUNCT_DEC_SUFFIX
#define HENI_PRV_FUNCT_DEF_SUFFIX

#else

#define HENI_PRV_DATA_DEC_PREFIX extern
#define HENI_PRV_DATA_DEF_PREFIX

#define HENI_EXT_FUNCT_DEC_PREFIX extern
#define HENI_EXT_FUNCT_DEF_PREFIX
#define HENI_EXT_FUNCT_DEC_SUFFIX
#define HENI_EXT_FUNCT_DEF_SUFFIX

#define HENI_API_FUNCT_DEC_PREFIX extern
#define HENI_API_FUNCT_DEF_PREFIX
#define HENI_API_FUNCT_DEC_SUFFIX
#define HENI_API_FUNCT_DEF_SUFFIX

#define HENI_HID_FUNCT_DEC_PREFIX
#define HENI_HID_FUNCT_DEF_PREFIX
#define HENI_HID_FUNCT_DEC_SUFFIX
#define HENI_HID_FUNCT_DEF_SUFFIX

#define HENI_INL_FUNCT_DEC_PREFIX static inline
#define HENI_INL_FUNCT_DEF_PREFIX static inline
#define HENI_INL_FUNCT_DEC_SUFFIX
#define HENI_INL_FUNCT_DEF_SUFFIX

#define HENI_PRV_FUNCT_DEC_PREFIX static
#define HENI_PRV_FUNCT_DEF_PREFIX static
#define HENI_PRV_FUNCT_DEC_SUFFIX
#define HENI_PRV_FUNCT_DEF_SUFFIX

#endif /* __cplusplus */



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                              Basic macros                              *
 *                                                                        *
 * ---------------------------------------------------------------------- */


#define HENI_PP_STRINGIZE(x) HENI_PP_STRINGIZE_IMPL(x)
#define HENI_PP_STRINGIZE_IMPL(x) #x

#ifdef HENI_LABEL_MAX_BIT_SIZE
#if ((HENI_LABEL_MAX_BIT_SIZE) != 64)
#error "HENI_LABEL_MAX_BIT_SIZE must be equal to 64!"
#endif /* (HENI_LABEL_MAX_BIT_SIZE) != 64 */
#else
#define HENI_LABEL_MAX_BIT_SIZE 64
#endif /* defined HENI_LABEL_MAX_BIT_SIZE */

#define HENI_LABEL_MAX_BYTE_SIZE ((HENI_LABEL_MAX_BIT_SIZE + 7) >> 3)

#ifdef HENI_ZONE_LID_MIN_BIT_SIZE
#if ((HENI_ZONE_LID_MIN_BIT_SIZE) != 4)
#error "HENI_ZONE_LID_MIN_BIT_SIZE must be equal to 4!"
#endif /* (HENI_ZONE_LID_MIN_BIT_SIZE) != 4 */
#else
#define HENI_ZONE_LID_MIN_BIT_SIZE 4
#endif /* HENI_ZONE_LID_MIN_BIT_SIZE */

#ifndef HENI_ZONE_LID_MAX_BIT_SIZE
#define HENI_ZONE_LID_MAX_BIT_SIZE 64
#endif /* HENI_ZONE_LID_MAX_BIT_SIZE */

#ifndef HENI_ZONE_DISCR_MAX_BIT_SIZE
#define HENI_ZONE_DISCR_MAX_BIT_SIZE 64
#endif /* HENI_ZONE_DISCR_MAX_BIT_SIZE */


#ifndef HENI_LINK_ADDR_MAX_BYTE_SIZE
/* The address is defined so as to allow for mixed-mode 802.15.4 addressing. */
#define HENI_LINK_ADDR_MAX_BYTE_SIZE 9
#endif /* HENI_LINK_ADDR_MAX_BYTE_SIZE */

#define HENI_MAX_LIDS_IN_LABEL ((HENI_LABEL_MAX_BIT_SIZE) / (HENI_ZONE_LID_MIN_BIT_SIZE))

#ifndef HENI_MAX_NUM_INSTANCES
#define HENI_MAX_NUM_INSTANCES 3
#else
#if ((HENI_MAX_NUM_INSTANCES) <= 0 || (HENI_MAX_NUM_INSTANCES) >= 16)
#error "HENI_MAX_NUM_INSTANCES must be between 1 and 15!"
#endif /* HENI_MAX_NUM_INSTANCES out of bounds */
#endif /* HENI_MAX_NUM_INSTANCES */



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                               Basic types                              *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/** An identifier of a HENI instance. */
typedef uint8_t   heni_instance_id_t;
#define HENI_PRI_INSTANCE_ID PRIu8
/** A counter for HENI instances. */
typedef uint8_t   heni_instance_count_t;
#define HENI_PRI_INSTANCE_COUNT PRIu8

enum
{
    /** An identifier of an invalid HENI instance. */
    HENI_INSTANCE_ID_INVALID = 0,
    /** A minimal identifier of a HENI instance. */
    HENI_INSTANCE_ID_MIN = HENI_INSTANCE_ID_INVALID + 1,
    /** A maximal identifier of a HENI instance. */
    HENI_INSTANCE_ID_MAX = HENI_MAX_NUM_INSTANCES,
};


/** The height of a HENI hierarchy or the identifier of a level. */
typedef uint8_t   heni_level_t;
#define HENI_PRI_LEVEL PRIu8

/** The local identifier of a zone within the parent zone. */
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) == 4 || (HENI_ZONE_LID_MAX_BIT_SIZE) == 8)
#define HENI_PRI_ZONE_LID PRIu8
typedef uint8_t   heni_zone_lid_t;
#elif ((HENI_ZONE_LID_MAX_BIT_SIZE) == 16)
#define HENI_PRI_ZONE_LID PRIu16
typedef uint16_t   heni_zone_lid_t;
#elif ((HENI_ZONE_LID_MAX_BIT_SIZE) == 32)
#define HENI_PRI_ZONE_LID PRIu32
typedef uint32_t   heni_zone_lid_t;
#elif ((HENI_ZONE_LID_MAX_BIT_SIZE) == 64)
#define HENI_PRI_ZONE_LID PRIu64
typedef uint64_t   heni_zone_lid_t;
#else
#error "Invalid value of HENI_ZONE_LID_MAX_BIT_SIZE=" HENI_PP_STRINGIZE(HENI_ZONE_LID_MAX_BIT_SIZE) "!"
#define HENI_PRI_ZONE_LID PRIu8
typedef uint8_t   heni_zone_lid_t;
#endif /* case HENI_ZONE_LID_MAX_BIT_SIZE */

/** A globally unique discriminator of a zone. */
#if ((HENI_ZONE_DISCR_MAX_BIT_SIZE) == 8)
#define HENI_PRI_ZONE_DISCR PRIu8
typedef uint8_t   heni_zone_discr_t;
//#define HENI_ZONE_DISCR_ANY (UINT8_MAX)
#elif ((HENI_ZONE_DISCR_MAX_BIT_SIZE) == 16)
#define HENI_PRI_ZONE_DISCR PRIu16
typedef uint16_t   heni_zone_discr_t;
//#define HENI_ZONE_DISCR_ANY (UINT16_MAX)
#elif ((HENI_ZONE_DISCR_MAX_BIT_SIZE) == 32)
#define HENI_PRI_ZONE_DISCR PRIu32
typedef uint32_t   heni_zone_discr_t;
//#define HENI_ZONE_DISCR_ANY (UINT32_MAX)
#elif ((HENI_ZONE_DISCR_MAX_BIT_SIZE) == 64)
#define HENI_PRI_ZONE_DISCR PRIu64
typedef uint64_t   heni_zone_discr_t;
//#define HENI_ZONE_DISCR_ANY (UINT64_MAX)
#else
#error "Invalid value of HENI_ZONE_DISCR_MAX_BIT_SIZE=" HENI_PP_STRINGIZE(HENI_PRI_ZONE_DISCR) "!"
#define HENI_PRI_ZONE_DISCR PRIu64
typedef uint64_t   heni_zone_discr_t;
//#define HENI_ZONE_DISCR_ANY (UINT64_MAX)
#endif /* case HENI_ZONE_LID_MAX_BIT_SIZE */
//#define HENI_ZONE_DISCR_BAD ((heni_zone_discr_t)0)
//#define HENI_ZONE_DISCR_MIN ((heni_zone_discr_t)1)
//#define HENI_ZONE_DISCR_MAX ((HENI_ZONE_DISCR_ANY) - 1)
//#define HENI_ZONE_DISCR_IS_VALID(d) ((d) >= (HENI_ZONE_DISCR_MIN) && (d) <= HENI_ZONE_DISCR_MAX)

/** A specification of a HENI label. */
typedef uint8_t   heni_lspec_t;
#define HENI_PRI_LSPEC PRIu8

/** A container that is guaranteed to hold any HENI label. */
typedef struct { uint8_t data8[HENI_LABEL_MAX_BYTE_SIZE]; }   heni_label_container_t;

/** A container that is guaranteed to hold any link-layer address used by HENI. */
typedef struct { uint8_t data8[HENI_LINK_ADDR_MAX_BYTE_SIZE]; }   heni_link_addr_container_t;

/** A hop limit for HENI packets. */
typedef uint8_t   heni_packet_hop_limit_t;
#define HENI_PRI_PACKET_HOP_LIMIT PRIu8

/** A HENI input/output (I/O) buffer. */
typedef struct heni_iobuf_s
{
    void *   base;
    size_t   len;
} heni_iobuf_t;



struct heni_instance_s;
/**
 * A HENI instance.
 *
 * This type is hidden to prevent direct
 * manipulation.
 */
typedef struct heni_instance_s   heni_instance_t;



struct heni_kernel_s;
/**
 * A HENI kernel.
 *
 * This type is hidden to prevent direct
 * manipulation.
 */
typedef struct heni_kernel_s   heni_kernel_t;



struct heni_packet_addr_s;
/** Addressing information for a HENI packet. */
typedef struct heni_packet_addr_s   heni_packet_addr_t;

struct heni_packet_tx_info_s;
/** The status of sending a packet. */
typedef struct heni_packet_tx_info_s   heni_packet_tx_info_t;
struct heni_packet_rx_info_s;
/** The status of receiving a packet. */
typedef struct heni_packet_rx_info_s   heni_packet_rx_info_t;

enum
{
    HENI_PACKET_ROUTING_ERROR_NONE = 0,
    HENI_PACKET_ROUTING_ERROR_INVALID_IID,
    HENI_PACKET_ROUTING_ERROR_INSTANCE_NOT_RUNNING,
    HENI_PACKET_ROUTING_ERROR_FRAME_FORWARDING_NOT_INITIATED,
    HENI_PACKET_ROUTING_ERROR_OUT_OF_TOKENS,
    HENI_PACKET_ROUTING_ERROR_HOP_BY_HOP_ACK_FAILED,
    HENI_PACKET_ROUTING_ERROR_NOT_FOR_ME,
};



struct heni_frame_addr_s;
/** Addressing information for a HENI frame. */
typedef struct heni_frame_addr_s   heni_frame_addr_t;

struct heni_frame_tx_info_s;
/** The status of transmitting a frame. */
typedef struct heni_frame_tx_info_s   heni_frame_tx_info_t;
struct heni_frame_rx_info_s;
/** The status of receiving a frame. */
typedef struct heni_frame_rx_info_s   heni_frame_rx_info_t;


/* Additional marcors. */


#include "detail/HENIBaseDetail.h"

#endif /* __HENI_BASE_H__ */

