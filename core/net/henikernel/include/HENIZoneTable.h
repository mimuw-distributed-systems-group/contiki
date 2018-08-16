/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_ZONE_TABLE_H__
#define __HENI_ZONE_TABLE_H__

#include "HENIBase.h"
#include "HENILabel.h"
#include "HENIZone.h"

/**
 * @file
 * HENI: Low-level functionality of managing
 * HENI zones locally at a node.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/**
 * A zone table in HENI.
 */
struct heni_zone_table_s;
typedef struct heni_zone_table_s   heni_zone_table_t;

/**
 * A bucket in a HENI zone table.
 * It corresponds to a hierarchy level.
 */
struct heni_zone_table_bucket_s;
typedef struct heni_zone_table_bucket_s   heni_zone_table_bucket_t;

/**
 * A transient iterator over a HENI zone table.
 * It can be used only if the table is not modified
 * externally (i.e., no entries are added to or
 * removed from the table).
 */
struct heni_zone_table_titer_s;
typedef struct heni_zone_table_titer_s    heni_zone_table_titer_t;

/**
 * A persistent iterator over a HENI zone table.
 * It can be used even if the table is modified
 * externally (but not if it is reinitialized)..
 */
struct heni_zone_table_piter_s;
typedef struct heni_zone_table_piter_s    heni_zone_table_piter_t;



/**
 * Initializes a HENI zone table.
 * @param inst The HENI instance for which the
 *   table is initialized.
 * @param zt The zone table.
 * @param bucketPtrsPerRow A table of a length equal
 *   to the number of LIDs in a zone label in the
 *   given instance.  This table will
 *   be copied, so it must live only throughout
 *   the invocation of this function. Each element
 *   of the table is another table: of buckets with
 *   the length given as the next parameter. These
 *   tables will be taken over by the zone table.
 * @param bucketCountPerRow The length of each of
 *   the bucket tables.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableInit(
        heni_zone_table_t * zt,
        heni_instance_t * inst,
        heni_zone_table_bucket_t * const * bucketPtrsPerRow,
        size_t bucketCountPerRow
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Inserts a zone entry with a given level, LID,
 * and discriminator to a HENI neighbor table.
 * An entry with the same data must not exist in
 * the table; otherwise, a fatal error occurs.
 * @param zt The zone table.
 * @param level The level.
 * @param lid The LID.
 * @param discr The discriminator.
 * @param zone The zone entry.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableAddNonexisting(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr,
        heni_zone_t * zone
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Searches for a zone entry with a given
 * level, LID, and discriminator in a HENI
 * neighbor table.
 * @param zt The zone table.
 * @param level The level.
 * @param lid The LID.
 * @param discr The discriminator.
 * @return A pointer to the neighbor entry or
 *   NULL if no neighbor with the given
 *   address exists in the table.
 */
