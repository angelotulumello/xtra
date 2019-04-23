#include <stdlib.h>
#include <odp/helper/tcp.h>
#include <odp/helper/ip.h>
#include <odp/helper/udp.h>
#include "tcp-ops.h"
#include "example_debug.h"
#include "parse-args.h"


odp_packet_t setup_tcp_pkt_ref(odp_pool_t pool, uint32_t ackno)
{
    odp_packet_t pkt;
    char *buf;
    odph_ethhdr_t *eth;
    odph_ipv4hdr_t *ip;
    odph_tcphdr_t *tcp;
    tcp_ts_opt *ts_opt;
    uint16_t payload = MSS;
    uint32_t ip_offset;

    pkt = odp_packet_alloc(pool, payload + sizeof(tcp_ts_opt) + ODPH_TCPHDR_LEN +
                                 ODPH_IPV4HDR_LEN + ODPH_ETHHDR_LEN);

    if (pkt == ODP_PACKET_INVALID)
        return pkt;

    buf = odp_packet_data(pkt);

    odph_ethaddr_t ethsrc = {0};
    odph_ethaddr_t ethdst = {0};
    odph_eth_addr_parse(&ethsrc, appl_args->src_mac);
    odph_eth_addr_parse(&ethdst, appl_args->dst_mac);


    /* ether */
    odp_packet_l2_offset_set(pkt, 0);
    eth = (odph_ethhdr_t *)buf;
    memcpy(eth->src.addr, &ethsrc, sizeof(odph_ethaddr_t));
    memcpy(eth->dst.addr, &ethdst, sizeof(odph_ethaddr_t));
    eth->type = odp_cpu_to_be_16(ODPH_ETHTYPE_IPV4);
    odp_packet_has_eth_set(pkt, 1);

    /* ip */
    odp_packet_l3_offset_set(pkt, ODPH_ETHHDR_LEN);
    odp_packet_has_ipv4_set(pkt, 1);
    ip = (odph_ipv4hdr_t *)(buf + ODPH_ETHHDR_LEN);
    ip->dst_addr = odp_cpu_to_be_32(appl_args->dst_addr);
    ip->src_addr = odp_cpu_to_be_32(appl_args->src_addr);
    ip->ver_ihl = ODPH_IPV4 << 4 | ODPH_IPV4HDR_IHL_MIN;
    ip->tot_len = odp_cpu_to_be_16(payload + ODPH_TCPHDR_LEN + sizeof(tcp_ts_opt) + ODPH_IPV4HDR_LEN);
    ip->proto = ODPH_IPPROTO_TCP;
    ip->id = 0;
    ip->ttl = 64;
    ip->chksum = 0;

    /* tcp */
    odp_packet_l4_offset_set(pkt, ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN);
    odp_packet_has_tcp_set(pkt, 1);
    tcp = (odph_tcphdr_t *)(buf + ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN);
    tcp->src_port = odp_cpu_to_be_16(appl_args->src_port);
    tcp->dst_port = odp_cpu_to_be_16(appl_args->dst_port);
    tcp->seq_no = odp_cpu_to_be_32(1);
    tcp->ack_no = odp_cpu_to_be_32(ackno);
    tcp->hl = odp_cpu_to_be_16(0x0800);
    tcp->ack = 1;
    tcp->window = odp_cpu_to_be_16(32768);
    tcp->urgptr = odp_cpu_to_be_16(0x0);

    /* tcp option timestamp */
    ts_opt = (tcp_ts_opt *) (buf + ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN + ODPH_TCPHDR_LEN);
    ts_opt->nop[0] = 0x01;
    ts_opt->nop[1] = 0x01;
    ts_opt->kind = 0x08;
    ts_opt->len = 0x0a;
    ts_opt->ts = 0;
    ts_opt->ts_echo = 0;

    //ip = odp_packet_l3_ptr(pkt, NULL);
    //ip_offset = odp_packet_l3_offset(pkt);
    //odph_ipv4_csum(pkt, ip_offset, ip, &ip->chksum);

    //odph_udp_tcp_chksum(pkt, ODPH_CHKSUM_GENERATE, &tcp->cksm);

    odp_packet_t out_pkt = odp_packet_ref(pkt, 0);

    return out_pkt;
}


