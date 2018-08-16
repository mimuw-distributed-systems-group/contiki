/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_NEIGHBOR_H__
#define __HENI_NEIGHBOR_H__

#include "HENIBase.h"
#include "HENILinkAddress.h"

/**
 * @file
 * HENI: Basic functionality of a HENI node's neighbor.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


/**
 * A node's neighbor in HENI.
 */
struct heni_neighbor_s;
typedef struct heni_neighbor_s   heni_neighbor_t;


/**
 * Returns a constant pointer to the link-layer
 * address of a HENI neighbor.
 * @param nbr The neighbor.
 * @return A pointer to the neighbor's link-layer
 *   address. The contents remain valid as long
 *   as the neighbor remains valid.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t const * heniNeighborEntryGetLinkAddrConstPtr(
        heni_neighbor_t const * nbr
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Allocates an entry for a HENI neighbor table.
 * @param ker The HENI kernel for which
 *   the entry is to be allocated.
 * @return A pointer to the allocated neighbor or
 *   NULL if there is no memory to allocate the neighbor.
 */
HENI_EXT_FUNCT_DEC_PREFIX heni_neighbor_t * heniNeighborEntryAlloc(
        heni_kernel_t * ker
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Frees an entry for a HENI neighbor table.
 * @param ker The HENI kernel for which
 *   the entry is to be freed.
 * @param nbr The neighbor entry to be freed.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniNeighborEntryFree(
        heni_kernel_t * ker,
        heni_neighbor_t * nbr
) HENI_EXT_FUNCT_DEC_SUFFIX;


#include "detail/HENINeighborDetail.h"

#endif /* __HENI_NEIGHBOR_H__ */
