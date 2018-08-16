#include "ieee-addr.h"
#include <stdio.h>
#include "string.h"
#include "linkaddr.h"

int_fast8_t heniLinkAddrCmp(
        uint8_t const * laddrPtr1,
        uint8_t const * laddrPtr2
)
{
    linkaddr_t const * addr1 = (linkaddr_t const *)laddrPtr1;
    linkaddr_t const * addr2 = (linkaddr_t const *)laddrPtr2;
    return linkaddr_cmp(addr1, addr2);
}

size_t heniLinkAddrHash(
        uint8_t const * laddrPtr
)
{
    // linkaddr_t const * addr = (linkaddr_t const *)laddrPtr;
    return (size_t)(*laddrPtr);
}

void heniLinkAddrCopy(
        uint8_t const * laddrSrcPtr,
        uint8_t * laddrDstPtr
)
{
    linkaddr_t const * srcAddr = (linkaddr_t const *)laddrSrcPtr;
    linkaddr_t * dstAddr = (linkaddr_t *)laddrDstPtr;
    linkaddr_copy(dstAddr, srcAddr);
}

void heniLinkAddrInvalidate(
        uint8_t * laddrPtr
)
{
    linkaddr_t * addr = (linkaddr_t *)laddrPtr;
    linkaddr_copy(addr, &linkaddr_null);
}

int_fast8_t heniLinkAddrIsValid(
        uint8_t const * laddrPtr
)
{
    linkaddr_t tmp;
    heniLinkAddrInvalidate((uint8_t *)&tmp);
    linkaddr_t const * addr = (linkaddr_t const *)laddrPtr;
    return heniLinkAddrCmp((uint8_t *)&tmp, (uint8_t *)addr) == 0 ? (int_fast8_t)1 : 0;
}

void heniLinkAddrFetchMine(
        uint8_t * laddrPtr
)
{
    heniLinkAddrCopy((const uint8_t *)&linkaddr_node_addr, laddrPtr);
}

int_fast8_t heniLinkAddrIsMine(
        uint8_t const * laddrPtr
)
{
    linkaddr_t const * addr = (linkaddr_t const *)laddrPtr;
    linkaddr_t myAddr = linkaddr_node_addr;
    return heniLinkAddrCmp((uint8_t *) addr, (uint8_t *) &myAddr);
}

void heniLinkAddrFetchAllNeighbors(
        uint8_t * laddrPtr
)
{
    linkaddr_t * addr = (linkaddr_t *)laddrPtr;
    memset(addr, 0xff, sizeof(linkaddr_t));
}

int_fast8_t heniLinkAddrIsAllNeighbors(
        uint8_t const * laddrPtr
)
{
    linkaddr_t tmp;
    heniLinkAddrFetchAllNeighbors((uint8_t *) &tmp);
    linkaddr_t const * addr = (linkaddr_t const *)laddrPtr;
    return heniLinkAddrCmp((uint8_t *) &tmp, (uint8_t *) addr);
}

int_fast8_t heniLinkAddrIsMulticast(
        uint8_t const * laddrPtr
)
{
    return heniLinkAddrIsAllNeighbors(laddrPtr);
}

