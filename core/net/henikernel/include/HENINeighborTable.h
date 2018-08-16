/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_NEIGHBOR_TABLE_H__
#define __HENI_NEIGHBOR_TABLE_H__

#include "HENIBase.h"
#include "HENILinkAddress.h"
#include "HENINeighbor.h"

/**
 * @file
 * HENI: Low-level functionality (mechanisms)
 * for managing a node's neighbors.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/**
 * A neighbor table in HENI.
 */
struct heni_neighbor_table_s;
typedef struct heni_neighbor_table_s   heni_neighbor_table_t;

/**
 * An element of a neighbor table in HENI.
 */
struct heni_neighbor_table_elem_s;
typedef struct heni_neighbor_table_elem_s   heni_neighbor_table_elem_t;

/**
 * A transient iterator over a HENI neighbor table
 * that can be used only if the table is not
 * modified externally (i.e., no entries are added to
 * or removed from the table).
 */
struct heni_neighbor_table_titer_s;
typedef struct heni_neighbor_table_titer_s    heni_neighbor_table_titer_t;

/**
 * A persistent iterator over a HENI neighbor table
 * that can be used even if the table is modified
 * externally (but not if it is reinitialized).
 */
struct heni_neighbor_table_piter_s;
typedef struct heni_neighbor_table_piter_s    heni_neighbor_table_piter_t;

/**
 * Initializes a HENI neighbor table.
 * @param ker The HENI kernel for which the
 *   table is initialized.
 * @param nbt The neighbor table.
 * @param bufPtr A pointer to the memory buffer
 *   that will hold the table.
 * @param bufLen The length of the memory buffer.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableInit(
        heni_neighbor_table_t * nbt,
        heni_kernel_t * ker,
        heni_neighbor_table_elem_t * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Inserts a neighbor entry with a given link-layer
 * address to a HENI neighbor table. An entry with
 * the same link-layer address must not exist in
 * the table; otherwise, a fatal error occurs.
 * @param nbt The neighbor table.
 * @param laddrPtr The link address.
 * @param nbr The neighbor entry.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableAddNonexisting(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr,
        heni_neighbor_t * nbr
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Searches for a neighbor entry with a given
 * link-layer address in a HENI neighbor table.
 * @param nbt The neighbor table.
 * @param laddrPtr The link address.
 * @return A pointer to the neighbor entry or
 *   NULL if no neighbor with the given
 *   address exists in the table.
 */
HENI_API_FUNCT_DEC_PREFIX heni_neighbor_t * heniNeighborTableFind(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Removes a neighbor entry from a HENI
 * neighbor table. An entry with
 * the link-layer address must exist in
 * the table; otherwise, a fatal error occurs.
 * @param nbt The neighbor table.
 * @param laddrPtr The link address.
 * @return A pointer to the removed neighbor
 *   entry or NULL if no neighbor with the given
 *   address exists in the table.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableRemoveExisting(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Cleans up a HENI neighbor table.
 * It is assumed that the table contains no entries;
 * otherwise, a fatal error is reported.
 * @param nbt The neighbor table.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableCleanup(
        heni_neighbor_table_t * nbt
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Initializes a transient iterator over a HENI
 * neighbor table.
 * @param nbtti The iterator.
 * @param nbt The neighbor table.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableTIterInit(
        heni_neighbor_table_titer_t * nbtti,
        heni_neighbor_table_t * nbt
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Copies a transient iterator over a HENI
 * neighbor table.
 * @param nbttiSrc The source iterator.
 * @param nbttiDst The destination iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableTIterCopy(
        heni_neighbor_table_titer_t const * nbttiSrc,
        heni_neighbor_table_titer_t * nbttiDst
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Converts a transient iterator over a HENI
 * neighbor table to a persistent one.
 * @param nbtti The transient iterator.
 * @param nbtpi The persistent iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableTIterToPIter(
        heni_neighbor_table_titer_t const * nbtti,
        heni_neighbor_table_piter_t * nbtpi
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Converts a persistent iterator over a HENI
 * neighbor table to a transient one.
 * @param nbtpi The persistent iterator.
 * @param nbtti The transient iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTablePIterToTIter(
        heni_neighbor_table_piter_t const * nbtpi,
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Checks if a transient iterator over a HENI
 * neighbor table is active, that is, whether
 * the iteration process has not finished.
 * @param nbtti The iterator.
 * @return Nonzero if the iteration process has
 *   not finished or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniNeighborTableTIterIsActive(
        heni_neighbor_table_titer_t const * nbtti
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a neighbor pointed by a transient
 * iterator over a HENI neighbor table.
 * The iteration process must be active.
 * @param nbtti The iterator.
 * @return The neighbor pointed at by the iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_neighbor_t * heniNeighborTableTIterGetNeighbor(
        heni_neighbor_table_titer_t const * nbtti
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Finishes iteration with a transient iterator
 * over a HENI neighbor table.
 * @param nbtti The iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniNeighborTableTIterFinish(
        heni_neighbor_table_titer_t * nbtti
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Advances a transient iterator over a HENI neighbor
 * table to the next entry, if it exists. The iteration
 * process must not be finished.
 * @param nbtti The iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableTIterAdvance(
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Removes the element pointed by a transient iterator
 * over a HENI neighbor table and advances the iterator
 * to the next entry, if it exists. The iteration
 * process must not be finished.
 * @param nbtti The iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniNeighborTableTIterRemoveAndAdvance(
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEC_SUFFIX;



#include "detail/HENINeighborTableDetail.h"

#endif /* __HENI_NEIGHBOR_TABLE_H__ */
