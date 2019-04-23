#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/traffic-control-layer.h"


using namespace ns3;


TcpXfsm* configure_xfsm() {
    /****************************************************/
    /* TCP XFSM configuration (sender side)             */
    /****************************************************/

    /*~~~~~~~~~~~~~~~~~~~ Conditions ~~~~~~~~~~~~~~~~~~~*/
    // available window > 0
    Condition c1 = Condition(XFSM_AVAILABLE_WIN, XFSM_ZERO, 0, XFSM_GREATER);
    // is dupack?
    Condition c2 = Condition(XFSM_LASTACKEDSEQ, XFSM_CONTEXT_PARAM0, 0, XFSM_EQUAL);

    // timerEntrySeqNo < lastAckedSeq ?
    Condition c3 = Condition(XFSM_CONTEXT_PARAM0, XFSM_LASTACKEDSEQ, 0, XFSM_LESS);
    // currentReTXRound < timerEntryReTXCount ?
    Condition c4 = Condition(XFSM_CUSTOM0, XFSM_CONTEXT_PARAM1, 0, XFSM_LESS);
    // nexttxseq >= highTXMark ?
    Condition c5 = Condition(XFSM_NEXTTXSEQ, XFSM_HIGHTXMARK, 0, XFSM_GREATER_EQ);
    // ackNo >= HighTxMark
    Condition c6 = Condition(XFSM_CONTEXT_PARAM0, XFSM_HIGHTXMARK, 0, XFSM_GREATER_EQ);
    // nagle modified
    Condition c7 = Condition(XFSM_AVAILABLE_WIN, XFSM_SEGMENTSIZE, 0, XFSM_LESS_EQ);
    // spurious retransmission
    Condition c8 = Condition(XFSM_RCVTIMESTAMPVALUEECHOREPLY, XFSM_CUSTOM7, 0, XFSM_LESS_EQ);
    // spurious in which state? misusing CUSTOM5
    Condition c9 = Condition(XFSM_CUSTOM5, XFSM_ZERO, 0, XFSM_GREATER);
    // valid time?
    Condition c10 = Condition(XFSM_TIMESTAMP, XFSM_CUSTOM2, 0, XFSM_GREATER_EQ);
    // real nagle
    Condition c11 = Condition(XFSM_AVAILABLE_WIN, XFSM_SEGMENTSIZE, 0, XFSM_LESS);

    ConditionList conditionList;
    conditionList.add_element(c1);
    conditionList.add_element(c2);
    conditionList.add_element(c3);
    conditionList.add_element(c4);
    conditionList.add_element(c5);
    conditionList.add_element(c6);
    conditionList.add_element(c7);
    conditionList.add_element(c8);
    conditionList.add_element(c9);
    conditionList.add_element(c10);
    conditionList.add_element(c11);

    /*~~~~~~~~~~~~~~~~~~~ Table entries ~~~~~~~~~~~~~~~~*/

    /*********** TABLE ENTRY 1 (generic timeout) ***********/
    /******* Create state table ********/
    std::vector <TableEntry> table;
  
    // expected results for table entry 1
    std::vector <Results_t> results1;
    results1.push_back(XFSM_TRUE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);

    //update available win
    XfsmAction availWin = XfsmAction(XFSM_MINUS, XFSM_HIGHTXMARK, XFSM_LASTACKEDSEQ, XFSM_AVAILABLE_WIN);
    XfsmAction availWin1 = XfsmAction(XFSM_MINUS, XFSM_CWND, XFSM_AVAILABLE_WIN, XFSM_AVAILABLE_WIN);

    /* TB timer */
    // set the timeout
    XfsmAction setTimer1_1 = XfsmAction(XFSM_SUM, XFSM_RTT, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer1_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequence number
    XfsmAction setTimer1_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer1_4 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer1_5 = XfsmAction(XFSM_SETTIMER);

    /* SendDataPacket */
    XfsmAction send1_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send1_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send1_3 = XfsmAction(XFSM_SENDPCKT);
    XfsmAction updateNextTx = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, XFSM_SEGMENTSIZE, XFSM_NEXTTXSEQ);

    /* Set "fake" timer to schedule next trasmissions, if any */
    XfsmAction setGenTimer1_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 2, XFSM_ARG0); // fake '5us' schedule time
    XfsmAction setGenTimer1_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer1_3 = XfsmAction(XFSM_SETGENERICTIMER);

    /* Create the action list */
    std::vector<XfsmAction> actions1;
    actions1.push_back(setTimer1_1);
    actions1.push_back(setTimer1_2);
    actions1.push_back(setTimer1_3);
    actions1.push_back(setTimer1_4);
    actions1.push_back(setTimer1_5);
    actions1.push_back(send1_1);
    actions1.push_back(send1_2);
    actions1.push_back(send1_3);
    actions1.push_back(updateNextTx);
    actions1.push_back(setGenTimer1_1);
    actions1.push_back(setGenTimer1_2);
    actions1.push_back(setGenTimer1_3);
    actions1.push_back(availWin);
    actions1.push_back(availWin1);
    
    /* Construct the table entry */
    TableEntry te1 = TableEntry(results1, 0, 0, actions1, 10, XFSM_GENERIC_TIMEOUT);

    table.push_back(te1);


    /*********** TABLE ENTRY 2 (ACK) ***********/
    // note: m_tcb->m_lastAckedSequence is updated from outside xfsm, no need to do it here

    // expected results for table entry 2
    std::vector <Results_t> results2;
    results2.push_back(XFSM_DONT_CARE); 
    results2.push_back(XFSM_FALSE);  
    results2.push_back(XFSM_DONT_CARE); 
    results2.push_back(XFSM_DONT_CARE); 
    results2.push_back(XFSM_DONT_CARE); 

    /* Increase window! */
    XfsmAction update2_1 = XfsmAction(XFSM_SUM, XFSM_CWND, XFSM_SEGMENTSIZE, XFSM_CWND);
    XfsmAction updateLastAcked = XfsmAction(XFSM_SUM, XFSM_CONTEXT_PARAM0, XFSM_ZERO, XFSM_LASTACKEDSEQ);

    /* Schedule "fake" timer to schedule next trasmissions, if any */
    XfsmAction setGenTimer2_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 2, XFSM_ARG0); // fake '5us' schedule time
    XfsmAction setGenTimer2_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer2_3 = XfsmAction(XFSM_SETGENERICTIMER);
    XfsmAction updateRTT = XfsmAction(XFSM_MAX, XFSM_RTT, XFSM_CONTEXT_PARAM1, XFSM_RTT);

    std::vector<XfsmAction> actions2;
    actions2.push_back(update2_1);
    actions2.push_back(updateLastAcked);
    actions2.push_back(setGenTimer2_1);
    actions2.push_back(setGenTimer2_2);
    actions2.push_back(setGenTimer2_3);
    actions2.push_back(updateRTT);
    actions2.push_back(availWin);
    actions2.push_back(availWin1);
    

    /* Construct the table entry */
    TableEntry te2 = TableEntry(results2, 0, 0, actions2, 0, XFSM_ACK);

    table.push_back(te2);

    /*********** TABLE ENTRY 3 (TEB timeout): state transition to recovery ***********/

    // expected results for table entry 1
    std::vector <Results_t> results3;
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_FALSE);
    results3.push_back(XFSM_TRUE);
    results3.push_back(XFSM_DONT_CARE); 

    /* Actions */
    // save previous Cwnd to custom 5 for SPURIOUS retx
    XfsmAction saveCwnd = XfsmAction(XFSM_SUM, XFSM_CWND, (uint32_t) 0, XFSM_CUSTOM6);

    // decrease window to cwnd/2
    XfsmAction update3_1 = XfsmAction(XFSM_DIVIDE, XFSM_CWND, (uint32_t) 2, XFSM_CWND);
    // mininum cwnd is segment size
    XfsmAction update3_11 = XfsmAction(XFSM_MAX, XFSM_CWND, XFSM_SEGMENTSIZE, XFSM_CWND);

    // set currentReTXRound++
    XfsmAction update3_2 = XfsmAction(XFSM_SUM, XFSM_CUSTOM0, 1, XFSM_CUSTOM0);

    // calculate the pacing delay
    XfsmAction update3_3 = XfsmAction(XFSM_DIVIDE, XFSM_CWND, XFSM_SEGMENTSIZE, XFSM_CUSTOM1); // cwnd/segSize
    XfsmAction update3_4 = XfsmAction(XFSM_DIVIDE, XFSM_RTT, XFSM_CUSTOM1, XFSM_CUSTOM1); // rtt/ cwnd [pkt]

    // save TIMESTAMP for spurious recovery
    XfsmAction update3_5 = XfsmAction(XFSM_SUM, XFSM_TIMESTAMP, (uint32_t) 0, XFSM_CUSTOM7);
    
    /* Start scheduling the next transmission at pacing time */
    XfsmAction setGenTimer3_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer3_2 = XfsmAction(XFSM_SUM, XFSM_CONTEXT_PARAM0, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer3_3 = XfsmAction(XFSM_SETGENERICTIMER);

    // Save delta in case some timer expires before it has to
    XfsmAction saveDelta1 = XfsmAction(XFSM_DIVIDE, XFSM_CUSTOM1, (uint32_t) 1000, XFSM_CUSTOM2);
    XfsmAction saveDelta2 = XfsmAction(XFSM_SUM, XFSM_TIMESTAMP, XFSM_CUSTOM2, XFSM_CUSTOM2);

    // update next TX sequence
    XfsmAction updateSeqNxt = XfsmAction(XFSM_SUM, XFSM_CONTEXT_PARAM0, 0, XFSM_NEXTTXSEQ);

    // populate the action vector
    std::vector<XfsmAction> actions3;
    actions3.push_back(saveCwnd);
    actions3.push_back(update3_1);
    actions3.push_back(update3_11);
    actions3.push_back(update3_2);
    actions3.push_back(update3_3);
    actions3.push_back(update3_4);
    actions3.push_back(setGenTimer3_1);
    actions3.push_back(setGenTimer3_2);
    actions3.push_back(setGenTimer3_3);
    actions3.push_back(update3_5);
    actions3.push_back(saveDelta1);
    actions3.push_back(saveDelta2);
    actions3.push_back(updateSeqNxt);
    

    /* Construct the table entry */
    TableEntry te3 = TableEntry(results3, 0, 1, actions3, 0, XFSM_TIMEOUT);

    table.push_back(te3);
    

    /*********** TABLE ENTRY 4S (SACK): removing sacked timers **************/

    std::vector<Results_t> results4s;
    results4s.push_back(XFSM_DONT_CARE);
    results4s.push_back(XFSM_TRUE);

    XfsmAction removeTimers0a(XFSM_SUM, XFSM_SACK_LEFT0, 0, XFSM_ARG0);
    XfsmAction removeTimers0b(XFSM_SUM, XFSM_SACK_RIGHT0, 0, XFSM_ARG1);
    XfsmAction removeTimers0c(XFSM_REMOVETIMERS);

    XfsmAction removeTimers1a(XFSM_SUM, XFSM_SACK_LEFT1, 0, XFSM_ARG0);
    XfsmAction removeTimers1b(XFSM_SUM, XFSM_SACK_RIGHT1, 0, XFSM_ARG1);
    XfsmAction removeTimers1c(XFSM_REMOVETIMERS);

    XfsmAction removeTimers2a(XFSM_SUM, XFSM_SACK_LEFT2, 0, XFSM_ARG0);
    XfsmAction removeTimers2b(XFSM_SUM, XFSM_SACK_RIGHT2, 0, XFSM_ARG1);
    XfsmAction removeTimers2c(XFSM_REMOVETIMERS);

    std::vector<XfsmAction> actions4s;
    actions4s.push_back(removeTimers0a);
    actions4s.push_back(removeTimers0b);
    actions4s.push_back(removeTimers0c);
        
    actions4s.push_back(removeTimers1a);
    actions4s.push_back(removeTimers1b);
    actions4s.push_back(removeTimers1c);

    actions4s.push_back(removeTimers2a);
    actions4s.push_back(removeTimers2b);
    actions4s.push_back(removeTimers2c);

    TableEntry te4s = TableEntry(results4s, 1, 1, actions4s, 0, XFSM_SACK);

    table.push_back(te4s);


    /*********** TABLE ENTRY 4teb (Teb Timeout): another Teb timeout ***********/
    // expected results for table entry 1
    std::vector <Results_t> results4t;
    results4t.push_back(XFSM_DONT_CARE);
    results4t.push_back(XFSM_DONT_CARE);
    results4t.push_back(XFSM_FALSE);
    results4t.push_back(XFSM_TRUE);
    results4t.push_back(XFSM_DONT_CARE); 

    // another retransission round: same as table entry 3
    TableEntry te4t = TableEntry(results4t, 1, 1, actions3, 0, XFSM_TIMEOUT);

    table.push_back(te4t);

    /*********** TABLE ENTRY 4 (generic timeo ut): recovery pacing transmission ***********/

    // expected results for table entry 4
    std::vector <Results_t> results4;
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_FALSE); 
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_TRUE);


    /* Actions */
    // retransmit packet
    XfsmAction send4_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send4_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send4_3 = XfsmAction(XFSM_SENDPCKT);

    /* TB timer */
    // set the timeout
    XfsmAction setTimer4_1 = XfsmAction(XFSM_SUM, XFSM_RTT, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer4_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequence number
    XfsmAction setTimer4_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer4_4 = XfsmAction(XFSM_SUM, XFSM_CUSTOM0, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer4_5 = XfsmAction(XFSM_SETTIMER);

    /* Schedule the next transmission at pacing time */
    XfsmAction setGenTimer4_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer4_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer4_3 = XfsmAction(XFSM_SETGENERICTIMER);

    std::vector<XfsmAction> actions4;
    actions4.push_back(send4_1);
    actions4.push_back(send4_2);
    actions4.push_back(send4_3);
    actions4.push_back(setTimer4_1);
    actions4.push_back(setTimer4_2);
    actions4.push_back(setTimer4_3);
    actions4.push_back(setTimer4_4);
    actions4.push_back(setTimer4_5);
    actions4.push_back(updateNextTx);
    actions4.push_back(setGenTimer4_1);
    actions4.push_back(setGenTimer4_2);
    actions4.push_back(setGenTimer4_3);
    actions4.push_back(saveDelta1);
    actions4.push_back(saveDelta2);

    /* Construct the table entry */
    TableEntry te4 = TableEntry(results4, 1, 1, actions4, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te4);


    /*********** TABLE ENTRY 5 (generic timeout): state transition to post recovery ***********/

    // expected results for table entry 5
    
    std::vector <Results_t> results5;
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_TRUE); 
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_TRUE);

    // reset globalReTXRound to 0
    XfsmAction update5_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 0, XFSM_CUSTOM0);

    // calculate the pacing delay
    // saving packets to pace in custom 3, so we can add one more at each scheduled tx to reduce delay
    XfsmAction update5_2 = XfsmAction(XFSM_DIVIDE, XFSM_CWND, XFSM_SEGMENTSIZE, XFSM_CUSTOM3); // cwnd/segSize = pktsToPace
    XfsmAction update5_3 = XfsmAction(XFSM_SUM, XFSM_CUSTOM3, (uint32_t) 1, XFSM_CUSTOM3); // pktsToPace++
    //XfsmAction update5_4 = XfsmAction(XFSM_DIVIDE, XFSM_RTT, XFSM_CUSTOM3, XFSM_CUSTOM1); // rtt/pktsToPace

    /* Schedule the next transmission at pacing time */
    XfsmAction setGenTimer5_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer5_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer5_3 = XfsmAction(XFSM_SETGENERICTIMER);

    /* TB timer */
    // set the timeout
    XfsmAction setTimer5_1 = XfsmAction(XFSM_SUM, XFSM_RTT, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer5_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequence number
    XfsmAction setTimer5_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer5_4 = XfsmAction(XFSM_SUM, XFSM_CUSTOM0, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer5_5 = XfsmAction(XFSM_SETTIMER);

    std::vector<XfsmAction> actions5;
    actions5.push_back(update5_2);
    actions5.push_back(update5_3);
    actions5.push_back(setGenTimer5_1);
    actions5.push_back(setGenTimer5_2);
    actions5.push_back(setGenTimer5_3);
    actions5.push_back(setTimer5_1);
    actions5.push_back(setTimer5_2);
    actions5.push_back(setTimer5_3);
    actions5.push_back(setTimer5_4);
    actions5.push_back(setTimer5_5);
    actions5.push_back(saveDelta1);
    actions5.push_back(saveDelta2);
     

    //Construct the table entry //
    TableEntry te5 = TableEntry(results5, 1, 1, actions5, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te5);


    /*********** TABLE ENTRY 5_2 (ACK): SPURIOUS retransmission, end recovery ***********/
    std::vector <Results_t> results5_2;
    results5_2.push_back(XFSM_DONT_CARE);
    results5_2.push_back(XFSM_DONT_CARE);
    results5_2.push_back(XFSM_DONT_CARE);
    results5_2.push_back(XFSM_DONT_CARE);
    results5_2.push_back(XFSM_TRUE); 
    results5_2.push_back(XFSM_DONT_CARE);
    results5_2.push_back(XFSM_DONT_CARE);
    results5_2.push_back(XFSM_TRUE);
    results5_2.push_back(XFSM_DONT_CARE);

    // cwnd = prevCwnd
    XfsmAction restoreCwnd = XfsmAction(XFSM_SUM, XFSM_CUSTOM6, (uint32_t) 0, XFSM_CWND);
    // restore Timestamp to 0
    XfsmAction restoreTS = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 0, XFSM_CUSTOM7);

    std::vector <XfsmAction> actions5_2;
    actions5_2.push_back(updateLastAcked);
    actions5_2.push_back(update5_1); // dec global retx round
    actions5_2.push_back(update5_2);
    actions5_2.push_back(update5_3);
    actions5_2.push_back(setGenTimer5_1);
    actions5_2.push_back(setGenTimer5_2);
    actions5_2.push_back(setGenTimer5_3);
    actions5_2.push_back(setTimer5_1);
    actions5_2.push_back(setTimer5_2);
    actions5_2.push_back(setTimer5_3);
    actions5_2.push_back(setTimer5_4);
    actions5_2.push_back(setTimer5_5);
    actions5_2.push_back(updateRTT);
    actions5_2.push_back(restoreCwnd);
    actions5_2.push_back(restoreTS);
    actions5_2.push_back(saveDelta1);
    actions5_2.push_back(saveDelta2);
    actions5_2.push_back(availWin);
    actions5_2.push_back(availWin1);
    


    TableEntry te5_2 = TableEntry(results5_2, 1, 2, actions5_2, 0, XFSM_ACK);

    table.push_back(te5_2);


    /*********** TABLE ENTRY 5_1 (ACK): new "good" ack recived, end recovery ***********/

    // expected results for table entry 5
    std::vector <Results_t> results5_1;
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_TRUE); 
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE); 
    results5_1.push_back(XFSM_FALSE); 
    results5_1.push_back(XFSM_DONT_CARE);

    std::vector <XfsmAction> actions5_1;
    actions5_1.push_back(updateLastAcked);
    actions5_1.push_back(update5_1);
    actions5_1.push_back(update5_2);
    actions5_1.push_back(update5_3);
    actions5_1.push_back(setGenTimer5_1);
    actions5_1.push_back(setGenTimer5_2);
    actions5_1.push_back(setGenTimer5_3);
    actions5_1.push_back(setTimer5_1);
    actions5_1.push_back(setTimer5_2);
    actions5_1.push_back(setTimer5_3);
    actions5_1.push_back(setTimer5_4);
    actions5_1.push_back(setTimer5_5);
    actions5_1.push_back(updateRTT);
    actions5_1.push_back(availWin);
    actions5_1.push_back(availWin1);
    

    TableEntry te5_1 = TableEntry(results5_1, 1, 2, actions5_1, 0, XFSM_ACK);

    table.push_back(te5_1);

    /*********** TABLE ENTRY 5s (SACK): new "good" sack recived, end recovery ***********/

    TableEntry te5s = TableEntry(results5_1, 1, 2, actions5_1, 0, XFSM_SACK);

    table.push_back(te5s);


    /*********** TABLE ENTRY 6 (generic timeout): post recovery pacing ***********/

    // expected results for table entry 6
    
    std::vector <Results_t> results6;
    results6.push_back(XFSM_TRUE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE); 
    results6.push_back(XFSM_DONT_CARE); 
    results6.push_back(XFSM_DONT_CARE); 
    results6.push_back(XFSM_TRUE); 
 

    // Actions 
    // transmit packet
    XfsmAction send6_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send6_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send6_3 = XfsmAction(XFSM_SENDPCKT);

    XfsmAction update6_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM3, (uint32_t) 1, XFSM_CUSTOM3); // pktsToPace++
    XfsmAction update6_2 = XfsmAction(XFSM_DIVIDE, XFSM_RTT, XFSM_CUSTOM3, XFSM_CUSTOM1); // rtt/pktsToPace

    /* Schedule the next transmission at pacing time */
    XfsmAction setGenTimer6_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer6_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer6_3 = XfsmAction(XFSM_SETGENERICTIMER);

    /* TB timer */
    // set the timeout
    XfsmAction setTimer6_1 = XfsmAction(XFSM_SUM, XFSM_RTT, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer6_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequence number
    XfsmAction setTimer6_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer6_4 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer6_5 = XfsmAction(XFSM_SETTIMER);


    std::vector<XfsmAction> actions6;
    actions6.push_back(send6_1);
    actions6.push_back(send6_2);
    actions6.push_back(send6_3);
    actions6.push_back(updateNextTx);
    actions6.push_back(update6_1);
    actions6.push_back(update6_2);
    actions6.push_back(setGenTimer6_1);
    actions6.push_back(setGenTimer6_2);
    actions6.push_back(setGenTimer6_3);
    actions6.push_back(setTimer6_1);
    actions6.push_back(setTimer6_2);
    actions6.push_back(setTimer6_3);
    actions6.push_back(setTimer6_4);
    actions6.push_back(setTimer6_5);
    actions6.push_back(saveDelta1);
    actions6.push_back(saveDelta2);
    actions6.push_back(availWin);
    actions6.push_back(availWin1);
    


    //Construct the table entry //
    TableEntry te6 = TableEntry(results6, 2, 2, actions6, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te6);

    /*********** TABLE ENTRY 7 (generic timeout): end post recovery -> goto congestion avoidance ***********/

    // expected results for table entry 7
    
    std::vector <Results_t> results7bis;
    results7bis.push_back(XFSM_DONT_CARE);
    results7bis.push_back(XFSM_FALSE);
    results7bis.push_back(XFSM_DONT_CARE);
    results7bis.push_back(XFSM_DONT_CARE);
    results7bis.push_back(XFSM_DONT_CARE); 
    results7bis.push_back(XFSM_DONT_CARE); 
    results7bis.push_back(XFSM_TRUE); 
    results7bis.push_back(XFSM_DONT_CARE); 
    results7bis.push_back(XFSM_FALSE); 

    // empty: only go in state 0
    std::vector<XfsmAction> actions7bis;

    actions7bis.push_back(updateLastAcked);
    actions7bis.push_back(availWin);
    actions7bis.push_back(availWin1);
    

    //Construct the table entry //
    TableEntry te7bis = TableEntry(results7bis, 2, 0, actions7bis, 0, XFSM_ACK);

    table.push_back(te7bis);

    /*********** TABLE ENTRY 7 (generic timeout): end post recovery -> goto congestion avoidance ***********/

    // expected results for table entry 7
   
    std::vector <Results_t> results7;
    results7.push_back(XFSM_FALSE);
    results7.push_back(XFSM_DONT_CARE);
    results7.push_back(XFSM_DONT_CARE);
    results7.push_back(XFSM_DONT_CARE);
    results7.push_back(XFSM_DONT_CARE); 

    // set spurious flag to 3 
    XfsmAction act = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 3, XFSM_CUSTOM5);

    std::vector<XfsmAction> actions7;
    actions7.push_back(act);

    //Construct the table entry //
    TableEntry te7 = TableEntry(results7, 2, 3, actions7, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te7);

    /*********** TABLE ENTRY 8 (ACK): new ack received! schedule next transmission ***********/

    // expected results for table entry 8
    std::vector <Results_t> results8;
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_FALSE);
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE); 
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE);
    //results8.push_back(XFSM_FALSE);

    // Actions 
    // increase cwnd (AIMD): cwnd += MSS*MSS/cwnd
    XfsmAction update8_1 = XfsmAction(XFSM_MULTIPLY, XFSM_SEGMENTSIZE, XFSM_SEGMENTSIZE, XFSM_CUSTOM4); // MSS*MSS
    XfsmAction update8_2 = XfsmAction(XFSM_DIVIDE, XFSM_CUSTOM4, XFSM_CWND, XFSM_CUSTOM4); // MSS^2/cwnd
    XfsmAction update8_3 = XfsmAction(XFSM_SUM, XFSM_CWND, XFSM_CUSTOM4, XFSM_CWND);

    /* Schedule "fake" timer to schedule next trasmissions, if any */
    XfsmAction setGenTimer8_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 2, XFSM_ARG0); // fake '5us' schedule time
    XfsmAction setGenTimer8_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer8_3 = XfsmAction(XFSM_SETGENERICTIMER);

    /* TB timer */
    // set the timeout
    XfsmAction setTimer8_1 = XfsmAction(XFSM_SUM, XFSM_RTT, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer8_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequencenumber
    XfsmAction setTimer8_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer8_4 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer8_5 = XfsmAction(XFSM_SETTIMER);

    // transmit packet
    XfsmAction send8_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send8_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send8_3 = XfsmAction(XFSM_SENDPCKT);


    std::vector<XfsmAction> actions8;
    actions8.push_back(update8_1);
    actions8.push_back(update8_2);
    actions8.push_back(update8_3);
    actions8.push_back(updateLastAcked);
    actions8.push_back(setTimer8_1);
    actions8.push_back(setTimer8_2);
    actions8.push_back(setTimer8_3);
    actions8.push_back(setTimer8_4);
    actions8.push_back(setTimer8_5);
    actions8.push_back(setGenTimer8_1);
    actions8.push_back(setGenTimer8_2);
    actions8.push_back(setGenTimer8_3);
    actions8.push_back(send8_1);
    actions8.push_back(send8_2);
    actions8.push_back(send8_3);
    actions8.push_back(updateRTT);
    actions8.push_back(updateNextTx);
    actions8.push_back(availWin);
    actions8.push_back(availWin1);
    

    //Construct the table entry //
    TableEntry te8 = TableEntry(results8, 3, 3, actions8, 0, XFSM_ACK);

    table.push_back(te8);

   
    /*********** TABLE ENTRY 9 (generic timeout) ***********/

    // expected results for table entry 9
    std::vector <Results_t> results9;
    results9.push_back(XFSM_TRUE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE); 
    results9.push_back(XFSM_DONT_CARE); 
    results9.push_back(XFSM_DONT_CARE); 
    results9.push_back(XFSM_DONT_CARE); 
    results9.push_back(XFSM_DONT_CARE); 
    results9.push_back(XFSM_DONT_CARE); 
    results9.push_back(XFSM_FALSE); 

    //Construct the table entry //
    // no need for actions!!! same as the first table entry
    TableEntry te9 = TableEntry(results9, 3, 3, actions1, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te9);


    /*********** TABLE ENTRY 10 (teb timeout)  go to recovery***********/

    // expected results for table entry 10
    std::vector <Results_t> results10;
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_FALSE);
    results10.push_back(XFSM_TRUE);
    results10.push_back(XFSM_DONT_CARE); 

    //Construct the table entry //
    // no need for actions!!! same as the third table entry
    TableEntry te10 = TableEntry(results10, 3, 1, actions3, 0, XFSM_TIMEOUT);

    table.push_back(te10);

    TcpXfsm *xfsm = new TcpXfsm(table, conditionList, 0);

    return xfsm;

}