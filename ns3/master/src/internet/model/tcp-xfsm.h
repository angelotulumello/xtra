#ifndef TCP_XFSM_H
#include "ns3/object.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#define TCP_XFSM_H

#define REG_NUMBER 32

namespace ns3{

typedef enum {
    XFSM_NULL,
    XFSM_CWND,
    XFSM_SSTHRESH,
    XFSM_SEGMENTSIZE,
    XFSM_LASTACKEDSEQ,
    XFSM_HIGHTXMARK,
    XFSM_NEXTTXSEQ,
    XFSM_RCVTIMESTAMPVALUE,
    XFSM_RCVTIMESTAMPVALUEECHOREPLY,
    XFSM_RTT,
    XFSM_AVAILABLE_WIN,
    XFSM_CUSTOM0,
    XFSM_CUSTOM1,
    XFSM_CUSTOM2,
    XFSM_CUSTOM3,
    XFSM_CUSTOM4,
    XFSM_CUSTOM5,
    XFSM_CUSTOM6,
    XFSM_CUSTOM7,
    XFSM_ARG0, // our three arguments used for passing parameters to an action
    XFSM_ARG1,
    XFSM_ARG2,
    XFSM_CONTEXT_PARAM0,
    XFSM_CONTEXT_PARAM1,
    XFSM_SACK_LEFT0,
    XFSM_SACK_LEFT1,
    XFSM_SACK_LEFT2,
    XFSM_SACK_RIGHT0,
    XFSM_SACK_RIGHT1,
    XFSM_SACK_RIGHT2,
    XFSM_TIMESTAMP,
    XFSM_ZERO
} Registers_t;

typedef enum {
    XFSM_SUM,
    XFSM_MINUS,
    XFSM_MULTIPLY,
    XFSM_DIVIDE,
    XFSM_MODULO,
    XFSM_MAX,
    XFSM_MIN
} Opcodes_t;

typedef enum {
    XFSM_LESS,
    XFSM_LESS_EQ,
    XFSM_GREATER,
    XFSM_GREATER_EQ,
    XFSM_EQUAL,
    XFSM_NOT_EQUAL
} Conditions_t;

typedef enum {
    XFSM_UPDATE=0,
    XFSM_SENDPCKT,
    XFSM_SENDBUFF,
    XFSM_SETTIMER,
    XFSM_SETGENERICTIMER,
    XFSM_REMOVETIMER,
    XFSM_REMOVETIMERS
} Actions_t;

typedef enum {
    XFSM_TRUE = 0,
    XFSM_FALSE,
    XFSM_DONT_CARE
} Results_t;

typedef enum {
    XFSM_ACK = 0,
    XFSM_TIMEOUT,
    XFSM_GENERIC_TIMEOUT,
    XFSM_SACK
} Event_t;

struct Xfsm_Timer {
    uint32_t seqNumber;
    uint32_t retrNumber;
};


// in the update function I need a referece to the TCB
class XfsmAction{
private:
    /*
     * UPDATE ACTION SYNTAX:
     * <first_operand><operation><second_operand><return_register>
     *  - first_operand must be a Registers_t
     *  - second_operand can be a Registers_t or a constant (disjunctive or!)
     *  - operation must be Opcodes_t
     *  - return_register must be a Registers_t
     */
public:
    Actions_t actionType;
    Opcodes_t opcode;
    Registers_t param1_reg = XFSM_NULL;
    Registers_t param2_reg = XFSM_NULL;
    uint32_t param2;

    Registers_t output;
    uint32_t nextState;

    void executeAction();
    uint32_t update(uint32_t *registers);
    Actions_t getActionType();


    XfsmAction();
    XfsmAction(Actions_t type);
    XfsmAction(Opcodes_t opcode, Registers_t param1_reg, uint32_t param2, Registers_t output);
                                                            // constructor for the update function
                                                           // if the second operator is a constant
                                                           // set value in param2 and put param2_reg to null
    XfsmAction(Opcodes_t opcode, Registers_t param1_reg,
               Registers_t param2_reg, Registers_t output);
    XfsmAction(Actions_t type, uint32_t nextState);
};

class TableEntry {
private:

public:
    std::vector<Results_t> expected_condition_result;
    uint32_t current_state;
    uint32_t nextState;
    std::vector<XfsmAction> actions;

    uint16_t priority;
    std::vector<XfsmAction> getActions();
    Event_t event;

    std::vector<Results_t> getConditionsValue();

    uint32_t getEntryNextState();

    uint32_t getEntryCurrentState();

    TableEntry(std::vector<Results_t> expected_condition_result, uint32_t current_state, uint32_t nextState,
               std::vector<XfsmAction> actions, uint16_t priority, Event_t event);

    TableEntry();
};

class Condition {
public:
    Registers_t register1, register2;
    uint32_t constant;
    Conditions_t opcode;
    Condition(Registers_t register1, Registers_t register2, uint32_t constant, Conditions_t opcode);
    Condition();
};


class ConditionList {
public:
    ConditionList();

    std::vector<Condition> vector;

    void add_element(Condition c);

    int getSize();


};


class TcpXfsm : public Object {
public:
    uint32_t registers[REG_NUMBER]; // uint32_t or pointer to them
    std::vector<TableEntry> tableEntryList;
    ConditionList conditionList;
    uint32_t current_state;
    std::vector<Actions_t> pendingActions;
    std::vector<struct Xfsm_Timer> timerList;
    Event_t event;
    // functions

    std::vector<Actions_t> getPendingActions();
    
    void addEntry(TableEntry entry);

    void configureCondition(Condition condition);

    TableEntry * tableLookup();

    uint32_t getRegister(Registers_t reg_num); // to get the value of a register

    void setRegister(Registers_t reg_num, uint32_t value); // to modify value of a register

    void sortTableEntries();

    void evaluateConditions();

    // when an event wakes the state machine
    //void xfsmWakeUp(Event_t event, uint32_t param);
    void xfsmWakeup(Event_t event, uint32_t param0, uint32_t param1);
    void setSackBlocks(uint32_t *left, uint32_t *right);

    // constructors
    TcpXfsm(std::vector<TableEntry> tableEntryList,
            ConditionList conditionList, uint32_t current_state);

    void setTimer(uint32_t seqNumber, uint32_t retrNumber, uint32_t expireTime);

    struct Xfsm_Timer getTimer(uint32_t seqNumber);

    void removeTimer(uint32_t seqNumber);

    void removeTimers(uint32_t from, uint32_t to);
private:
    bool *condition_results;
};

}
#endif /* TCP_XFSM_H */