/*
 * Xfsm hardcoded table entries description
 * This version does not support spurious loss detection
 * and sack parsing.
 */

#ifndef ODP_XFSM_TEB_XFSM_H
#define ODP_XFSM_TEB_XFSM_H

#include "../src/xfsm.h"
#include "../src/tcp-ops.h"

#define TABLE_SIZE 11
#define CONDITIONS 9

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
xfsm_condition_t c0 = {.register1=XFSM_CONTEXT_PARAM1, .register2=XFSM_ZERO,
        .constant=0, .condition_type=XFSM_GREATER};
// AVAILABLE WIN > 0
xfsm_condition_t c1 = {.register1=XFSM_AVAILABLE_WIN, .register2=XFSM_ZERO,
        .constant=0, .condition_type=XFSM_GREATER};
// LAST ACKED SEQUENCE == ACK NUMBER
xfsm_condition_t c2 = {.register1=XFSM_LASTACKEDSEQ, .register2=XFSM_CONTEXT_PARAM0,
        .constant=0, .condition_type=XFSM_EQUAL};
// timerEntrySeq < last acked
xfsm_condition_t c3 = {.register1=XFSM_CONTEXT_PARAM0, .register2=XFSM_LASTACKEDSEQ,
        .constant=0, .condition_type=XFSM_LESS};
// currentReTXRound < timerEntryReTXCount ?
xfsm_condition_t c4 = {.register1=XFSM_CUSTOM0, .register2=XFSM_CONTEXT_PARAM1,
        .constant=0, .condition_type=XFSM_LESS};
// nexttxseq >= highTXMark ?
xfsm_condition_t c5 = {.register1=XFSM_NEXTTXSEQ, .register2=XFSM_HIGHTXMARK,
        .constant=0, .condition_type=XFSM_GREATER_EQ};
// nexttxseq >= highTXMark ?
xfsm_condition_t c6 = {.register1=XFSM_CONTEXT_PARAM0, .register2=XFSM_HIGHTXMARK,
        .constant=0, .condition_type=XFSM_GREATER_EQ};
// availableWin >= segmentSize ?
xfsm_condition_t c7 = {.register1=XFSM_AVAILABLE_WIN, .register2=XFSM_SEGMENTSIZE,
        .constant=0, .condition_type=XFSM_GREATER_EQ};
// timerEntrySeq < last acked
xfsm_condition_t c8 = {.register1=XFSM_CONTEXT_PARAM0, .register2=XFSM_NEXTTXSEQ,
        .constant=0, .condition_type=XFSM_EQUAL};


xfsm_condition_t *conditions[CONDITIONS] = {&c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8};

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
xfsm_action_t availWin1 = {.type=XFSM_UPDATE, .register1=XFSM_HIGHTXMARK, .register2=XFSM_LASTACKEDSEQ,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_MINUS, .output=XFSM_AVAILABLE_WIN};
xfsm_action_t availWin2 = {.type=XFSM_UPDATE, .register1=XFSM_CWND, .register2=XFSM_AVAILABLE_WIN,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_MINUS, .output=XFSM_AVAILABLE_WIN};
xfsm_action_t doubRtt = {.type=XFSM_UPDATE, .register1=XFSM_RTT, .register2=XFSM_NULL,
        .register3=XFSM_NULL, .param2=2, .opcode=XFSM_MULTIPLY, .output=XFSM_ARG0};
xfsm_action_t setTimerF = {.type=XFSM_SETTIMER, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_ONE, .register3=XFSM_ARG0,
        .param2=0, .opcode=XFSM_SUM, .output=XFSM_NULL};
xfsm_action_t setFastTimer = {.type=XFSM_SETTIMER, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_ZERO, .register3=XFSM_ONE,
        .param2=0, .opcode=XFSM_SUM, .output=XFSM_NULL};
xfsm_action_t sendPkt = {.type=XFSM_SENDPKT, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_SEGMENTSIZE, .register3=XFSM_NULL,
        .param2=0, .opcode=XFSM_SUM, .output=XFSM_NULL};
xfsm_action_t nextTX = {.type=XFSM_UPDATE, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_SEGMENTSIZE,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_SUM, .output=XFSM_NEXTTXSEQ};
xfsm_action_t highTX = {.type=XFSM_UPDATE, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_SEGMENTSIZE,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_SUM, .output=XFSM_HIGHTXMARK};

xfsm_action_t *a1[8] = {&doubRtt, &setTimerF, &sendPkt, &nextTX, &highTX, &setFastTimer, &availWin1, &availWin2};

