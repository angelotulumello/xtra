/*
 * Xfsm hardcoded table entries description
 * This version does not support spurious loss detection
 * and sack parsing.
 */

#ifndef ODP_XFSM_TEB_XFSM_H
#define ODP_XFSM_TEB_XFSM_H

#include "../src/xfsm.h"
#include "../src/tcp-ops.h"

#define TABLE_SIZE 2
#define CONDITIONS 1

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

/*
 * Configure conditions
 */
// it's a bad timer????
xfsm_condition_t c0 = {.register1=XFSM_NULL, .register2=XFSM_NULL, .field1=TCP_FLAGS,
        .field2=NULL_FIELD, .constant=2, .condition_type=XFSM_EQUAL};

xfsm_condition_t *conditions[CONDITIONS] = {&c0};

/*
 * Configure table entries
 */

/**
 * **************************************************************
 * Slow start
 * **************************************************************
*/

/**
 * Table entry 1
 * SLOWSTART SEND
 */

xfsm_action_t store = {.type=XFSM_STOREPKT, .register1=XFSM_NULL, .register2=XFSM_NULL, .param2=0,
                       .register3=XFSM_NULL, .opcode=XFSM_SUM, .output=XFSM_NULL};
xfsm_action_t dport = {.type=XFSM_UPDATE, .register1=XFSM_ZERO, .register2=XFSM_NULL, .param2=TCP_TS_ECHOREPLY,
    .register3=XFSM_NULL, .opcode=XFSM_SUM, .output=XFSM_CUSTOM0};
xfsm_action_t dport_value = {.type=XFSM_UPDATE, .register1=XFSM_ZERO, .register2=XFSM_NULL, .param2=7777,
    .register3=XFSM_NULL, .opcode=XFSM_SUM, .output=XFSM_CUSTOM1};
xfsm_action_t set_field = {.type=XFSM_SETFIELD, .register1=XFSM_CUSTOM0, .register2=XFSM_ZERO, .register3=XFSM_CUSTOM1};
xfsm_action_t send_stored = {.type=XFSM_SENDPKT, .register1=XFSM_ZERO, .register2=XFSM_NULL, .param2=1,
    .register3=XFSM_NULL, .opcode=XFSM_SUM, .output=XFSM_NULL};
xfsm_action_t setTimer = {.type=XFSM_SETTIMER, .register1=XFSM_NULL, .register2=XFSM_NULL, .register3=XFSM_NULL,
                           .param1=1212, .param2=12, .param3=5000000,
                           .opcode=XFSM_SUM, .output=XFSM_NULL};

xfsm_action_t *a1[6] = {&store, &dport, &dport_value, &set_field, &send_stored, &setTimer};

xfsm_results_t r1[CONDITIONS] = {XFSM_TRUE};

xfsm_table_entry_t e1 = {.results=r1, .actions=a1, .actions_num=6,
        .current_state=0, .event=XFSM_ACK, .next_state=0, .priority=0};


/**
 * table entry 2
 */

xfsm_action_t *a2[1] = {&send_stored};

xfsm_results_t r2[CONDITIONS] = {XFSM_DONT_CARE};

xfsm_table_entry_t e2 = {.results=r2, .actions=a2, .actions_num=1,
    .current_state=0, .event=XFSM_TIMEOUT, .next_state=0, .priority=0};

/**
 * final entries
 */
xfsm_table_entry_t *entries[TABLE_SIZE] = {&e1, &e2};

#endif //ODP_XFSM_TEB_XFSM_H
