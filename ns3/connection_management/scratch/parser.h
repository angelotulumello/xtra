#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

#include "src/internet/model/tcp-xfsm.h"

using json=nlohmann::json;

namespace ns3 {


    Event_t string_to_event(char vect[]) {


        if (strcmp(vect, "XFSM_ACK") == 0)
            return XFSM_ACK;
        if (strcmp(vect, "XFSM_TIMEOUT") == 0)
            return XFSM_TIMEOUT;
        if (strcmp(vect, "XFSM_GENERIC_TIMEOUT") == 0)
            return XFSM_GENERIC_TIMEOUT;
        if (strcmp(vect, "XFSM_SACK") == 0)
            return XFSM_SACK;
        if (strcmp(vect, "XFSM_EMPTYPACKET") == 0)
            return XFSM_EMPTYPACKET;
        if (strcmp(vect, "XFSM_CONNECT") == 0)
            return XFSM_CONNECT;
        if (strcmp(vect, "XFSM_SYN") == 0)
            return XFSM_SYN;
        if (strcmp(vect, "XFSM_SYNACK") == 0)
            return XFSM_CLOSE;
        if (strcmp(vect, "XFSM_SYNACK") == 0)
            return XFSM_CLOSE;
        if (strcmp(vect, "XFSM_FIN") == 0)
            return XFSM_FIN;
        if (strcmp(vect, "XFSM_FINACK") == 0)
            return XFSM_FINACK;

        else {
            perror("An event was not recognized. Please check your XFSM");
            exit(1);
        }

    }


     Conditions_t string_to_condition(char vect[]) {
        
        if (strcmp(vect, "XFSM_LESS") == 0)
            return XFSM_LESS;
        if (strcmp(vect, "XFSM_LESS_EQ") == 0)
            return XFSM_LESS_EQ;
        if (strcmp(vect, "XFSM_GREATER") == 0)
            return XFSM_GREATER;
        if (strcmp(vect, "XFSM_GREATER_EQ") == 0)
            return XFSM_GREATER_EQ;
        if (strcmp(vect, "XFSM_EQUAL") == 0)
            return XFSM_EQUAL;
        if (strcmp(vect, "XFSM_NOT_EQUAL") == 0)
            return XFSM_NOT_EQUAL;
        else {
            perror("A condition operator is not verified! Please check your XFSM");
            exit(1);
        }
    }


    Registers_t string_to_register(char vect[]) {
        
        if (strcmp(vect, "XFSM_NULL")==0)
            return XFSM_NULL;
        if (strcmp(vect, "XFSM_CWND")==0)
            return XFSM_CWND;
        if (strcmp(vect, "XFSM_SSTHRESH")==0)
            return XFSM_SSTHRESH;
        if (strcmp(vect, "XFSM_SEGMENTSIZE")==0)
            return XFSM_SEGMENTSIZE;
        if (strcmp(vect, "XFSM_LASTACKEDSEQ")==0)
            return XFSM_LASTACKEDSEQ;
        if (strcmp(vect, "XFSM_HIGHTXMARK")==0)
            return XFSM_HIGHTXMARK;
        if (strcmp(vect, "XFSM_NEXTTXSEQ")==0)
            return XFSM_NEXTTXSEQ;
        if (strcmp(vect, "XFSM_RCVTIMESTAMPVALUE")==0)
            return XFSM_RCVTIMESTAMPVALUE;
        if (strcmp(vect, "XFSM_RCVTIMESTAMPVALUEECHOREPLY")==0)
            return XFSM_RCVTIMESTAMPVALUEECHOREPLY;
        if (strcmp(vect, "XFSM_RTT")==0)
            return XFSM_RTT;
        if (strcmp(vect, "XFSM_AVAILABLE_WIN")==0)
            return XFSM_AVAILABLE_WIN;
        if (strcmp(vect, "XFSM_CUSTOM0")==0)
            return XFSM_CUSTOM0;
        if (strcmp(vect, "XFSM_CUSTOM1")==0)
            return XFSM_CUSTOM1;
        if (strcmp(vect, "XFSM_CUSTOM2")==0)
            return XFSM_CUSTOM2;
        if (strcmp(vect, "XFSM_CUSTOM3")==0)
            return XFSM_CUSTOM3;
        if (strcmp(vect, "XFSM_CUSTOM4")==0)
            return XFSM_CUSTOM4;
        if (strcmp(vect, "XFSM_CUSTOM5")==0)
            return XFSM_CUSTOM5;
        if (strcmp(vect, "XFSM_CUSTOM6")==0)
            return XFSM_CUSTOM6;
        if (strcmp(vect, "XFSM_CUSTOM7")==0)
            return XFSM_CUSTOM7;
        if (strcmp(vect, "XFSM_ARG0")==0)
            return XFSM_ARG0;
        if (strcmp(vect, "XFSM_ARG1")==0)
            return XFSM_ARG1;
        if (strcmp(vect, "XFSM_ARG2")==0)
            return XFSM_ARG2;
        if (strcmp(vect, "XFSM_CONTEXT_PARAM0")==0)
            return XFSM_CONTEXT_PARAM0;
        if (strcmp(vect, "XFSM_CONTEXT_PARAM1")==0)
            return XFSM_CONTEXT_PARAM1;
        if (strcmp(vect, "XFSM_SACK_LEFT0")==0)
            return XFSM_SACK_LEFT0;
        if (strcmp(vect, "XFSM_SACK_LEFT1")==0)
            return XFSM_SACK_LEFT1;
        if (strcmp(vect, "XFSM_SACK_LEFT2")==0)
            return XFSM_SACK_LEFT2;
        if (strcmp(vect, "XFSM_SACK_RIGHT0")==0)
            return XFSM_SACK_RIGHT0;
        if (strcmp(vect, "XFSM_SACK_RIGHT1")==0)
            return XFSM_SACK_RIGHT1;
        if (strcmp(vect, "XFSM_SACK_RIGHT2")==0)
            return XFSM_SACK_RIGHT2;
        if (strcmp(vect, "XFSM_TCPFLAGS")==0)
            return XFSM_TCPFLAGS;
        if (strcmp(vect, "XFSM_SOCKETISCLOSED")==0)
            return XFSM_SOCKETISCLOSED;
        if (strcmp(vect, "XFSM_TIMESTAMP")==0)
            return XFSM_TIMESTAMP;
        if (strcmp(vect, "XFSM_BYTESTOTRANSMIT")==0)
            return XFSM_BYTESTOTRANSMIT;
        if (strcmp(vect, "XFSM_ZERO")==0)
            return XFSM_ZERO;
        
        else {
            perror("A register was not recognized. Please check your XFSM");
            exit(1);
        }
    }

