#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
#include "heni-wrapper.h"
#include "net/netstack.h"
#include "net/mac/mac.h"
#include "lib/memb.h"
#include "lib/list.h"
#include "pt.h"
#include <stdio.h>
#include <stdint.h>


MEMB(InstanceAllocator, heni_instance_t, HENI_PORT_DEFAULT_MAX_INSTANCES);
MEMB(PacketAllocator, heni_packet_t, HENI_PORT_DEFAULT_MAX_PACKETS);
MEMB(NeighborAllocator, heni_neighbor_t, HENI_PORT_DEFAULT_MAX_NEIGHBORS);
MEMB(ZoneAllocator, heni_zone_t, HENI_PORT_DEFAULT_MAX_ZONES);
MEMB(ReceivedMessageAllocator, message_info_rx_t, HENI_PORT_DEFAULT_MAX_PACKETS);

static struct pt computation_thread_pt, buffer_thread_pt;

heni_kernel_t m_kernel;
sent_callback sent_callback_f = 0;
received_callback received_callback_f = 0;

inline message_info_rx_t * iovToRxMsg(heni_iobuf_list_t * iov)
{
    return (message_info_rx_t *)((uint8_t *)iov - offsetof(message_info_rx_t, iovList));
}

void heniSetCallbacks(sent_callback _sent_callback, received_callback _received_callback) {
  sent_callback_f = _sent_callback;
  received_callback_f = _received_callback;
}


PT_THREAD(computation_thread(struct pt *pt))
{
    PT_BEGIN(pt);
    heniKernelResumeComputations(&m_kernel);
    PT_END(pt);
}

PT_THREAD(buffer_thread(struct pt *pt))
{
    PT_BEGIN(pt);
    if (PT_SCHEDULE(computation_thread(&computation_thread_pt))) {
      PRINTF("I don't know what to do with this information\n");
    }
    PT_END(pt);
}


void heniKernelPostponeComputations(
        heni_kernel_t * ker
)
{
    HENI_DASSERT(ker == &m_kernel);
    if (PT_SCHEDULE(buffer_thread(&buffer_thread_pt))) {
      PRINTF("I don't know what to do with this information\n");
    }
}

void heniKernelInstanceStopDone(
        heni_kernel_t * ker,
        heni_instance_id_t iid
)
{
    HENI_DASSERT(ker == &m_kernel);
}

void heniKernelInstanceStopAllDone(
        heni_kernel_t * ker
)
{
    HENI_DASSERT(ker == &m_kernel);
    // Ignore.
}


static void
packet_sent(void *ptr, int status, int num_tx)
{
  heni_frame_tx_info_t finfo;

  switch(status) {
  case MAC_TX_COLLISION:
    PRINTF("[HeniKernelWrapper] collision after %d tx\n", num_tx);
    break;
  case MAC_TX_NOACK:
    PRINTF("[HeniKernelWrapper] noack after %d tx\n", num_tx);
    break;
  case MAC_TX_OK:
    PRINTF("[HeniKernelWrapper] sent after %d tx\n", num_tx);
    break;
  default:
    PRINTF("[HeniKernelWrapper] error %d after %d tx\n", status, num_tx);
  }
  heniFrameTxInfoReset(&finfo);
  packetbuf_clear();
  heniKernelFrameSendFinish(&m_kernel, (heni_iobuf_list_t *)ptr, &finfo);
}

void heniPacketSendFinish(
        heni_kernel_t * ker,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld,
        heni_packet_tx_info_t const * psts
)
{
    HENI_DASSERT(ker == &m_kernel);
    HENI_DASSERT(sent_callback_f);
    sent_callback_f(paddr, ppld, psts);

}


int_fast8_t heniKernelFrameSendStart(
        heni_kernel_t * ker,
        heni_frame_addr_t const * faddr,
        heni_iobuf_list_t * fpld
)
{
    heni_iobuf_list_fiter_t   payloadIter;
    uint8_t *                 payloadPtr;
    uint16_t                  payloadLen;
    linkaddr_t                address;

    payloadLen = heniIOBufListGetCapacity(fpld);

    if (payloadLen > PACKETBUF_SIZE) {
      PRINTF("Payload too big\n");
      return -1;
    }

    packetbuf_set_datalen(payloadLen);
    payloadPtr = packetbuf_dataptr();

    heniIOBufListFIterInit(
        &payloadIter,
        fpld
    );
    heniIOBufListFIterTryCopyIntoOneBufAndAdvance(
        &payloadIter,
        payloadPtr,
        payloadLen
    );

    heniLinkAddrCopy(heniFrameAddrGetSrcLinkAddrConstPtr(faddr), (uint8_t *)&address);
    packetbuf_set_addr(PACKETBUF_ADDR_SENDER, &address);

    heniLinkAddrCopy(heniFrameAddrGetDstLinkAddrConstPtr(faddr), (uint8_t *)&address);
    packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, &address);

    NETSTACK_LLSEC.send(packet_sent, fpld);

    return (int_fast8_t)0;
}

int_fast8_t heniPacketReceiveStart(
        heni_kernel_t * ker,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld,
        heni_packet_rx_info_t const * psts
)
{
    int status;
    HENI_DASSERT(ker == &m_kernel);
    HENI_DASSERT(received_callback_f);
    status = received_callback_f(paddr, ppld, psts);
    return (int_fast8_t)status;
}


