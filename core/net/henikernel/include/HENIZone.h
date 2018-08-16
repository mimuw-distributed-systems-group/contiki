/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_ZONE_H__
#define __HENI_ZONE_H__

#include "HENIBase.h"
#include "HENILabel.h"

/**
 * @file
 * HENI: Basic functionality of a HENI zone.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


/**
 * A zone in HENI.
 */
struct heni_zone_s;
typedef struct heni_zone_s   heni_zone_t;


/**
 * Returns the local identifier for a zone entry.
 * @param zone The zone entry.
 * @return The local identifier (LID) for the entry.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniZoneEntryGetLID(
        heni_zone_t const * zone
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the discriminator for a zone entry.
 * @param zone The zone entry.
 * @return The discriminator for the entry.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_discr_t heniZoneEntryGetDiscr(
        heni_zone_t const * zone
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Allocates an entry for a HENI zone table.
 * @param ker The HENI kernel for which
 *   the entry is to be allocated.
 * @param iid The identifier of the instance
 *   in the context of which the entry is
 *   to be allocated.
 * @return A pointer to the allocated zone or
 *   NULL if there is no memory to allocate the zone.
 */
HENI_EXT_FUNCT_DEC_PREFIX heni_zone_t * heniZoneEntryAlloc(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Frees an entry for a HENI zone table.
 * @param ker The HENI kernel for which
 *   the entry is to be freed.
 * @param iid The identifier of the instance
 *   in the context of which the entry is
 *   to be freed.
 * @param zone The zone entry to be freed.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniZoneEntryFree(
        heni_kernel_t * ker,
        heni_instance_id_t iid,
        heni_zone_t * zone
) HENI_EXT_FUNCT_DEC_SUFFIX;



#include "detail/HENIZoneDetail.h"

#endif /* __HENI_ZONE_H__ */