    Actions_t string_to_action(char action_type[])
    {
        if (strcmp(action_type, "XFSM_SENDPCKT") == 0)
            return XFSM_SENDPCKT;
        else if (strcmp(action_type, "XFSM_SENDBUFF") == 0)
            return XFSM_SENDBUFF;
        else if (strcmp(action_type, "XFSM_SETTIMER") == 0)
            return XFSM_SETTIMER;
        else if (strcmp(action_type, "XFSM_SETGENERICTIMER") == 0)
            return XFSM_SETGENERICTIMER;
        else if (strcmp(action_type, "XFSM_REMOVETIMER") == 0)
            return XFSM_REMOVETIMER;
        else if (strcmp(action_type, "XFSM_REMOVETIMERS") == 0)
            return XFSM_REMOVETIMERS;
        else if (strcmp(action_type, "XFSM_SENDEMPTYPACKET") == 0)
            return XFSM_SENDEMPTYPACKET;
        else if (strcmp(action_type, "XFSM_CLOSESOCKET") == 0)
            return XFSM_CLOSESOCKET;
        else
            return XFSM_UPDATE;
    }

    Opcodes_t string_to_opcode(char action_type[])
    {        
        Opcodes_t opcode;
        if (strcmp(action_type, "XFSM_SUM") == 0)
            opcode = XFSM_SUM;
        else if (strcmp(action_type, "XFSM_MINUS") == 0)
            opcode = XFSM_MINUS;
        else if (strcmp(action_type, "XFSM_MULTIPLY") == 0)
            opcode = XFSM_MULTIPLY;
        else if (strcmp(action_type, "XFSM_DIVIDE") == 0)
            opcode = XFSM_DIVIDE;
        else if (strcmp(action_type, "XFSM_MODULO") == 0)
            opcode = XFSM_MODULO;
        else if (strcmp(action_type, "XFSM_MAX") == 0)
            opcode = XFSM_MAX;
        else if (strcmp(action_type, "XFSM_MIN") == 0)
            opcode = XFSM_MIN;
        else if (strcmp(action_type, "XFSM_AND") == 0)
            opcode = XFSM_AND;
        else if (strcmp(action_type, "XFSM_OR") == 0)
            opcode = XFSM_OR;
        else
        {
            perror("An action was not recognized. Please check your XFSM");
            exit(1);
        }

        return opcode;      
    }

    Flags_t string_to_flag(char flag[])
    {
        if (strcmp(flag, "XFSM_FLAG_SYN") == 0)
            return XFSM_FLAG_SYN;
        if (strcmp(flag, "XFSM_FLAG_ACK") == 0)
            return XFSM_FLAG_ACK;
        if (strcmp(flag, "XFSM_FLAG_FIN") == 0)
            return XFSM_FLAG_FIN;
        if (strcmp(flag, "XFSM_FLAG_RST") == 0)
            return XFSM_FLAG_RST;
        else {
            perror("error: the flag specified was not correct");
            exit(1);
        }
    }




    void from_json(const json& j, Condition& c) {
        c.register1 = string_to_register((char *) j.at("op0").get<std::string>().c_str());
        std::string reg2 = j.at("op1").get<std::string>();

        char* p;
        long converted = strtol(reg2.c_str(), &p, 10);
        if (*p) {
            // conversion failed because the input wasn't a number
            c.register2=string_to_register((char *)reg2.c_str());
        }
        else {
            // use converted
            c.constant = (uint32_t) converted;
        }
        c.opcode = string_to_condition((char *)j.at("opcode").get<std::string>().c_str());

    }

