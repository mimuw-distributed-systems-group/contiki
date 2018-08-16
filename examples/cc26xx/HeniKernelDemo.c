#include "contiki.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "random.h"
#include "button-sensor.h"
#include "batmon-sensor.h"
#include "board-peripherals.h"
#include "netstack.h"
#include "lib/memb.h"

#include "ti-lib.h"
#include "contiki-conf.h"
#include "rf-core/rf-core.h"

#include <stdio.h>
#include <stdint.h>
#include "sicslowmac.h"

#include "heni-wrapper.h"
#include "HENIKernel.h"
#include "HENINeighbor.h"
#include "HENIZone.h"

/*---------------------------------------------------------------------------*/
#define CC26XX_DEMO_LOOP_INTERVAL       (4 * CLOCK_SECOND * 2)
#define CC26XX_DEMO_LEDS_PERIODIC       LEDS_ALL
#define CC26XX_DEMO_LEDS_BUTTON         LEDS_RED
#define CC26XX_DEMO_LEDS_REBOOT         LEDS_ALL
/*---------------------------------------------------------------------------*/
#define CC26XX_DEMO_SENSOR_NONE         (void *)0xFFFFFFFF


/*---------------------------------------------------------------------------*/
static struct etimer et;
/*---------------------------------------------------------------------------*/
PROCESS(cc26xx_demo_process, "cc26xx demo process");
AUTOSTART_PROCESSES(&cc26xx_demo_process);

#define SENSOR_READING_PERIOD (CLOCK_SECOND * 20)
#define SENSOR_READING_RANDOM (CLOCK_SECOND << 4)

#define MAX_MESSAGES 1

void counterToLeds(uint16_t counter)
{
    if ((counter & 0x1) != 0)
    {
        leds_on(CC26XX_DEMO_LEDS_PERIODIC);
    }
    else
    {
        leds_off(CC26XX_DEMO_LEDS_PERIODIC);
    }
}

typedef struct radio_count_msg_s
{
    uint16_t counterValue;
    uint16_t nodeId;
} radio_count_msg_t;


typedef struct message_record_s
{
    heni_iobuf_list_t         iovList;
    heni_iobuf_list_node_t    iovNode;
    uint8_t                   iovPayload[sizeof(radio_count_msg_t)];
} message_record_t;

int m_counter = 0;
MEMB(MsgAlloc, message_record_t, MAX_MESSAGES);

inline heni_iobuf_list_t * msgToIov(message_record_t * msg)
{
    return &msg->iovList;
}

inline message_record_t * iovToMsg(heni_iobuf_list_t * iov)
{
    return (message_record_t *)((uint8_t *)iov - offsetof(message_record_t, iovList));
}

message_record_t *createMessage()
{
    message_record_t *    msg;
    radio_count_msg_t *   pld;

    msg = memb_alloc(&MsgAlloc);
    if (msg == NULL)
    {
        printf("[App] message was not allocated\n");
        return NULL;
    }

    pld = (radio_count_msg_t *)(&(msg->iovPayload[0]));
    pld->counterValue = m_counter;
    pld->nodeId = 0x1234;
    heniIOBufNodeInitMem(&msg->iovNode, pld, sizeof(radio_count_msg_t));
    heniIOBufListInit(&msg->iovList);
    heniIOBufNodeAddBack(&msg->iovList, &msg->iovNode);
    return msg;
}


void destroyMessage(message_record_t * msg)
{
    memb_free(&MsgAlloc, msg);
}

void createAndSendMessages(){
    heni_packet_addr_t   paddr;
    message_record_t *   msg;

    printf("[App] Timer fired. - %d\n", m_counter);
    msg = createMessage();
    printf("[App] message created\n");
    while (msg != NULL)
    {
        m_counter++;
        printf("[App] The counter is %d.\n", m_counter);
        heniPacketAddrReset(&paddr);
        heniPacketAddrSetHopLimit(&paddr, 1);
        heniPacketAddrSetInstanceID(&paddr, 1);

        printf("[App] startSending\n");
        if (heniStartSending(1, &paddr, msgToIov(msg)) != 0)
        {
            destroyMessage(msg);
            printf("[App] Failed to send a message with the counter equal to %d.\n", (int16_t)m_counter);
            return;
        }
        printf("[App] Successfully started sending a message with " \
            "the counter equal to %d.\n", (int16_t)m_counter);
        msg = createMessage();
    }
    printf("[App] exiting\n");
}

static void sent_callback_demo(heni_packet_addr_t const * paddr,
            heni_iobuf_list_t * ppld,
            heni_packet_tx_info_t const * psts) {
  message_record_t *   msg;
  msg = iovToMsg(ppld);
  printf("[App] Finished sending a message with the counter equal to %d.\n",
      (int16_t)((radio_count_msg_t *)(&(msg->iovPayload[0])))->counterValue);
  destroyMessage(msg);
}

static int received_callback_demo(heni_packet_addr_t const * paddr,
                heni_iobuf_list_t * ppld,
                heni_packet_rx_info_t const * psts) {
  heni_iobuf_list_fiter_t   piter;
  radio_count_msg_t         rcvMsg;

  printf("[App] Started receiving a packet, %p.\n", ppld);
  if (heniIOBufListGetCapacity(ppld) != sizeof(radio_count_msg_t))
  {
      printf("[App] Invalid packet size, %u vs. %u.\n", \
          (unsigned)heniIOBufListGetCapacity(ppld), \
          (unsigned)sizeof(radio_count_msg_t));
      return -1;
  }
  heniIOBufListFIterInit(&piter, ppld);

  if (heniIOBufListFIterTryCopyIntoOneBufAndAdvance(
          &piter, &rcvMsg, sizeof(radio_count_msg_t)) != sizeof(radio_count_msg_t)) {
      printf("heniIOBufListFIterTryCopyIntoOneBufAndAdvance failed\n");
  }

  printf("[App] The value of the counter received from " \
      "node %u is %u.\n", (uint16_t)rcvMsg.nodeId, \
      (uint16_t)rcvMsg.counterValue);

  counterToLeds(rcvMsg.counterValue);

  heniReceiveFinish(ppld);
  return 0;
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(cc26xx_demo_process, ev, data)
{
  PROCESS_BEGIN();

  if(NETSTACK_RADIO.set_value(RADIO_PARAM_RX_MODE, 0) != RADIO_RESULT_OK) {
    printf("sensniff: Error setting RF in promiscuous mode\n");
    PROCESS_EXIT();
  }

  if (NETSTACK_RADIO.on() != RF_CORE_CMD_OK) {
    printf("on()\n");
  }

  heniSetCallbacks(sent_callback_demo, received_callback_demo);
  memb_init(&MsgAlloc);

  printf("CC26XX demo\n");

  etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);

  while(1) {

    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER) {
      if(data == &et) {
        createAndSendMessages();
        etimer_set(&et, CC26XX_DEMO_LOOP_INTERVAL);
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 * @}
 */
