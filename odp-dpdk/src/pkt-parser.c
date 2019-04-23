
#include "pkt-parser.h"
#include "xfsm.h"
#include "tcp-ops.h"


const struct xfsm_hdr_fields xfsm_header_fields[NUM_HDR_FIELDS] = {
    {SRC_MAC,               "SRC_MAC" ,                 6},
    {DST_MAC,               "DST_MAC",                  6},
    {ETH_TYPE,              "ETH_TYPE",                 2},
    {IPv4_SRC,              "IPv4_SRC",                 4},
    {IPv4_DST,              "IPv4_DST",                 4},
    {IPv4_TTL,              "IPv4_TTL",                 1},
    {IPv4_PROTO,            "IPv4_PROTO",               1},
    {IPv4_CHKSUM,           "IPv4_CHKSUM",              2},
    {TCP_SPORT,             "TCP_SPORT",                2},
    {TCP_DPORT,             "TCP_DPORT",                2},
    {TCP_SEQNO,             "TCP_SEQNO",                4},
    {TCP_ACKNO,             "TCP_ACKNO",                4},
    {TCP_WIN,               "TCP_WIN",                  1},
    {TCP_FLAGS,             "TCP_FLAGS",                1},
    {TCP_CHKSUM,            "TCP_CHKSUM",               2},
    {TCP_TIMESTAMP,         "TCP_TIMESTAMP",            4},
    {TCP_TS_ECHOREPLY,      "TCP_TS_ECHOREPLY",         4},
    {TCP_WIN_SCALE,         "TCP_WIN_SCALE",            1},
    {TCP_MAX_MSS,           "TCP_MAX_MSS",              4},
    {UDP_SPORT,             "UDP_SPORT",                2},
    {UDP_DPORT,             "UDP_DPORT",                2},
    {UDP_LENGTH,            "UDP_LENGTH",               2},
    {UDP_CHKSUM,            "UDP_CHKSUM",               2},
    {UDP_P1,                "UDP_P1",                   4},
    {UDP_P2,                "UDP_P2",                   4},
    {UDP_P3,                "UDP_P3",                   4},
    {UDP_P4,                "UDP_P4",                   4}
};