    std::vector<Results_t> getConditions(const char *input) {
        std::vector<Results_t> list;
        unsigned int i;
        for (i = 0; i<strlen(input); i++) {
            
            if(input[i]== 'T') {
                list.push_back(XFSM_TRUE);
            }
            if(input[i]== 'F') {
                list.push_back(XFSM_FALSE);
            }
            else
                list.push_back(XFSM_DONT_CARE);
        }
        return list;
    }

   /* template <typename TableEntry>
    struct adl_serializer<TableEntry> {
        // note: the return type is no longer 'void', and the method only takes
        // one argument
        static TableEntry from_json(const json& j) {
            std::string cond_string = j.at("cond_results").get<std::string>();
            std::vector<Results_t> expected_condition_result = getConditions(cond_string.c_str());
            uint32_t current_state= j.at("state").get<uint32_t>();
            uint32_t next_state = j.at("next_state").get<uint32_t>();
            std::string actions_string = j.at("actions").get<std::string>();
            std::vector<XfsmAction> actions = getActions(actions_string);
            Event_t event = string_to_event((char *)j.at("event").get<std::string>().c_str());

            return TableEntry(expected_condition_result, current_state, next_state, actions, 0, event);

        }
        
        // Here's the catch! You must provide a to_json method! Otherwise you
        // will not be able to convert move_only_type to json, since you fully
        // specialized adl_serializer on that type
        static void to_json(json& j, TableEntry t) {
            j = NULL;
        }
    }; */



     void to_json(json& j, const TableEntry& te) {
        return;
    }

    void from_json(const json& j, XfsmAction& a)
    {
        fprintf(stderr, "get XfsmAction\n");
        Actions_t type = string_to_action((char *) j.at("type").get<std::string>().c_str());

        a.actionType = type;

        if(type == XFSM_UPDATE)
        {
            Opcodes_t opcode = string_to_opcode((char *) j.at("type").get<std::string>().c_str());
            a.opcode = opcode;

            a.param1_reg = string_to_register((char *) j.at("op0").get<std::string>().c_str());
            std::string reg2 = j.at("op1").get<std::string>();

            char* p;
            long converted = strtol(reg2.c_str(), &p, 10);
            if (*p) {
                // conversion failed because the input wasn't a number
                if (strncmp(p, "XFSM_FLAG", 9)==0)
                    //this means that the 2nd parameter is a flag
                    a.param2= (uint32_t) string_to_flag((char *)reg2.c_str());
                else {
                    a.param2_reg=string_to_register((char *)reg2.c_str());
                    a.param2 = 0;
                }
            }
            else {
                // use converted
                a.param2 = (uint32_t) converted;
            }
            a.output = string_to_register((char *) j.at("dest").get<std::string>().c_str());
        }
    }


    void from_json(const json& j, TableEntry& te) {
        
        std::string cond_string = j.at("condition_results").get<std::string>();
        te.expected_condition_result = getConditions(cond_string.c_str());

        te.current_state= (uint32_t)atoi(j.at("state").get<std::string>().c_str());

        te.nextState = (uint32_t) atoi(j.at("next_state").get<std::string>().c_str());

        std::vector<json> actions = j["actions"];
        std::vector <XfsmAction> actionsList;
        for (unsigned int m = 0; m<actions.size();m++) {
            actionsList.push_back(actions[m].get<XfsmAction>());
        }

        te.actions = actionsList;
        te.event = string_to_event((char *)j.at("event").get<std::string>().c_str());
    }

    TcpXfsm* getXfsm(const char *filename) {
        //open and create the json object
        //system("pwd");
        fprintf(stderr, "ADDING CONDIITON0\n");
        std::ifstream i(filename);
        json j;
        i >> j;

        ConditionList cond_list;
        std::vector <TableEntry> table_vector;

        fprintf(stderr, "ADDING CONDIITON\n");
        std::vector<json> conditions = j["conditions"];
        unsigned int m;
        for (m = 0; m<conditions.size();m++) {
            cond_list.add_element(conditions[m].get<Condition>());
            fprintf(stderr, "ADDING CONDIITON\n");
        }
        fprintf(stderr, "ADDING CONDIITON2\n");
        std::vector<json> table = j["table"];
        fprintf(stderr, "ADDING CONDIITON3\n");
        for (m = 0; m<table.size();m++) {
            table_vector.push_back(table[m].get<TableEntry>());
            fprintf(stderr, "ADDING TABLEENTRY\n");
        }

        uint32_t initial_state = (uint32_t) atoi(j.at("initialstate").get<std::string>().c_str());
        fprintf(stderr, "ALLOCATING XFSM\n");
        return new TcpXfsm(table_vector, cond_list, initial_state);

    }



} 
// namespace ns3

