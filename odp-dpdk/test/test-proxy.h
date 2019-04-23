//
// Created by marco on 04/06/18.
//

#ifndef ODP_XFSM_TEST_PROXY_H
#define ODP_XFSM_TEST_PROXY_H

#include "../src/xfsm.h"
#include "../src/tcp-ops.h"

#define TABLE_SIZE 5
#define CONDITIONS 4
#define EXTERNAL_IF 1
#define INTERNAL_IF 2

#endif //ODP_XFSM_TEST_PROXY_H

/**
 * Init XFSM
 */
void init_xfsm(xfsm_instance_t *xfsm) {
    xfsm->registers[XFSM_LASTACKEDSEQ] = 1;
    xfsm->registers[XFSM_CWND] = 4*MSS;
    xfsm->registers[XFSM_AVAILABLE_WIN] = 0;
    xfsm->registers[XFSM_HIGHTXMARK] = 3*MSS+1;
    xfsm->registers[XFSM_ONE] = 1;
    xfsm->registers[XFSM_NEXTTXSEQ] = 3*MSS+1;
    xfsm->registers[XFSM_RTT] = 10;
    xfsm->registers[XFSM_SEGMENTSIZE] = MSS;
}

//TODO: add conditions

//TRUE if the arriving packet comes from the external interface
xfsm_condition_t c0 = {.type1=XFSM_REGISTER, .value1 = XFSM_CONTEXT_PARAM0, .type2=XFSM_CONSTANT, .value2=EXTERNAL_IF,
                        .condition_type= XFSM_EQUAL};
//TRUE if the incoming packet is a SYN
xfsm_condition_t c1 = {.type1=XFSM_HDR_FIELD, .value1 = TCP_FLAGS, .type2=XFSM_CONSTANT, .value2=2,
                        .condition_type= XFSM_EQUAL};
//TRUE if the incoming packet is an ACK
xfsm_condition_t c2 = {.type1=XFSM_HDR_FIELD, .value1 = TCP_FLAGS, .type2=XFSM_CONSTANT, .value2=16,
    .condition_type= XFSM_EQUAL};
//TRUE if the incoming packet is a SYNACK
xfsm_condition_t c3 = {.type1=XFSM_HDR_FIELD, .value1 = TCP_FLAGS, .type2=XFSM_CONSTANT, .value2=18,
    .condition_type= XFSM_EQUAL};

xfsm_condition_t *conditions[CONDITIONS] = {&c0, &c1, &c2, &c3};

/*
 * Table entry 1: a SYN arrives
 *
 */

//Save the arrived packet in the buffer at position 0

xfsm_action_t store = {.type=XFSM_STOREPKT, .type1=XFSM_REGISTER, .value1=XFSM_NULL, .type2=XFSM_CONSTANT, .value2=0,
        .opcode=XFSM_SUM, .output=XFSM_NULL};

//Switch from source port to destination port

xfsm_action_t get_src_port = {.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD,.value1 = TCP_SPORT,
        .type2=XFSM_CONSTANT, .value2=0, .opcode=XFSM_SUM, .output=XFSM_CUSTOM1};

//Switch from destination port to source port


xfsm_action_t set_src_port = {.type=XFSM_SETFIELD, .type1=XFSM_HDR_FIELD,.value1 = TCP_SPORT, .type2=XFSM_CONSTANT,
        .value2 = 255, .type3=XFSM_HDR_FIELD, .value3 = TCP_DPORT};

xfsm_action_t set_dst_port = {.type=XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1=TCP_DPORT, .type2=XFSM_CONSTANT,
                              .value2 = 255, .type3=XFSM_REGISTER, .value3 = XFSM_CUSTOM1};


//Get the seqnum and put in the ACKNUM ADDING 1


xfsm_action_t get_seqnum = {.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD, .value1 = TCP_SEQNO,
        .type2=XFSM_CONSTANT, .value2=0, .opcode=XFSM_SUM, .output=XFSM_CUSTOM1};

xfsm_action_t get_seqnum_plus1 = {.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD, .value1 = TCP_SEQNO,
        .type2=XFSM_CONSTANT, .value2=1, .opcode=XFSM_SUM, .output=XFSM_CUSTOM1};