odp_packet_t setup_tcp_empty_pkt_ref(odp_pool_t pool)
{
    odp_packet_t pkt;
    char *buf;
    odph_ethhdr_t *eth;
    odph_ipv4hdr_t *ip;
    odph_tcphdr_t *tcp;
    tcp_ts_opt *ts_opt;

    pkt = odp_packet_alloc(pool, sizeof(tcp_ts_opt) + ODPH_TCPHDR_LEN + ODPH_IPV4HDR_LEN + ODPH_ETHHDR_LEN);

    if (pkt == ODP_PACKET_INVALID)
        return pkt;

    buf = odp_packet_data(pkt);

    odph_ethaddr_t ethsrc = {0};
    odph_ethaddr_t ethdst = {0};
    odph_eth_addr_parse(&ethsrc, appl_args->src_mac);
    odph_eth_addr_parse(&ethdst, appl_args->dst_mac);

    /* ether */
    odp_packet_l2_offset_set(pkt, 0);
    eth = (odph_ethhdr_t *)buf;
    memcpy(eth->src.addr, &ethsrc, sizeof(odph_ethaddr_t));
    memcpy(eth->dst.addr, &ethdst, sizeof(odph_ethaddr_t));
    eth->type = odp_cpu_to_be_16(ODPH_ETHTYPE_IPV4);

    /* ip */
    odp_packet_l3_offset_set(pkt, ODPH_ETHHDR_LEN);
    odp_packet_has_ipv4_set(pkt, 1);
    ip = (odph_ipv4hdr_t *)(buf + ODPH_ETHHDR_LEN);
    ip->dst_addr = odp_cpu_to_be_32(appl_args->dst_addr);
    ip->src_addr = odp_cpu_to_be_32(appl_args->src_addr);
    ip->ver_ihl = ODPH_IPV4 << 4 | ODPH_IPV4HDR_IHL_MIN;
    ip->tot_len = odp_cpu_to_be_16(ODPH_TCPHDR_LEN + sizeof(tcp_ts_opt) + ODPH_IPV4HDR_LEN);
    ip->proto = ODPH_IPPROTO_TCP;
    ip->id = 0;
    ip->ttl = 64;
    ip->chksum = 0;

    /* tcp */
    odp_packet_l4_offset_set(pkt, ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN);
    odp_packet_has_tcp_set(pkt, 1);
    tcp = (odph_tcphdr_t *)(buf + ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN);
    tcp->src_port = odp_cpu_to_be_16(appl_args->src_port);
    tcp->dst_port = odp_cpu_to_be_16(appl_args->dst_port);
    tcp->seq_no = odp_cpu_to_be_32(10101010);
    tcp->ack_no = odp_cpu_to_be_32(0);
    tcp->hl = odp_cpu_to_be_16(0x0800);
    tcp->ack = 0;
    tcp->window = odp_cpu_to_be_16(32768);
    tcp->urgptr = odp_cpu_to_be_16(0x0);

    /* tcp option timestamp */
    ts_opt = (tcp_ts_opt *) (buf + ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN + ODPH_TCPHDR_LEN);
    ts_opt->nop[0] = 0x01;
    ts_opt->nop[1] = 0x01;
    ts_opt->kind = 0x08;
    ts_opt->len = 0x0a;
    ts_opt->ts = 0;
    ts_opt->ts_echo = 0;

    odp_packet_t out_pkt = odp_packet_ref(pkt, 0);

    odp_packet_print(out_pkt);

    return out_pkt;
}

void pack_tcp_syn(odp_packet_t packet)
{
    odph_tcphdr_t *tcp;
    odph_ipv4hdr_t *ip;
    uint32_t ip_offset;

    tcp = (odph_tcphdr_t *) odp_packet_l4_ptr(packet, NULL);
    tcp->seq_no = 0;
    tcp->flags = 0x0000;
    tcp->hl = odp_cpu_to_be_16(0x0800);
    tcp->syn = 1;

    ip = odp_packet_l3_ptr(packet, NULL);
    ip_offset = odp_packet_l3_offset(packet);
    odph_ipv4_csum(packet, ip_offset, ip, &ip->chksum );

    odph_udp_tcp_chksum(packet, ODPH_CHKSUM_GENERATE, &tcp->cksm);

}

void pack_tcp_ack(odp_packet_t packet, uint32_t ackno)
{
    odph_tcphdr_t *tcp;
    odph_ipv4hdr_t *ip;
    uint32_t ip_offset;

    tcp = (odph_tcphdr_t *) odp_packet_l4_ptr(packet, NULL);
    tcp->hl = odp_cpu_to_be_16(0x0800);
    tcp->syn = 0;
    tcp->ack = 1;
    tcp->ack_no = odp_cpu_to_be_32(ackno);
    tcp->seq_no = odp_cpu_to_be_32(1);

    ip = odp_packet_l3_ptr(packet, NULL);
    ip_offset = odp_packet_l3_offset(packet);
    odph_ipv4_csum(packet, ip_offset, ip, &ip->chksum );

    odph_udp_tcp_chksum(packet, ODPH_CHKSUM_GENERATE, &tcp->cksm);
}

