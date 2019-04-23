#include <ns3/simulator.h>
#include "tcp-xfsm.h"

/*
 * XFSM CONSTRUCTORS
 */

namespace ns3{


TcpXfsm::TcpXfsm(std::vector<TableEntry> tableEntryList,
                 ConditionList conditionList, uint32_t current_state) :
        tableEntryList(tableEntryList), conditionList(conditionList), current_state(
        current_state) {
            int dim = conditionList.getSize();

            fprintf(stderr, "size is %d\n", dim);

            condition_results = (bool *) malloc(sizeof(bool)*dim);

            if(condition_results == nullptr)
            {
                fprintf(stderr, "[FATAL ERROR] Error in malloc (TcpXfsm::TcpXfsm)\n");
                exit(1);
            }
        }

TableEntry::TableEntry() {}

/* Table entry */
TableEntry::TableEntry(std::vector<Results_t> expected_condition_result, uint32_t current_state,
                       uint32_t nextState, std::vector<XfsmAction> actions, uint16_t priority, Event_t event)
        : expected_condition_result(expected_condition_result),
          current_state(current_state),
          nextState(nextState),
          actions(actions),
          priority(priority),
          event(event) {}

/* Actions */
XfsmAction::XfsmAction(Opcodes_t opcode, Registers_t param1_reg, uint32_t param2, Registers_t output) :
        actionType(XFSM_UPDATE),
        opcode(opcode),
        param1_reg(param1_reg),
        param2(param2),
        output(output) {}

XfsmAction::XfsmAction(Opcodes_t opcode, Registers_t param1_reg, Registers_t param2_reg, Registers_t output) :
        actionType(XFSM_UPDATE),
        opcode(opcode),
        param1_reg(param1_reg),
        param2_reg(param2_reg),
        param2(0),
        output(output) {}

XfsmAction::XfsmAction(Actions_t type) :
        actionType(type) {}

XfsmAction::XfsmAction(Actions_t type, uint32_t nextState) :
        actionType(type),
        nextState (nextState) {}

XfsmAction::XfsmAction(){}

Condition::Condition(Registers_t register1, Registers_t register2, uint32_t constant, Conditions_t opcode) :
        register1(register1),
        register2(register2),
        constant(constant),
        opcode(opcode) {}

Condition::Condition() {}



ConditionList::ConditionList()
{}


uint32_t TableEntry::getEntryNextState(){
    return this->nextState;
}

uint32_t TableEntry::getEntryCurrentState(){
    return this->current_state;
}

// Extended Finite State Machine functions

std::vector<Actions_t> TcpXfsm::getPendingActions()
{
    return pendingActions;
}

void TcpXfsm::addEntry(TableEntry entry) {
    this->tableEntryList.push_back(entry);
}

void TcpXfsm::configureCondition(Condition condition) {
    this->conditionList.add_element(condition);
}

TableEntry * TcpXfsm::tableLookup() {
    std::vector<TableEntry>::iterator it;

    for (it = this->tableEntryList.begin(); it != this->tableEntryList.end(); ++it) {
        if(this->event != it->event)
            continue;
        std::vector<Results_t> condition_values = it->getConditionsValue();
        bool match = true;
        for (unsigned k=0; k < condition_values.size(); k++){
           //if (condition_values[k]==XFSM_DONT_CARE)
              //  continue;
            // horrible... we have to change it
            if ( ( (this->condition_results[k] && condition_values[k]==XFSM_TRUE)
                || (!this->condition_results[k] && condition_values[k]==XFSM_FALSE)
                || condition_values[k] == XFSM_DONT_CARE )
                && current_state == it->getEntryCurrentState())
                continue;
            else {
                match = false;
                break;
            }
        }
        if (match)
            return &(*it);
    }
    return nullptr;
}
/*
void TcpXfsm::execute(TableEntry * entry) {
    std::vector<XfsmAction>::iterator it;
    for (it = entry->getActions().begin(); it != entry->getActions().end(); ++it ){
        it->executeAction();
    }
}
*/

uint32_t TcpXfsm::getRegister(Registers_t reg_num) {
    return registers[reg_num];
}

void TcpXfsm::setRegister(Registers_t reg_num, uint32_t value) {
    registers[reg_num] = value;
}

void TcpXfsm::setTimer(uint32_t seqNumber, uint32_t retrNumber, uint32_t timeout){
    struct Xfsm_Timer timer;
    timer.seqNumber = seqNumber;
    timer.retrNumber = retrNumber;
    timerList.push_back(timer);
}

bool compare (const TableEntry& a, const TableEntry& b) {
    return a.priority < b.priority;
}

void TcpXfsm::sortTableEntries() {
    //this->tableEntryList.sort(compare);
}

void TcpXfsm::evaluateConditions() {
    std::vector<Condition> vector = conditionList.vector;

    uint32_t param1, param2;

    unsigned int i = 0;
    fprintf(stderr, "@@@@@@ Condition evaluation:\n");
    for(i = 0; i < vector.size(); ++i) {
        
        param1 = registers[vector[i].register1];
        param2 = vector[i].register2 != XFSM_NULL ? registers[vector[i].register2] : vector[i].constant;

        if (vector[i].register2 == XFSM_ZERO)
            param2 = 0;

        fprintf(stderr, "condition[%d] -> param1: %d, param2: %d\n", i, param1, param2 );

        switch(vector[i].opcode)
        {
        case(XFSM_LESS):
            condition_results[i] = param1 < param2;
            break;
        case(XFSM_LESS_EQ):
            condition_results[i] = param1 <= param2;
            break;
        case(XFSM_GREATER):
            condition_results[i] = param1 > param2;
            break;
        case(XFSM_GREATER_EQ):
            condition_results[i] = param1 >= param2;
            break;
        case(XFSM_EQUAL):
            condition_results[i] = param1 == param2;
            break;
        case(XFSM_NOT_EQUAL):
            condition_results[i] = param1 != param2;
            break;
        default:
            condition_results[i] = false;
            break; // Some warning log
        }        
    }
}

struct Xfsm_Timer TcpXfsm::getTimer(uint32_t seqNumber) 
{    
    unsigned int i = 0;
    for(i = 0; i < timerList.size(); ++i) 
    {
        if(timerList[i].seqNumber == seqNumber) {
            fprintf(stderr, "TIMER FOUND\n");
            return timerList[i];      
        }
    }
    struct Xfsm_Timer negativeTimer;
    negativeTimer.seqNumber = 0;
    negativeTimer.retrNumber = 1;
    return negativeTimer; // strange situation, negative timer is simply used to manage the case in which nothing was found
}

void TcpXfsm::xfsmWakeup(ns3::Event_t event, uint32_t param0, uint32_t param1)
{
    this->event = event;
    if(event == XFSM_TIMEOUT)
    {   
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!TIMER EXPIRED with seqnum =%d\n", param0);
    	Xfsm_Timer timer = getTimer(param0);
    	registers[XFSM_CONTEXT_PARAM0] = timer.seqNumber;
    	if (timer.seqNumber == 0)
    		return; // No timer found. It happen because it has been removed before it fired.
                    // Just return and the event will be ignored, like it never happened ;) 
        registers[XFSM_CONTEXT_PARAM1] = timer.retrNumber;
    }
    else if (event == XFSM_GENERIC_TIMEOUT)
    {
    	registers[XFSM_CONTEXT_PARAM0] = param0;
    }
    else if (event == XFSM_ACK || event == XFSM_SYN || event == XFSM_SYNACK || event == XFSM_FIN || event == XFSM_FINACK){
        registers[XFSM_CONTEXT_PARAM0] = param0;
        registers[XFSM_CONTEXT_PARAM1] = param1;
    }
    else if (event == XFSM_SACK)
    {
        registers[XFSM_CONTEXT_PARAM0] = param0;
    }
    else if (event == XFSM_EMPTYPACKET)
    {
        registers[XFSM_CONTEXT_PARAM0] = param0;
        if (param1 != 0)
            registers[XFSM_CONTEXT_PARAM1] = param1; // we must check if the RTT is != 0!!
        //TODO: open question: what can we do with acknum and rtt here??
    }
    else if (event == XFSM_CONNECT) {
    }
    else if (event == XFSM_CLOSE) {
        fprintf(stderr, "Time to close our socket\n");

    }

    // in case of XFSM_MOVE_STATE and XFSM_CLOSE, no parameters shoul be passed
    evaluateConditions();
}

/*void TcpXfsm::xfsmWakeup(ns3::Event_t event, uint32_t param0, uint32_t param1)
{
    this->event = event;

    if(event == XFSM_ACK)
    {
        registers[XFSM_CONTEXT_PARAM0] = param0;
        registers[XFSM_CONTEXT_PARAM1] = param1;
    }

    evaluateConditions();
}*/

void TcpXfsm::removeTimer(uint32_t seqNumber) 
{
    unsigned int i = 0;
    for(i = 0; i < timerList.size(); ++i) 
    {
        if(timerList[i].seqNumber == seqNumber)
            timerList.erase(timerList.begin() + i);   
    }
}

void TcpXfsm::setSackBlocks(uint32_t *left, uint32_t *right) 
{
    registers[XFSM_SACK_LEFT0] = left[0];
    registers[XFSM_SACK_LEFT1] = left[1];
    registers[XFSM_SACK_LEFT2] = left[2];
    registers[XFSM_SACK_RIGHT0] = right[0];
    registers[XFSM_SACK_RIGHT1] = right[1];
    registers[XFSM_SACK_RIGHT2] = right[2];
}

// Table Entry

std::vector<XfsmAction> TableEntry::getActions() {
    return this->actions;
}

std::vector<Results_t> TableEntry::getConditionsValue() {
    return this->expected_condition_result;
}


uint32_t XfsmAction::update(uint32_t *registers)
{
    uint32_t result = 0;
    uint32_t param1 = (param1_reg != XFSM_ZERO) ? registers[param1_reg] : 0;
    param2 = (param2_reg == XFSM_NULL) ? param2 : registers[param2_reg];

    switch(opcode)
    {
        case(XFSM_SUM):
            result = param1 + param2;
            break;
        case(XFSM_MINUS):
            result = param1 - param2;
            break;
        case(XFSM_MULTIPLY):
            result = param1 * param2;
            break;
        case(XFSM_DIVIDE):
            result = param1 / param2;
            break;
        case (XFSM_MODULO):
            result = param1 % param2;
            break;
        case(XFSM_MAX):
            result = param1 > param2 ? param1 : param2;
            break;
        case(XFSM_MIN):
            result = param1 < param2 ? param1 : param2;
            break;
        case(XFSM_OR):
            result = param1 | param2;
            break;
        case(XFSM_AND):
            result = param1 & param2;
            break;
        default:
            break; // Some warning log
    }
    return result; // Xfsm register update
}


Actions_t XfsmAction::getActionType()
{
    return actionType;
}


//Condition vector functions

void ConditionList::add_element(Condition c) {
    vector.push_back(c);
}


int ConditionList::getSize()
{
    return vector.size();
}


void TcpXfsm::setTcpFlags(TcpHeader tcpHeader) {
    fprintf(stderr, "register is in%d\n", XFSM_TCPFLAGS);
    registers[XFSM_TCPFLAGS] = tcpHeader.GetFlags();
}

// Condition functions


/*bool Condition::evaluate()
{
    switch(this->opcode)
    {
        case(XFSM_LESS):
            return param1 < param2;

        case(XFSM_LESS_EQ):
            return param1 <= param2;

        case(XFSM_GREATER):
            return param1 > param2;

        case(XFSM_GREATER_EQ):
            return param1 >= param2;

        case(XFSM_EQUAL):
            fprintf(stderr, "param1: %d, param2: %d\n", param1, param2 );
            return param1 == param2;

        case(XFSM_NOT_EQUAL):
            return param1 != param2;
        default:
            break; // Some warning log
    }
    return false;
}*/

}