void heniReceiveFinish(
        heni_iobuf_list_t * fpld
)
{
    heniPacketReceiveFinish(&m_kernel, fpld);
}

void heniKernelFrameReceiveFinish(
        heni_kernel_t * ker,
        heni_iobuf_list_t * fpld
)
{
    HENI_DASSERT(ker == &m_kernel);
    message_info_rx_t *msg = iovToRxMsg(fpld);
    memb_free(&ReceivedMessageAllocator, msg);
}

int heniStartSending(heni_instance_id_t iid,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld)
{
    int_fast8_t   res;

    HENI_DASSERT(heniPacketAddrGetInstanceID(paddr) == iid);

    res = heniPacketSendStart(&m_kernel, paddr, ppld);
    if (res < 0)
    {
      PRINTF("heniPacketSendStart failed\n");
      return 1;
    }
    return 0;
}

static void
input(void)
{
  int original_datalen;
  uint8_t *original_dataptr;
  heni_frame_rx_info_t  finfo;
  heni_frame_addr_t     faddr;
  message_info_rx_t *msg = memb_alloc(&ReceivedMessageAllocator);

  if (msg == 0) {
    PRINTF("ReceivedMessageAllocator failed to allocate a message\n");
    return;
  }

  heniFrameAddrReset(&faddr);
  heniLinkAddrCopy(
          (const uint8_t *) packetbuf_addr(PACKETBUF_ADDR_SENDER),
          heniFrameAddrGetSrcLinkAddrPtr(&faddr)
  );

  // when Contiki receives a broadcast message
  // the destination address isn't a broadcast address,
  // but a null adress
  if (packetbuf_holds_broadcast()) {
    heniLinkAddrFetchAllNeighbors(heniFrameAddrGetDstLinkAddrPtr(&faddr));
  } else {
    heniLinkAddrCopy(
          (const uint8_t *) packetbuf_addr(PACKETBUF_ADDR_RECEIVER),
          heniFrameAddrGetDstLinkAddrPtr(&faddr)
    );
  }

  original_datalen = packetbuf_datalen();
  original_dataptr = packetbuf_dataptr();
  memcpy(msg->data, original_dataptr, original_datalen);
  heniIOBufNodeInitMem(&msg->iovNode, msg->data, original_datalen);
  heniIOBufListInit(&msg->iovList);
  heniIOBufNodeAddBack(&msg->iovList, &msg->iovNode);
  heniKernelFrameReceiveStart(&m_kernel, &faddr, &msg->iovList, &finfo);
}
/*---------------------------------------------------------------------------*/
static void
init(void)
{
  queuebuf_init();
  packetbuf_clear();
  PT_INIT(&computation_thread_pt);
  PT_INIT(&buffer_thread_pt);
  memb_init(&InstanceAllocator);
  memb_init(&PacketAllocator);
  memb_init(&NeighborAllocator);
  memb_init(&ZoneAllocator);
  memb_init(&ReceivedMessageAllocator);
  heniKernelInit(&m_kernel);
  heniKernelInstanceStart(&m_kernel, 1);
}


heni_instance_t * heniKernelInstanceAlloc(
        heni_kernel_t * ker,
        heni_instance_id_t iid
)
{
    HENI_DASSERT(ker == &m_kernel);
    return memb_alloc(&InstanceAllocator);
}



void heniKernelInstanceFree(
        heni_kernel_t * ker,
        heni_instance_id_t iid,
        heni_instance_t * inst
)
{
    HENI_DASSERT(ker == &m_kernel);
    memb_free(&InstanceAllocator, inst);
}



heni_packet_t * heniPacketAlloc(
        heni_kernel_t * ker,
        heni_instance_id_t iid
)
{
    HENI_DASSERT(ker == &m_kernel);
    return memb_alloc(&PacketAllocator);
}



void heniPacketFree(
        heni_kernel_t * ker,
        heni_instance_id_t iid,
        heni_packet_t * packet
)
{
    HENI_DASSERT(ker == &m_kernel);
    memb_free(&PacketAllocator, packet);
}



heni_zone_t * heniZoneEntryAlloc(
        heni_kernel_t * ker,
        heni_instance_id_t iid
)
{
    HENI_DASSERT(ker == &m_kernel);
    return memb_alloc(&ZoneAllocator);
}

void heniZoneEntryFree(
        heni_kernel_t * ker,
        heni_instance_id_t iid,
        heni_zone_t * zone
)
{
    HENI_DASSERT(ker == &m_kernel);
    memb_free(&ZoneAllocator, zone);
}

heni_neighbor_t * heniNeighborEntryAlloc(
        heni_kernel_t * ker
)
{
    HENI_DASSERT(ker == &m_kernel);
    return memb_alloc(&NeighborAllocator);
}

void heniNeighborEntryFree(
        heni_kernel_t * ker,
        heni_neighbor_t * nbr
)
{
    HENI_DASSERT(ker == &m_kernel);
    memb_free(&NeighborAllocator, nbr);
}


/*---------------------------------------------------------------------------*/
const struct network_driver heni_driver = {
  "HENIKernel",
  init,
  input
};
/** @} */
