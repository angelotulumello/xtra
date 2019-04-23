//
// Created by angelo on 10/04/18.
//

#ifndef ODP_XFSM_PARSE_ARGS_H
#define ODP_XFSM_PARSE_ARGS_H

#include <inttypes.h>

typedef struct appl_args{
    uint32_t src_addr;
    uint32_t dst_addr;
    uint16_t src_port;
    uint16_t dst_port;
    char *dst_mac;
    char *src_mac;
    uint8_t num_pktios;
} appl_args_t;

appl_args_t *appl_args;

void parse_args(int argc, char *argv[], appl_args_t *appl_args);


#endif //ODP_XFSM_PARSE_ARGS_H