xfsm_action_t add1= {.type=XFSM_UPDATE, .type1=XFSM_REGISTER, .value1=XFSM_CUSTOM1, .type2=XFSM_CONSTANT, .value2=1,
                     .opcode=XFSM_SUM, .output=XFSM_CUSTOM1};

xfsm_action_t set_acknum = {.type = XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1 = TCP_ACKNO, .type2=XFSM_CONSTANT,
                            .value2 = 255, .type3=XFSM_REGISTER, .value3 = XFSM_CUSTOM1};


//generate acknum 1234  put it in SEQNUM

xfsm_action_t get_1234acknum = {.type=XFSM_UPDATE, .type1=XFSM_CONSTANT, .value1 = 1234,
        .type2=XFSM_CONSTANT, .value2=0, .opcode=XFSM_SUM, .output=XFSM_CUSTOM5};


xfsm_action_t set_seqnum = {.type = XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1 = TCP_SEQNO, .type2=XFSM_CONSTANT,
                            .value2 = 255, .type3=XFSM_REGISTER, .value3 = XFSM_CUSTOM1};

xfsm_action_t set_seqnum_custom5 = {.type = XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1 = TCP_SEQNO, .type2=XFSM_CONSTANT,
        .value2 = 255, .type3=XFSM_REGISTER, .value3 = XFSM_CUSTOM5};


//Set SYNACK flag

xfsm_action_t set_synack_flag = {.type = XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1 = TCP_FLAGS,.type2=XFSM_CONSTANT ,
                                 .value2 = 255, .type3=XFSM_CONSTANT, .value3 = 18};



xfsm_action_t send_pkt_ext = {.type = XFSM_SENDPKT, .type1=XFSM_CONSTANT, .value1 = 255, .type2 = XFSM_CONSTANT,
                              .value2 = EXTERNAL_IF};

//Switch the two IPs


xfsm_action_t get_src_ip ={.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD, .value1=IPv4_SRC, .type2=XFSM_CONSTANT, .value2=0,
                           .opcode=XFSM_SUM, .output=XFSM_CUSTOM0};


xfsm_action_t set_src_ip ={.type=XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1=IPv4_SRC, .type2=XFSM_CONSTANT,
        .value2=255, .type3=XFSM_HDR_FIELD, .value3=IPv4_DST};


xfsm_action_t set_dst_ip ={.type=XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1=IPv4_DST, .type2=XFSM_CONSTANT,
                           .value2=255, .type3=XFSM_REGISTER, .value3=XFSM_CUSTOM0};


//Switch the two MAC addresses

xfsm_action_t get_src_mac ={.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD, .value1=SRC_MAC, .type2=XFSM_CONSTANT, .value2=0, .opcode=XFSM_SUM, .output=XFSM_CUSTOM0};

xfsm_action_t set_src_mac ={.type=XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1=SRC_MAC, .type2=XFSM_CONSTANT, .value2=255, .type3=XFSM_HDR_FIELD, .value3=DST_MAC};

xfsm_action_t set_dst_mac ={.type=XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1=DST_MAC, .type2=XFSM_CONSTANT, .value2=255, .type3=XFSM_REGISTER, .value3=XFSM_CUSTOM0};



//set transmission window to 1

//xfsm_action_t transm_to_1= {.type=XFSM_SETFIELD, .param1=TCP_WIN, .param2=255, .param3=1};



xfsm_action_t *a1[16] = {&store, &get_src_port, &set_src_port, &set_dst_port, &get_seqnum_plus1,&set_acknum, &get_1234acknum,
                        &set_seqnum_custom5, &set_synack_flag, &get_src_ip,
                         &set_src_ip, &set_dst_ip, &get_src_mac, &set_src_mac, &set_dst_mac, &send_pkt_ext};

xfsm_results_t r1[CONDITIONS] = {XFSM_TRUE, XFSM_TRUE, XFSM_FALSE, XFSM_FALSE};

xfsm_table_entry_t e1 = {.results=r1, .actions=a1, .actions_num=16,
        .current_state=0, .event=XFSM_ACK, .next_state=1, .priority=0};

/*
 * Table 2: an ACK arrives from the external interface: we can now start our connection to the server
 */