HENI_API_FUNCT_DEC_PREFIX heni_zone_t * heniZoneTableFind(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Removes a zone entry with a given level,
 * LID, and discriminator from a HENI
 * zone table. An entry with the given data
 * must exist in the table; otherwise, a fatal
 * error occurs.
 * @param zt The zone table.
 * @param level The level.
 * @param lid The LID.
 * @param discr The discriminator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableRemoveExisting(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_API_FUNCT_DEC_SUFFIX;


/**
 * Cleans up a HENI zone table.
 * It is assumed that the table contains no entries;
 * otherwise, a fatal error is reported.
 * @param zt The zone table.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableCleanup(
        heni_zone_table_t * zt
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Initializes a HENI zone table iterator to the
 * first entry at a given level of the zone table
 * if an entry exists in the row. If no entry
 * exists in the row, the iteration process
 * is finished.
 * @param iter The iterator to set.
 * @param zt The zone table.
 * @param zoneLevel The level in the table.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterSetAtLevel(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Initializes a HENI zone table iterator to the entry with
 * a given level, LID, and any discriminator. If no entry
 * exists with the given data, the iteration process is
 * finished.
 * @param iter The iterator to set.
 * @param zt The zone table.
 * @param zoneLevel The level of the row in the table.
 * @param zoneLID The LID.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterSetAtLevelAndLID(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Initializes a HENI zone table iterator to the entry with
 * a given level, LID, and discriminator. If no entry
 * exists with the given data, the iteration process is
 * finished.
 * @param iter The iterator to set.
 * @param zt The zone table.
 * @param zoneLevel The level of the row in the table.
 * @param zoneLID The LID.
 * @param zoneDiscr The discriminator or HENI_ZONE_DISCR_ANY.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterSetAtLevelAndLIDAndDiscr(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID,
        heni_zone_discr_t zoneDiscr
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Copies a transient iterator over a HENI
 * zone table.
 * @param iterSrc The source iterator.
 * @param iterDst The destination iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterCopy(
        heni_zone_table_titer_t const * iterSrc,
        heni_zone_table_titer_t * iterDst
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Checks if a transient iterator over a HENI
 * zone table is active, that is, whether
 * the iteration process has not finished.
 * @param iter The iterator.
 * @return Nonzero if the iteration process has
 *   not finished or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniZoneTableTIterIsActive(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns a zone pointed by a transient
 * iterator over a HENI zone table.
 * The iterator must be active.
 * @param iter The iterator.
 * @return The zone pointed at by the iterator.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_t * heniZoneTableTIterGetZone(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the current level of a transient
 * iterator over a HENI zone table.
 * The iterator must be active.
 * @param iter The iterator.
 * @return The level.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_level_t heniZoneTableTIterGetLevel(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the current LID of a zone
 * pointed by a transient iterator over
 * a HENI zone table.
 * The iterator must be active.
 * @param iter The iterator.
 * @return The LID.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniZoneTableTIterGetLID(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the current discriminator of a zone
 * pointed by a transient iterator over
 * a HENI zone table.
 * The iterator must be active.
 * @param iter The iterator.
 * @return The discriminator.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_discr_t heniZoneTableTIterGetDiscr(
        heni_zone_table_titer_t const * iter
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Finishes iteration with a transient iterator
 * over a HENI zone table.
 * @param iter The iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterFinish(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Advances a transient iterator over a HENI zone
 * table to the next entry at the same level, if it
 * exists. The iteration process must not be finished.
 * If no more entries exist at the level, the iteration
 * process is finished.
 * @param iter The iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterAdvancePreservingLevel(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Advances a transient iterator over a HENI zone
 * table to the next entry at the level, if it
 * exists and its LID is the same as the current
 * entry's LID. If the next entry exists but has
 * a different LID, the iteration process will
 * be finished. The iteration process must not be
 * finished.
 * @param iter The iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterAdvancePreservingLevelAndLID(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Removes a zone entry pointed by a transient iterator
 * over a HENI zone table and advances the iterator
 * to the next entry at the level, if it exists.
 * The iteration process must not be finished.
 * @param iter The iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterRemoveAndAdvancePreservingLevel(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Removes a zone entry pointed by a transient iterator
 * over a HENI zone table row and advances the iterator
 * to the next entry at the level, if it exists and has
 * the same LID; otherwise, the iteration process is
 * finished.
 * The iteration process must not be finished.
 * @param iter The iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Converts a transient iterator over a HENI
 * zone table to a persistent one.
 * @param titer The transient iterator.
 * @param piter The persistent iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTableTIterToPIter(
        heni_zone_table_titer_t const * titer,
        heni_zone_table_piter_t * piter
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Converts a persistent iterator over a HENI
 * neighbor table to a transient one, preserving
 * the level.
 * @param piter The persistent iterator.
 * @param titer The transient iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTablePIterToTIterPreservingLevel(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Converts a persistent iterator over a HENI
 * neighbor table to a transient one, preserving
 * the level and LID.
 * @param piter The persistent iterator.
 * @param titer The transient iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTablePIterToTIterPreservingLevelAndLID(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Converts a persistent iterator over a HENI
 * neighbor table to a transient one, preserving
 * the level, LID, and discriminator.
 * @param piter The persistent iterator.
 * @param titer The transient iterator.
 */
HENI_API_FUNCT_DEC_PREFIX void heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEC_SUFFIX;



#include "detail/HENIZoneTableDetail.h"

#endif /* __HENI_ZONE_TABLE_H__ */