void pack_tcp_data(odp_packet_t packet, uint32_t seq, uint32_t timestamp)
{
    odph_tcphdr_t *tcp;
    tcp_ts_opt *ts_opt;
    odph_ipv4hdr_t *ip;
    uint32_t ip_offset;

    tcp = (odph_tcphdr_t *) odp_packet_l4_ptr(packet, NULL);
    tcp->seq_no = odp_cpu_to_be_32(seq);
    ts_opt = odp_packet_offset(packet, ODPH_ETHHDR_LEN+ODPH_IPV4HDR_LEN+ODPH_TCPHDR_LEN, NULL, NULL);
    ts_opt->ts = odp_cpu_to_be_32(timestamp);

    ip = odp_packet_l3_ptr(packet, NULL);
    ip_offset = odp_packet_l3_offset(packet);
    odph_ipv4_csum(packet, ip_offset, ip, &ip->chksum );

    odph_udp_tcp_chksum(packet, ODPH_CHKSUM_GENERATE, &tcp->cksm);
}

uint32_t *get_tcp_ack_ts(odp_packet_t packet)
{
    odph_tcphdr_t *tcp;
    tcp_ts_opt *ts_opt;
    uint32_t ackNo = 0, ts_echo = 0;

    //packet = odp_packet_ref_static(packet);

    if (!odp_packet_has_tcp(packet)){
        EXAMPLE_ERR("Packet has no tcp\n");
        return NULL;
    } else {
        tcp = (odph_tcphdr_t *) odp_packet_l4_ptr(packet, NULL);
        ts_opt = odp_packet_offset(packet, ODPH_ETHHDR_LEN+ODPH_IPV4HDR_LEN+ODPH_TCPHDR_LEN, NULL, NULL);

        ackNo = odp_be_to_cpu_32(tcp->ack_no);
        ts_echo = odp_be_to_cpu_32(ts_opt->ts_echo);

        uint32_t *buf = (uint32_t *) malloc(8);
        buf[0] = ackNo;
        buf[1] = ts_echo;
        return buf;
    }

}

odp_packet_t setup_udp_pkt_ref(odp_pool_t pool)
{
    odp_packet_t pkt;
    char *buf;
    odph_ethhdr_t *eth;
    odph_ipv4hdr_t *ip;
    odph_udphdr_t *udp;
    tcp_ts_opt *ts_opt;
    uint16_t payload = MSS;
    uint32_t ip_offset;

    pkt = odp_packet_alloc(pool, payload + ODPH_UDPHDR_LEN + ODPH_IPV4HDR_LEN + ODPH_ETHHDR_LEN);

    if (pkt == ODP_PACKET_INVALID)
        return pkt;

    buf = odp_packet_data(pkt);

    odph_ethaddr_t ethsrc = {0};
    odph_ethaddr_t ethdst = {0};
    odph_eth_addr_parse(&ethsrc, appl_args->src_mac);
    odph_eth_addr_parse(&ethdst, appl_args->dst_mac);


    /* ether */
    odp_packet_l2_offset_set(pkt, 0);
    eth = (odph_ethhdr_t *)buf;
    memcpy(eth->src.addr, &ethsrc, sizeof(odph_ethaddr_t));
    memcpy(eth->dst.addr, &ethdst, sizeof(odph_ethaddr_t));
    eth->type = odp_cpu_to_be_16(ODPH_ETHTYPE_IPV4);
    odp_packet_has_eth_set(pkt, 1);

    /* ip */
    odp_packet_l3_offset_set(pkt, ODPH_ETHHDR_LEN);
    odp_packet_has_ipv4_set(pkt, 1);
    ip = (odph_ipv4hdr_t *)(buf + ODPH_ETHHDR_LEN);
    ip->dst_addr = odp_cpu_to_be_32(appl_args->dst_addr);
    ip->src_addr = odp_cpu_to_be_32(appl_args->src_addr);
    ip->ver_ihl = ODPH_IPV4 << 4 | ODPH_IPV4HDR_IHL_MIN;
    ip->tot_len = odp_cpu_to_be_16((uint16_t ) (payload + ODPH_UDPHDR_LEN + ODPH_IPV4HDR_LEN));
    ip->proto = ODPH_IPPROTO_UDP;
    ip->id = 0;
    ip->ttl = 64;
    ip->chksum = 0;

    odp_packet_l4_offset_set(pkt, ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN);
    odp_packet_has_udp_set(pkt, 1);
    udp = (odph_udphdr_t *) (buf + ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN);
    udp->src_port = odp_cpu_to_be_16(appl_args->src_port);
    udp->dst_port = odp_cpu_to_be_16(appl_args->dst_port);
    udp->length = 0;
    udp->chksum = 0;

    odp_packet_t out_pkt = odp_packet_ref(pkt, 0);

    return out_pkt;
}
