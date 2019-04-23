#ifndef ODP_XFSM_TCP_OPS_H
#define ODP_XFSM_TCP_OPS_H


#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "odp_api.h"
#include "odp/helper/odph_api.h"

#define MSS     1448

#define END_OF_LIST_OPT     0x00
#define NOP_OPT             0x01
#define MAX_SEG_SIZE_OPT    0x02
#define WIN_SCALE_OPT       0x03
#define SACK_OK_OPT         0x04
#define SACK_OPT            0x05
#define TIMESTAMP_OPT       0x08

typedef struct timestamp {
    uint8_t nop[2];
    uint8_t kind;
    uint8_t len;
    uint32_t ts;
    uint32_t ts_echo;
} tcp_ts_opt;

odp_packet_t setup_tcp_pkt_ref(odp_pool_t pool, uint32_t ackno);

odp_packet_t setup_udp_pkt_ref(odp_pool_t pool);

odp_packet_t setup_tcp_empty_pkt_ref(odp_pool_t pool);

void pack_tcp_syn(odp_packet_t packet);

void pack_tcp_ack(odp_packet_t packet, uint32_t init_seq);

void pack_tcp_data(odp_packet_t packet, uint32_t seq, uint32_t timestamp);

uint32_t *get_tcp_ack_ts(odp_packet_t packet);

#endif //ODP_XFSM_TCP_OPS_H
