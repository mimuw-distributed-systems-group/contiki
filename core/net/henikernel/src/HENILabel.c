/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIError.h"
#include "HENILabel.h"



HENI_PRV_DATA_DEF_PREFIX heni_level_t const __const_heniLabelSpecNumLevels[HENI_LABEL_SPEC_NUM_VALUES] =
        {0,  1,  2,  1,  4,  2,  1,  8,  4,  2,  1, 16,  8,  4,  2,  1};
HENI_PRV_DATA_DEF_PREFIX uint8_t const __const_heniLabelSpecZoneLIDWidths[HENI_LABEL_SPEC_NUM_VALUES] =
        {0,  4,  4,  8,  4,  8, 16,  4,  8, 16, 32,  4,  8, 16, 32, 64};



HENI_API_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDAll(
        heni_lspec_t labSpec
) HENI_API_FUNCT_DEF_SUFFIX
{
    switch (heniLabelSpecGetZoneLIDWidth(labSpec))
    {
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)
    case 4:
        return (heni_zone_lid_t)0x0fU;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)
    case 8:
        return (heni_zone_lid_t)UINT8_MAX;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)
    case 16:
        return (heni_zone_lid_t)UINT16_MAX;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)
    case 32:
        return (heni_zone_lid_t)UINT32_MAX;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)
    case 64:
        return (heni_zone_lid_t)UINT64_MAX;
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */
    default:
        HENI_PASSERT(0);
        return 0/*whatever*/;
    }
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)

HENI_INL_FUNCT_DEF_PREFIX uint8_t heniLabelGetElement4(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t   byteIdx = (i >> 1);
    uint8_t   noMaskShift = (i & 1);
    uint8_t   byteVal = labPtr[byteIdx];
    return noMaskShift ? (byteVal & 0x0f) : (byteVal >> 4);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement4(
        uint8_t * labPtr,
        heni_level_t i,
        uint8_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t   byteIdx = (i >> 1);
    uint8_t   noMaskShift = (i & 1);
    uint8_t   byteVal = labPtr[byteIdx];
    if (noMaskShift)
    {
       byteVal &= 0xf0;
       byteVal |= ((uint8_t)elem & 0x0f);
    }
    else
    {
       byteVal &= 0x0f;
       byteVal |= ((uint8_t)elem << 4);
    }
    labPtr[byteIdx] = byteVal;
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)

HENI_INL_FUNCT_DEF_PREFIX uint8_t heniLabelGetElement8(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return labPtr[i];
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement8(
        uint8_t * labPtr,
        heni_level_t i,
        uint8_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    labPtr[i] = elem;
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)

HENI_INL_FUNCT_DEF_PREFIX uint16_t heniLabelGetElement16(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 1);
    return ((((uint16_t)labPtr[byteIdx + 0]) << 8) | (labPtr[byteIdx + 1]));
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement16(
        uint8_t * labPtr,
        heni_level_t i,
        uint16_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 1);
    labPtr[byteIdx + 0] = (uint8_t)(elem >> 8);
    labPtr[byteIdx + 1] = (uint8_t)(elem);
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)

HENI_INL_FUNCT_DEF_PREFIX uint32_t heniLabelGetElement32(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 2);
    return ((((uint32_t)labPtr[byteIdx + 0]) << 24) |
            (((uint32_t)labPtr[byteIdx + 1]) << 16) |
            (((uint32_t)labPtr[byteIdx + 2]) << 8) |
            (labPtr[byteIdx + 3]));
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement32(
        uint8_t * labPtr,
        heni_level_t i,
        uint32_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 2);
    labPtr[byteIdx + 0] = (uint8_t)(elem >> 24);
    labPtr[byteIdx + 1] = (uint8_t)(elem >> 16);
    labPtr[byteIdx + 2] = (uint8_t)(elem >> 8);
    labPtr[byteIdx + 3] = (uint8_t)(elem);
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)

