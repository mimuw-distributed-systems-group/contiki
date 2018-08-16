/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_LABEL_DETAIL_H__
#define __HENI_LABEL_DETAIL_H__


#ifndef __HENI_LABEL_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_LABEL_H__ */

#include "HENIError.h"



enum
{
	HENI_LABEL_SPEC_VALUE_INVALID = 0x0,
	HENI_LABEL_SPEC_VALUE_MINIMAL = 0x1,
	HENI_LABEL_SPEC_VALUE_MAXIMAL = 0xf,
    HENI_LABEL_SPEC_NUM_VALUES = HENI_LABEL_SPEC_VALUE_MAXIMAL + 1,
};


HENI_PRV_DATA_DEC_PREFIX heni_level_t const __const_heniLabelSpecNumLevels[HENI_LABEL_SPEC_NUM_VALUES];
HENI_PRV_DATA_DEC_PREFIX uint8_t const __const_heniLabelSpecZoneLIDWidths[HENI_LABEL_SPEC_NUM_VALUES];



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniLabelSpecIsValid(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return labSpec >= HENI_LABEL_SPEC_VALUE_MINIMAL &&
            labSpec <= HENI_LABEL_SPEC_VALUE_MAXIMAL ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniLabelSpecCmp(
        heni_lspec_t labSpec1,
        heni_lspec_t labSpec2
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return labSpec1 == labSpec2 ?
            0 : (labSpec1 < labSpec2 ? (int_fast8_t)-1 : (int_fast8_t)1);
}



HENI_INL_FUNCT_DEF_PREFIX heni_level_t heniLabelSpecGetNumLevels(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniLabelSpecIsValid(labSpec));
    return __const_heniLabelSpecNumLevels[labSpec];
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t heniLabelSpecGetZoneLIDWidth(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniLabelSpecIsValid(labSpec));
    return __const_heniLabelSpecZoneLIDWidths[labSpec];
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDInvalid(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (heni_zone_lid_t)0;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDLeader(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniLabelSpecGetZoneLIDInvalid(labSpec) + 1;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDAny(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniLabelSpecGetZoneLIDAll(labSpec) - 1;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDMinAssignable(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniLabelSpecGetZoneLIDLeader(labSpec) + 1;
}



HENI_INL_FUNCT_DEC_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDMaxAssignable(
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEC_SUFFIX
{
    return heniLabelSpecGetZoneLIDAny(labSpec) - 1;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniLabelSpecIsZoneLIDAssignable(
        heni_zone_lid_t lid,
        heni_lspec_t labSpec
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return lid >= heniLabelSpecGetZoneLIDMinAssignable(labSpec) &&
            lid <= heniLabelSpecGetZoneLIDMaxAssignable(labSpec) ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrInvalid(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (heni_zone_discr_t)0;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrAny(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return numDiscrBits == 0 ? (heni_zone_discr_t)0 : (((heni_zone_discr_t)1 << numDiscrBits) - 1);
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrMinAssignable(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(numDiscrBits > 1);
    return heniZoneDiscrSpecGetZoneDiscrInvalid(numDiscrBits) + 1;
}



HENI_INL_FUNCT_DEF_PREFIX heni_zone_discr_t heniZoneDiscrSpecGetZoneDiscrMaxAssignable(
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(numDiscrBits > 1);
    return heniZoneDiscrSpecGetZoneDiscrAny(numDiscrBits) - 1;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniZoneDiscrSpecIsZoneDiscrAssignable(
        heni_zone_discr_t discr,
        uint8_t numDiscrBits
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return discr > heniZoneDiscrSpecGetZoneDiscrInvalid(numDiscrBits) &&
            discr < heniZoneDiscrSpecGetZoneDiscrAny(numDiscrBits);
}


#endif /* __HENI_LABEL_DETAIL_H__ */
