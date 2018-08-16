#ifndef HENI_WRAPPER_H_
#define HENI_WRAPPER_H_

#include "net/queuebuf.h"
#include "net/linkaddr.h"
#include "net/packetbuf.h"

#include "net/mac/mac.h"

#include "HENIKernel.h"
#include "HENINeighbor.h"
#include "HENIFrame.h"
#include "HENIZone.h"

extern const struct network_driver heni_driver;

#define HENI_PORT_DEFAULT_MAX_INSTANCES 1
#define HENI_PORT_DEFAULT_MAX_PACKETS 3
#define HENI_PORT_DEFAULT_MAX_NEIGHBORS 10
#define HENI_PORT_DEFAULT_MAX_ZONES 10
#define MAX_MESSAGE_SIZE 256
typedef void (*sent_callback)(heni_packet_addr_t const *, heni_iobuf_list_t *, heni_packet_tx_info_t const *);
typedef int (*received_callback)(heni_packet_addr_t const *, heni_iobuf_list_t *, heni_packet_rx_info_t const *);

int heniStartSending(heni_instance_id_t iid,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld);

void heniSetCallbacks(sent_callback _sent_callback, received_callback _received_callback);

void heniReceiveFinish(heni_iobuf_list_t * fpld);

typedef struct message_info_rx
{
    heni_iobuf_list_t         iovList;
    heni_iobuf_list_node_t    iovNode;
    char *data[MAX_MESSAGE_SIZE];
} message_info_rx_t;

#endif /* HENI_WRAPPER_H_ */

/** @} */
/** @} */