HENI_INL_FUNCT_DEF_PREFIX uint64_t heniLabelGetElement64(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 3);
    return ((((uint64_t)labPtr[byteIdx + 0]) << 56) |
            (((uint64_t)labPtr[byteIdx + 1]) << 48) |
            (((uint64_t)labPtr[byteIdx + 2]) << 40) |
            (((uint64_t)labPtr[byteIdx + 3]) << 32) |
            (((uint64_t)labPtr[byteIdx + 4]) << 24) |
            (((uint64_t)labPtr[byteIdx + 5]) << 16) |
            (((uint64_t)labPtr[byteIdx + 6]) << 8) |
            (labPtr[byteIdx + 7]));
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement64(
        uint8_t * labPtr,
        heni_level_t i,
        uint64_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 3);
    labPtr[byteIdx + 0] = (uint8_t)(elem >> 56);
    labPtr[byteIdx + 1] = (uint8_t)(elem >> 48);
    labPtr[byteIdx + 2] = (uint8_t)(elem >> 40);
    labPtr[byteIdx + 3] = (uint8_t)(elem >> 32);
    labPtr[byteIdx + 4] = (uint8_t)(elem >> 24);
    labPtr[byteIdx + 5] = (uint8_t)(elem >> 16);
    labPtr[byteIdx + 6] = (uint8_t)(elem >> 8);
    labPtr[byteIdx + 7] = (uint8_t)(elem);
}

#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */



HENI_API_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelGetElement(
        uint8_t const * labPtr,
        heni_lspec_t labSpec,
        heni_level_t i
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (i >= heniLabelSpecGetNumLevels(labSpec))
    {
        goto RETURN_INVALID_ELEMENT;
    }
    switch (heniLabelSpecGetZoneLIDWidth(labSpec))
    {
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)
    case 4:
        return heniLabelGetElement4(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)
    case 8:
        return heniLabelGetElement8(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)
    case 16:
        return heniLabelGetElement16(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)
    case 32:
        return heniLabelGetElement32(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)
    case 64:
        return heniLabelGetElement64(labPtr, i);
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */
    default:
        HENI_PASSERT(0);
        return 0/*whatever*/;
    }
RETURN_INVALID_ELEMENT:
    return 0;
}



HENI_API_FUNCT_DEF_PREFIX void heniLabelSetElement(
        uint8_t * labPtr,
        heni_lspec_t labSpec,
        heni_level_t i,
        heni_zone_lid_t lid
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (i >= heniLabelSpecGetNumLevels(labSpec))
    {
        goto SET_ELEMENT_AT_INVALID_INDEX;
    }
    switch (heniLabelSpecGetZoneLIDWidth(labSpec))
    {
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)
    case 4:
        heniLabelSetElement4(labPtr, i, (uint8_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)
    case 8:
        heniLabelSetElement8(labPtr, i, (uint8_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)
    case 16:
        heniLabelSetElement16(labPtr, i, (uint16_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)
    case 32:
        heniLabelSetElement32(labPtr, i, (uint32_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)
    case 64:
        heniLabelSetElement64(labPtr, i, (uint64_t)lid);
        break;
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */
    default:
        HENI_PASSERT(0);
    }
    return;
SET_ELEMENT_AT_INVALID_INDEX:
    HENI_PASSERTM(0, "An invalid label element index, " PRIu8 "!", i);
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniLabelCmp(
        uint8_t const * labPtr1,
        uint8_t const * labPtr2,
        heni_lspec_t labSpec1,
        heni_lspec_t labSpec2
) HENI_API_FUNCT_DEF_SUFFIX
{
    int_fast8_t    res;
    heni_level_t   i, n;
    
    res = heniLabelSpecCmp(labSpec1, labSpec2);
    if (res != 0)
    {
        return res;
    }
    for (i = 0, n = heniLabelSpecGetNumLevels(labSpec1); i < n; ++i)
    {
        heni_zone_lid_t   lid1 = heniLabelGetElement(labPtr1, labSpec1, i);
        heni_zone_lid_t   lid2 = heniLabelGetElement(labPtr2, labSpec2, i);
        if (lid1 != lid2)
        {
            return lid1 < lid2 ? (int_fast8_t)-1 : (int_fast8_t)1;
        }
    }
    return 0;
}



#ifdef HENI_DEBUG

HENI_API_FUNCT_DEF_PREFIX void heniDebugContinueLabel(
        uint8_t const * labPtr,
        heni_lspec_t labSpec
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_level_t   i, n;

    HENI_DLOGC("|");
    n = heniLabelSpecGetNumLevels(labSpec);
    if (n > 0)
    {
        heni_zone_lid_t   lid = heniLabelGetElement(labPtr, labSpec, 0);
        HENI_DLOGC(HENI_PRI_ZONE_LID, lid);
        for (i = 1; i < n; ++i)
        {
            lid = heniLabelGetElement(labPtr, labSpec, i);
            HENI_DLOGC("|" HENI_PRI_ZONE_LID, lid);
        }
    }
    HENI_DLOGC("|");
}

#endif /* HENI_DEBUG */