int parse_pkt_header(xfsm_table_t *xfsm, odp_packet_t packet){
    uint8_t *eth, *ipv4, *tcp, *udp;
    uint32_t len = 0;
    uint32_t offset = 0;
    int has_tcp, has_udp;

    if (packet == ODP_PACKET_INVALID)
        return -1;

    /*if (!odp_packet_has_eth(packet))
        return -2;*/

    if (!odp_packet_has_ipv4(packet))
        return -3;

    has_tcp = odp_packet_has_tcp(packet);
    has_udp = odp_packet_has_udp(packet);

    if (!has_tcp && !has_udp)
        return -4;

    eth = odp_packet_data(packet);

    // Ethernet
    xfsm->pkt_fields[DST_MAC].value = eth;
    xfsm->pkt_fields[SRC_MAC].value = eth + 6;
    xfsm->pkt_fields[ETH_TYPE].value = eth + 12;

    offset += ODPH_ETHHDR_LEN;

    // IPv4
    ipv4 = eth + offset;
    xfsm->pkt_fields[IPv4_DST].value = ipv4 + 16;
    xfsm->pkt_fields[IPv4_SRC].value = ipv4 + 12;
    xfsm->pkt_fields[IPv4_PROTO].value = ipv4 + 9;
    xfsm->pkt_fields[IPv4_TTL].value = ipv4 + 8;
    xfsm->pkt_fields[IPv4_CHKSUM].value = ipv4 + 10;

    offset += ODPH_IPV4HDR_LEN;

    // TCP
    if (has_tcp){
        xfsm->pkt_fields[UDP_SPORT].value  = NULL;
        xfsm->pkt_fields[UDP_DPORT].value  = NULL;
        xfsm->pkt_fields[UDP_LENGTH].value = NULL;
        xfsm->pkt_fields[UDP_CHKSUM].value = NULL;
        xfsm->pkt_fields[UDP_P1].value = NULL;
        xfsm->pkt_fields[UDP_P2].value = NULL;
        xfsm->pkt_fields[UDP_P3].value = NULL;
        xfsm->pkt_fields[UDP_P4].value = NULL;

        tcp = eth + offset;
        xfsm->pkt_fields[TCP_SPORT].value  = tcp;
        xfsm->pkt_fields[TCP_DPORT].value  = tcp + 2;
        xfsm->pkt_fields[TCP_SEQNO].value  = tcp + 4;
        xfsm->pkt_fields[TCP_ACKNO].value  = tcp + 8;
        xfsm->pkt_fields[TCP_FLAGS].value  = tcp + 13;
        xfsm->pkt_fields[TCP_WIN].value    = tcp + 14;
        xfsm->pkt_fields[TCP_CHKSUM].value = tcp + 16;

        uint8_t tcp_hdr_len = (uint8_t ) (tcp[12] & 0xf0) >> 2;

        // there are options
        if (tcp_hdr_len > 20) {
            uint8_t *cursor;
            for (int i=0; i<tcp_hdr_len; i++) {
                cursor = tcp + 20 + i;

                int opt_len = 0;
                if (*cursor != NOP_OPT) {
                    switch (*cursor) {
                        // TODO: parse all options
                        case WIN_SCALE_OPT:
                            opt_len = *(cursor + 1);
                            xfsm->pkt_fields[TCP_WIN_SCALE].value = cursor + 2;
                            i += opt_len;
                            break;
                        case TIMESTAMP_OPT:
                            opt_len = *(cursor + 1);
                            xfsm->pkt_fields[TCP_TIMESTAMP].value = cursor + 2;
                            xfsm->pkt_fields[TCP_TS_ECHOREPLY].value = cursor + 6;
                            i += opt_len;
                            break;
                        case SACK_OPT:
                            break;
                        case MAX_SEG_SIZE_OPT:
                            break;
                        case END_OF_LIST_OPT:
                            break;
                        default:
                            break;
                    }
                }
            }

        }
        return ODPH_IPPROTO_TCP;
    }
        // UDP
    else {
        udp = odp_packet_l4_ptr(packet, &len);

        xfsm->pkt_fields[TCP_SPORT].value = NULL;
        xfsm->pkt_fields[TCP_DPORT].value = NULL;
        xfsm->pkt_fields[TCP_SEQNO].value = NULL;
        xfsm->pkt_fields[TCP_ACKNO].value = NULL;
        xfsm->pkt_fields[TCP_FLAGS].value = NULL;
        xfsm->pkt_fields[TCP_WIN].value   = NULL;
        xfsm->pkt_fields[TCP_CHKSUM].value = NULL;

        xfsm->pkt_fields[TCP_WIN_SCALE].value      = NULL;
        xfsm->pkt_fields[TCP_TIMESTAMP].value      = NULL;
        xfsm->pkt_fields[TCP_TS_ECHOREPLY].value   = NULL;

        xfsm->pkt_fields[UDP_SPORT].value   = udp;
        xfsm->pkt_fields[UDP_DPORT].value   = udp + 2;
        xfsm->pkt_fields[UDP_LENGTH].value  = udp + 4;
        xfsm->pkt_fields[UDP_CHKSUM].value  = udp + 6;
        xfsm->pkt_fields[UDP_P1].value      = udp + 8;
        xfsm->pkt_fields[UDP_P2].value      = udp + 12;
        xfsm->pkt_fields[UDP_P3].value      = udp + 16;
        xfsm->pkt_fields[UDP_P4].value      = udp + 20;

        return ODPH_IPPROTO_UDP;
    }
}


uint64_t get_hdr_value(xfsm_fields_tlv_t field) {
    uint64_t value = 0;

    if (field.value == NULL)
        return 0;

    uint8_t *byte = field.value;
    switch (field.field->len) {
        case 6:
            for (int i=0; i<6; i++)
                value |= ((uint64_t ) byte[i]) << (5-i)*8;
            break;
        case 4:
            for (int i=0; i<4; i++)
                value |= byte[i] << (3-i)*8;
            break;
        case 2:
            value |= byte[0]<<8 | byte[1];
            break;
        case 1:
            value = byte[0];
            break;
        default:
            break;
    }

    return value;
}