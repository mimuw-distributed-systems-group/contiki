/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_LABEL_H__
#define __HENI_LABEL_H__

#include "HENIBase.h"
#include "HENIDebug.h"


/**
 * @file
 * HENI: Functions for handling labels.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/**
 * Checks if a given HENI label specification is
 * valid.
 * @param labSpec The specification for labels.
 * @return Nonzero if the specification is valid
 *   or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniLabelSpecIsValid(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Compares two HENI label specifications.
 * @param labSpec1 The first specification.
 * @param labSpec2 The second specification.
 * @return 0 iff the specifications are equal;
 *   <0 iff the first specification precedes
 *   the second one; >0 iff the first specification
 *   succeeds the second one.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniLabelSpecCmp(
        heni_lspec_t labSpec1,
        heni_lspec_t labSpec2
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the number of zone identifiers in
 * a HENI label described by a given specification.
 * @param labSpec The specification for labels.
 * @return The maximal number of zone identifiers
 *   in a label satisfying the specification.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_level_t heniLabelSpecGetNumLevels(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the width (in bits) of zone identifiers in
 * a HENI label described by a given specification.
 * @param labSpec The specification for labels.
 * @return The width in bits of zone identifiers
 *   in a label satisfying the specification.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t heniLabelSpecGetZoneLIDWidth(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * For a given HENI label specification, returns
 * a zone identifier that represents an invalid zone.
 * @param labSpec The specification for labels.
 * @return The identifier representing
 *   an invalid zone for the specification.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDInvalid(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * For a given HENI label specification, returns
 * a zone identifier that represents a leader zone.
 * @param labSpec The specification for labels.
 * @return The identifier representing
 *   a leader zone for the specification.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDLeader(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * For a given HENI label specification, returns
 * a zone identifier that represents any zone.
 * @param labSpec The specification for labels.
 * @return The identifier representing
 *   any zone for the specification.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDAny(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * For a given HENI label specification, returns
 * a zone identifier that represents all zones.
 * @param labSpec The specification for labels.
 * @return The identifier representing
 *   all zones for the specification.
 */
HENI_API_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDAll(
        heni_lspec_t labSpec
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * For a given HENI label specification, returns
 * a minimal assignable (i.e., not special)
 * zone identifier.
 * @param labSpec The specification for labels.
 * @return The minimal assignable zone identifier.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDMinAssignable(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * For a given HENI label specification, returns
 * a maximal assignable (i.e., not special)
 * zone identifier.
 * @param labSpec The specification for labels.
 * @return The maximal assignable zone identifier.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDMaxAssignable(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * For a given HENI label specification, checks
 * if a given zone identifier can actually be assigned
 * to some zone or whether it is some special identifier.
 * @param labSpec The specification for labels.
 * @param lid The zone identifier to check.
 * @return Nonzero if the identifier can be assigned
 *    to a zone or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniLabelSpecIsZoneLIDAssignable(
        heni_zone_lid_t lid,
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the i-th local zone identifier in
 * a HENI label with a given specification.
 * @param labPtr A pointer to the label.
 * @param labSpec The specification for the label.
 * @param i The index of the element to return.
 * @return The i-th local zone identifier in
 *   a HENI label.
 */
HENI_API_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelGetElement(
        uint8_t const * labPtr,
        heni_lspec_t labSpec,
        heni_level_t i
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Sets the i-th local zone identifier in
 * a HENI label with a given specification.
 * @param labPtr A pointer to the label.
 * @param labSpec The specification for the label.
 * @param i The index of the element to return.
 * @param lid The zone identifier to set.
 */
HENI_API_FUNCT_DEC_PREFIX void heniLabelSetElement(
        uint8_t * labPtr,
        heni_lspec_t labSpec,
        heni_level_t i,
        heni_zone_lid_t lid
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Lexicographically compares two HENI labels.
 * @param labPtr1 A pointer to the first label.
 * @param labPtr2 A pointer to the second label.
 * @param labSpec1 The specification for the first label.
 * @param labSpec2 The specification for the second label.
 * @return 0 iff the labels are equal; <0 iff lab1
 *   lexicographically precedes lab2; >0 iff lab2
 *   lexicographically succeeds lab2.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniLabelCmp(
        uint8_t const * labPtr1,
        uint8_t const * labPtr2,
        heni_lspec_t labSpec1,
        heni_lspec_t labSpec2
) HENI_API_FUNCT_DEC_SUFFIX;


#ifdef HENI_DEBUG
#define HENI_DLOGC_LABEL(l, s) heniDebugContinueLabel(l, s)
/**
 * Sends a label to a debug channel.
 * @param labPtr A pointer to the label.
 * @param labSpec The specification for the label.
 */
HENI_API_FUNCT_DEC_PREFIX void heniDebugContinueLabel(
        uint8_t const * labPtr,
        heni_lspec_t labSpec
) HENI_API_FUNCT_DEC_SUFFIX;
#else
#define HENI_DLOGC_LABEL(l, s)
#endif /* HENI_DEBUG */


/**
 * Given a configuration for a HENI zone discriminator,
 * returns an invalid discriminator.
 * @param numDiscrBits The number of bits in a zone discriminator.
 * @return The requested HENI zone discriminator.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrInvalid(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Given a configuration for a HENI zone discriminator,
 * returns a discriminator matching all discriminators.
 * @param numDiscrBits The number of bits in a zone discriminator.
 * @return The requested HENI zone discriminator.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrAny(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Given a configuration for a HENI zone discriminator,
 * returns the minimal assignable (i.e., not special) discriminator.
 * @param numDiscrBits The number of bits in a zone discriminator.
 * @return The minimal assignable HENI zone discriminator.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrMinAssignable(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Given a configuration for a HENI zone discriminator,
 * returns the maximal assignable (i.e., not special) discriminator.
 * @param numDiscrBits The number of bits in a zone discriminator.
 * @return The maximal assignable HENI zone discriminator.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrMaxAssignable(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * For a given configuration of a HENI zone discriminator,
 * checks if a given zone discriminator can actually be assigned
 * to some zone or whether it is some special discriminator.
 * @param discr The zone discriminator to check.
 * @param numDiscrBits The number of bits in a zone discriminator.
 * @return Nonzero if the discriminator can be assigned
 *    to a zone or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniZoneDiscrSpecIsZoneDiscrAssignable(
        heni_zone_discr_t discr,
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEC_SUFFIX;




#include "detail/HENILabelDetail.h"

#endif /* __HENI_LABEL_H__ */

