//
// Created by angelo on 13/05/18.
//

#ifndef ODP_XFSM_PKT_PARSER_H
#define ODP_XFSM_PKT_PARSER_H

#include <wchar.h>
#include <stdint.h>
#include <odp.h>
#include <assert.h>

#define NUM_HDR_FIELDS 27

typedef enum {
    SRC_MAC = 0,
    DST_MAC,
    ETH_TYPE,

    IPv4_SRC,
    IPv4_DST,
    IPv4_TTL,
    IPv4_PROTO,
    IPv4_CHKSUM,

    TCP_SPORT,
    TCP_DPORT,
    TCP_SEQNO,
    TCP_ACKNO,
    TCP_WIN,
    TCP_FLAGS,
    TCP_CHKSUM,
//                      tcp options todo: SACK
    TCP_TIMESTAMP,
    TCP_TS_ECHOREPLY,
    TCP_WIN_SCALE,
    TCP_MAX_MSS,

    UDP_SPORT,
    UDP_DPORT,
    UDP_LENGTH,
    UDP_CHKSUM,
    UDP_P1,
    UDP_P2,
    UDP_P3,
    UDP_P4
} hdr_fields;

const struct xfsm_hdr_fields {
    hdr_fields type;
    const char *name;
    size_t len;
} xfsm_header_fields[NUM_HDR_FIELDS];

typedef struct xfsm_fields_tlv {
    const struct xfsm_hdr_fields *field;
    uint8_t *value;
} xfsm_fields_tlv_t;

/**
 * Copies the value of the provided header field in little endian.
 *
 * @param field xfsm_fields_tlv packet field
 * @return value
 */
uint64_t
get_hdr_value(xfsm_fields_tlv_t field);

#endif //ODP_XFSM_PKT_PARSER_H
