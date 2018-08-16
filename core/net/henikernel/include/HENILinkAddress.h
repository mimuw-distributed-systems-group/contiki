/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_LINK_ADDRESS_H__
#define __HENI_LINK_ADDRESS_H__

#include "HENIBase.h"

/**
 * @file
 * HENI: Functions for handling link-layer addresses.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/**
 * Lexicographically compares two link-layer
 * addresses passed to HENI.
 * @param laddrPtr1 A pointer to the first address.
 * @param laddrPtr2 A pointer to the second address.
 * @return 0 iff the addresses are equal; <0 iff laddr1
 *   lexicographically precedes laddr2; >0 iff laddr2
 *   lexicographically succeeds laddr2.
 */
HENI_EXT_FUNCT_DEC_PREFIX int_fast8_t heniLinkAddrCmp(
        uint8_t const * laddrPtr1,
        uint8_t const * laddrPtr2
) HENI_EXT_FUNCT_DEC_SUFFIX;

/*
 * Hashes a link-layer address passed to HENI to
 * an unsigned integer value.
 * @param laddrPtr A pointer to the address.
 * @return A hash value for the address.
 */
HENI_EXT_FUNCT_DEC_PREFIX size_t heniLinkAddrHash(
        uint8_t const * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Copies the source link-layer address passed
 * to HENI to a destination address.
 * @param laddrSrcPtr A pointer to the source address.
 * @param laddrDstPtr A pointer to the destination address.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniLinkAddrCopy(
        uint8_t const * laddrSrcPtr,
        uint8_t * laddrDstPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Invalides a given link-layer address passed to HENI.
 * @param laddrPtr A pointer to the address buffer.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniLinkAddrInvalidate(
        uint8_t * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Checks if a given link-layer address
 * passed to HENI is valid.
 * @param laddrPtr A pointer to the address.
 * @return Nonzero if the address is valid or
 *   zero otherwise.
 */
HENI_EXT_FUNCT_DEC_PREFIX int_fast8_t heniLinkAddrIsValid(
        uint8_t const * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Copies to the given buffer the HENI link-layer
 * address of the present node.
 * @param laddrPtr A pointer to the address buffer.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniLinkAddrFetchMine(
        uint8_t * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Checks if a given link-layer address
 * passed to HENI is the present's node address.
 * @param laddrPtr A pointer to the address.
 * @return Nonzero if the address is the node's
 *   own address or zero otherwise.
 */
HENI_EXT_FUNCT_DEC_PREFIX int_fast8_t heniLinkAddrIsMine(
        uint8_t const * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Copies to the given buffer the HENI link-layer
 * multicast address denoting all neighbors of the
 * present node that operate the HENI protocol.
 * @param laddrPtr A pointer to the address buffer.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniLinkAddrFetchAllNeighbors(
        uint8_t * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Checks if a given link-layer address
 * passed to HENI is a multicast address denoting
 * the present node's all neighbors running the
 * HENI protocol.
 * @param laddrPtr A pointer to the address.
 * @return Nonzero if the address is the checked
 *   multicast address or zero otherwise.
 */
HENI_EXT_FUNCT_DEC_PREFIX int_fast8_t heniLinkAddrIsAllNeighbors(
        uint8_t const * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Checks if a given link-layer address
 * passed to HENI is a multicast address.
 * @param laddrPtr A pointer to the address.
 * @return Nonzero if the address is the checked
 *   multicast address or zero otherwise.
 */
HENI_EXT_FUNCT_DEC_PREFIX int_fast8_t heniLinkAddrIsMulticast(
        uint8_t const * laddrPtr
) HENI_EXT_FUNCT_DEC_SUFFIX;


#endif /* __HENI_LINK_ADDRESS_H__ */