xfsm_action_t send_syn = {.type=XFSM_SENDPKT, .type1=XFSM_CONSTANT, .value1=0, .type2=XFSM_CONSTANT, .value2=INTERNAL_IF};

xfsm_action_t *a2[1] = {&send_syn};

xfsm_results_t r2[CONDITIONS] = {XFSM_TRUE, XFSM_FALSE, XFSM_TRUE, XFSM_FALSE};

xfsm_table_entry_t e2 = {.results=r2, .actions = a2, .actions_num = 1,
        .current_state = 1, .event = XFSM_ACK, .next_state = 2, .priority = 0};

/*
 *Table 3: a SYNACK from the server arrives
 */
xfsm_action_t save_difference= {.type=XFSM_UPDATE, .type1=XFSM_REGISTER, .value1=XFSM_CUSTOM5, .value2=XFSM_REGISTER,
                                .value2=XFSM_CUSTOM1, .opcode=XFSM_MINUS, .output=XFSM_CUSTOM5};

xfsm_action_t get_acknum = {.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD, .value1=TCP_ACKNO, .type2=XFSM_CONSTANT,
                            .value2=0, .opcode=XFSM_SUM, .output=XFSM_CUSTOM2};

xfsm_action_t set_seqnum2 = {.type = XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1=TCP_SEQNO, .type2=XFSM_CONSTANT,
                             .value2=255, .type3=XFSM_REGISTER, .value3=XFSM_CUSTOM2};

//set the ACK flag

xfsm_action_t set_ack_flag={.type=XFSM_SETFIELD, .type1=XFSM_HDR_FIELD, .value1=TCP_FLAGS, .type2=XFSM_CONSTANT,
                            .value2=255, .type3=XFSM_CONSTANT, .value3=16};

xfsm_action_t send_pkt_int={.type=XFSM_SENDPKT, .type1=XFSM_CONSTANT, .value1=255, .type2=XFSM_CONSTANT,
                            .value2=INTERNAL_IF};

xfsm_action_t *a3[17]={&get_src_port, &set_src_port, &set_dst_port, &get_seqnum, &save_difference,
                       &get_acknum,  &add1, &set_acknum,
                       &set_seqnum2, &set_ack_flag,&get_src_ip,
                       &set_src_ip, &set_dst_ip,  &get_src_mac, &set_src_mac, &set_dst_mac, &send_pkt_int};

xfsm_results_t r3[CONDITIONS]={XFSM_FALSE, XFSM_FALSE, XFSM_FALSE, XFSM_TRUE};

xfsm_table_entry_t e3 ={.results=r3, .actions=a3, .actions_num=17, .current_state=2, .event=XFSM_ACK, .next_state=3, .priority=0};


/*
 * Table 4: an ACK comes from the external interface: in this case, the connection is established (STATE 3) so we can forward the packet
 */

xfsm_action_t translate_acknum ={.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD, .value1=TCP_ACKNO, .type2=XFSM_REGISTER,
                                 .value2=XFSM_CUSTOM5, .opcode = XFSM_MINUS, .output=XFSM_CUSTOM1};


xfsm_action_t *a4[3]={&translate_acknum, &set_acknum, &send_pkt_int};

xfsm_results_t r4[CONDITIONS]={XFSM_TRUE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_table_entry_t e4={.results=r4, .actions=a4, .actions_num=3, .current_state=3, .event=XFSM_ACK, .next_state=3, .priority=0};


/*
 * Table 5: and ACK comes from the external interface: in this case, the connection is established (STATE 3) so we can forward the packet
 */

xfsm_action_t translate_seqnum={.type=XFSM_UPDATE, .type1=XFSM_HDR_FIELD, .value1=TCP_SEQNO, .type2=XFSM_REGISTER,
                                .value2=XFSM_CUSTOM5, .opcode=XFSM_SUM, .output=XFSM_CUSTOM1};

xfsm_action_t *a5[3]={&translate_seqnum, &set_seqnum, &send_pkt_ext};

xfsm_results_t r5[CONDITIONS]={XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_table_entry_t e5={.results=r5, .actions=a5, .actions_num=3, .current_state=3, .event=XFSM_ACK, .next_state=3, .priority=0};

xfsm_table_entry_t *entries[TABLE_SIZE]= {&e1, &e2, &e3, &e4, &e5};