xfsm_results_t r1[CONDITIONS] = {XFSM_FALSE, XFSM_TRUE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_table_entry_t e1 = {.results=r1, .actions=a1, .actions_num=8,
        .current_state=0, .event=XFSM_TIMEOUT, .next_state=0, .priority=0};

/**
 * Table entry 2
 * SLOWSTART ACK
 */
xfsm_action_t updateRTT = {.type=XFSM_UPDATE, .register1=XFSM_RTT, .register2=XFSM_CONTEXT_PARAM1, .register3=XFSM_NULL,
        .param2=0, .opcode=XFSM_MAX, .output=XFSM_RTT};
xfsm_action_t lastAcked = {.type=XFSM_UPDATE, .register1=XFSM_CONTEXT_PARAM0, .register2=XFSM_ZERO, .register3=XFSM_NULL,
        .param2=0, .opcode=XFSM_SUM, .output=XFSM_LASTACKEDSEQ};
xfsm_action_t doubCWND = {.type=XFSM_UPDATE, .register1=XFSM_CWND, .register2=XFSM_SEGMENTSIZE, .register3=XFSM_NULL,
        .param2=0, .opcode=XFSM_SUM, .output=XFSM_CWND};

xfsm_results_t r2[CONDITIONS] = {XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_action_t *a2[6] = {&doubCWND, &lastAcked, &setFastTimer, &updateRTT, &availWin1, &availWin2};

xfsm_table_entry_t e2 = {.results=r2, .actions=a2, .actions_num=6,
        .current_state=0, .event=XFSM_ACK, .next_state=0, .priority=0};


/**
 * Table entry 3
 * SLOWSTART -> RECOVERY
 */
xfsm_action_t divCwnd = {.type=XFSM_UPDATE, .register1=XFSM_CWND, .register2=XFSM_NULL, .register3=XFSM_NULL,
        .param2=2, .opcode=XFSM_DIVIDE, .output=XFSM_CWND};
xfsm_action_t incReTx = {.type=XFSM_UPDATE, .register1=XFSM_CUSTOM0, .register2=XFSM_NULL, .register3=XFSM_NULL,
        .param2=1, .opcode=XFSM_SUM, .output=XFSM_CUSTOM0};
xfsm_action_t pacing1 = {.type=XFSM_UPDATE, .register1=XFSM_CWND, .register2=XFSM_SEGMENTSIZE, .register3=XFSM_NULL,
        .param2=0, .opcode=XFSM_DIVIDE, .output=XFSM_CUSTOM2};
xfsm_action_t pacing2 = {.type=XFSM_UPDATE, .register1=XFSM_RTT, .register2=XFSM_CUSTOM2, .register3=XFSM_NULL,
        .param2=0, .opcode=XFSM_DIVIDE, .output=XFSM_CUSTOM1};
xfsm_action_t nextTXRec = {.type=XFSM_UPDATE, .register1=XFSM_ZERO, .register2=XFSM_LASTACKEDSEQ,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_SUM, .output=XFSM_NEXTTXSEQ};
xfsm_action_t setTimerPac = {.type=XFSM_SETTIMER, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_ZERO,
        .register3=XFSM_CUSTOM1, .param2=0, .opcode=XFSM_SUM, .output=XFSM_NULL};



xfsm_results_t r3[CONDITIONS] = {XFSM_TRUE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_FALSE, XFSM_TRUE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_action_t *a3[6] = {&divCwnd, &incReTx, &pacing1, &pacing2, &nextTXRec, &setTimerPac};

xfsm_table_entry_t e3 = {.results=r3, .actions=a3, .actions_num=6,
        .current_state=0, .event=XFSM_TIMEOUT, .next_state=1, .priority=0};

/**
 * Table entry 4
 * RECOVERY PACING
 */
xfsm_results_t r4[CONDITIONS] = {XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_TRUE};

xfsm_action_t *a4[3] = {&sendPkt, &nextTX, &setTimerPac};

xfsm_table_entry_t e4 = {.results=r4, .actions=a4, .actions_num=3,
        .current_state=1, .event=XFSM_TIMEOUT, .next_state=1, .priority=0};

/**
 * Table entry 5
 * RECOVERY -> POST-RECOVERY
 */
xfsm_action_t pktsToPace = {.type=XFSM_UPDATE, .register1=XFSM_CUSTOM2, .register2=XFSM_ONE,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_SUM, .output=XFSM_CUSTOM2};
xfsm_action_t decReTX = {.type=XFSM_UPDATE, .register1=XFSM_CUSTOM0, .register2=XFSM_ONE,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_MINUS, .output=XFSM_CUSTOM0};

xfsm_results_t r5[CONDITIONS] = {XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_TRUE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_action_t *a5[7] = {&decReTX, &pktsToPace, &pacing2, &setTimerPac,
                        &lastAcked, &availWin1, &availWin2};

xfsm_table_entry_t e5 = {.results=r5, .actions=a5, .actions_num=7,
        .current_state=1, .event=XFSM_ACK, .next_state=2, .priority=0};

/**
 * Table entry 6
 * POST-RECOVERY
 */

xfsm_results_t r6[CONDITIONS] = {XFSM_FALSE, XFSM_TRUE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_TRUE};

xfsm_action_t *a6[8] = {&sendPkt, &nextTX, &highTX, &pktsToPace, &pacing2, &setTimerPac,
                        &availWin1, &availWin2};

xfsm_table_entry_t e6 = {.results=r6, .actions=a6, .actions_num=8,
        .current_state=2, .event=XFSM_TIMEOUT, .next_state=2, .priority=0};

/**
 * Table entry 7
 * POST RECOVERY -> LINEAR INCREASE
 */

xfsm_results_t r7[CONDITIONS] = {XFSM_FALSE, XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_TRUE};

xfsm_action_t *a7[0] = {};

xfsm_table_entry_t e7 = {.results=r7, .actions=a7, .actions_num=0,
        .current_state=2, .event=XFSM_TIMEOUT, .next_state=3, .priority=0};

/**
 * Table entry 7bis
 * POST RECOVERY -> LINEAR INCREASE
 */

xfsm_results_t r7bis[CONDITIONS] = {XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_action_t *a7bis[3] = {&lastAcked, &availWin1, &availWin2};

xfsm_table_entry_t e7bis = {.results=r7bis, .actions=a7bis, .actions_num=3,
        .current_state=2, .event=XFSM_ACK, .next_state=2, .priority=0};

/**
 * Table entry 8
 * LINEAR INCREASE
 */

xfsm_action_t cwndInc1 = {.type=XFSM_UPDATE, .register1=XFSM_SEGMENTSIZE, .register2=XFSM_SEGMENTSIZE,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_MULTIPLY, .output=XFSM_ARG0};
xfsm_action_t cwndInc2 = {.type=XFSM_UPDATE, .register1=XFSM_ARG0, .register2=XFSM_CWND,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_DIVIDE, .output=XFSM_ARG0};
xfsm_action_t cwndInc3 = {.type=XFSM_UPDATE, .register1=XFSM_CWND, .register2=XFSM_ARG0,
        .register3=XFSM_NULL, .param2=0, .opcode=XFSM_SUM, .output=XFSM_CWND};


xfsm_results_t r8[CONDITIONS] = {XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

xfsm_action_t *a8[13] = {&cwndInc1, &cwndInc2, &cwndInc3, &lastAcked, &doubRtt, &setTimerF, &sendPkt,
                         &updateRTT, &nextTX, &highTX, &setFastTimer, &availWin1, &availWin2};

xfsm_table_entry_t e8 = {.results=r8, .actions=a8, .actions_num=13,
        .current_state=3, .event=XFSM_ACK, .next_state=3, .priority=0};

/**
 * Table entry 9
 * LINEAR INCREASE (TIMEOUT)
 */

xfsm_results_t r9[CONDITIONS] = {XFSM_FALSE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_TRUE, XFSM_DONT_CARE};

xfsm_action_t *a9[8] = {&doubRtt, &sendPkt, &nextTX, &highTX, &setTimerF, &setFastTimer, &availWin1, &availWin2};

xfsm_table_entry_t e9 = {.results=r9, .actions=a9, .actions_num=8,
        .current_state=3, .event=XFSM_TIMEOUT, .next_state=3, .priority=0};

/**
 * Table entry 10
 * LINEAR INCREASE (TIMEOUT)
 */

xfsm_results_t r10[CONDITIONS] = {XFSM_TRUE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_FALSE, XFSM_TRUE,
                                 XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE, XFSM_DONT_CARE};

// a10 is equal to a3: same actions

xfsm_table_entry_t e10 = {.results=r10, .actions=a3, .actions_num=6,
        .current_state=3, .event=XFSM_TIMEOUT, .next_state=1, .priority=0};

/**
 * final entries
 */
xfsm_table_entry_t *entries[TABLE_SIZE] = {&e1,&e2,&e3,&e4,&e5,&e6,&e7,&e7bis,&e8,&e9,&e10};

#endif //ODP_XFSM_TEB_XFSM_H
