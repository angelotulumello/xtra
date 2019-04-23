#include "loader.h"

typedef int RETURN_STATUS;


RETURN_STATUS str_to_uint64(char *str, uint64_t *val)
{
    char *endptr;

    errno = 0;
    *val = (uint64_t) strtoul(str, &endptr, 10);

    if ((errno == ERANGE && (*val == LONG_MAX || *val == LONG_MIN))
        || (errno != 0 && *val == 0) || endptr == str)
        return RETURN_ERROR;

    return RETURN_SUCCESS;
}

RETURN_STATUS str_to_uint32(char *str, uint32_t *val)
{
    char *endptr;

    errno = 0;
    *val = (uint64_t) strtoul(str, &endptr, 10);

    if ((errno == ERANGE && (*val == LONG_MAX || *val == LONG_MIN))
        || (errno != 0 && *val == 0) || endptr == str)
        return RETURN_ERROR;

    return RETURN_SUCCESS;
}

RETURN_STATUS str_to_condition_type(char *str, xfsm_condition_types_t *condition_type)
{
    if (strcmp(str, "XTRA_LESS") == 0)
        *condition_type = XFSM_LESS;
    else if (strcmp(str, "XTRA_LESS_EQ") == 0)
        *condition_type = XFSM_LESS_EQ;
    else if (strcmp(str, "XTRA_GREATER") == 0)
        *condition_type = XFSM_GREATER;
    else if (strcmp(str, "XTRA_GREATER_EQ") == 0)
        *condition_type = XFSM_GREATER_EQ;
    else if (strcmp(str, "XTRA_EQUAL") == 0)
        *condition_type = XFSM_EQUAL;
    else if  (strcmp(str, "XTRA_NOT_EQUAL") == 0)
        *condition_type = XFSM_NOT_EQUAL;
    else
    {
        return RETURN_ERROR;
    }

    return RETURN_SUCCESS;
}

RETURN_STATUS str_to_result(char *str, xfsm_results_t *result) {
    if (strcmp(str, "XTRA_TRUE") == 0)
        *result = XFSM_TRUE;
    else if (strcmp(str, "XTRA_FALSE") == 0)
        *result = XFSM_FALSE;
    else if (strcmp(str, "XTRA_DONT_CARE") == 0)
        *result = XFSM_DONT_CARE;
    else
    {
        return RETURN_ERROR;
    }

    return RETURN_SUCCESS;
}

RETURN_STATUS str_to_register64(char *str, uint64_t *reg) {
    if (strncmp(str, "XTRA_CUSTOM", 11) == 0) {
        char *reg_name = str + 11;
        if (str_to_uint64(reg_name, reg) == RETURN_SUCCESS)
        {
            *reg += CUSTOM_OFFSET;
            return RETURN_SUCCESS;
        }
        else
            return RETURN_ERROR;
    }
    else if (strcmp(str, "XTRA_TIMESTAMP") == 0)
    {
        *reg = XFSM_TIMESTAMP;
        return RETURN_SUCCESS;
    }
    else if (strcmp(str, "XTRA_CONTEXT_PARAM0") == 0)
    {
        *reg = XFSM_CONTEXT_PARAM0;
        return RETURN_SUCCESS;
    }
    else if (strcmp(str, "XTRA_CONTEXT_PARAM1") == 0)
    {
        *reg = XFSM_CONTEXT_PARAM1;
        return RETURN_SUCCESS;
    }/*
    else if (strcmp(str, "XTRA_ECHO_REPLY") == 0)
    {
        *reg = XFSM_ECHOREPLY;
        return RETURN_SUCCESS;
    }*/
    else
        return RETURN_ERROR;

}

RETURN_STATUS str_to_register32(char *str, xfsm_registers_t *reg) {
    if (strncmp(str, "XTRA_CUSTOM", 11) == 0) {
        char *reg_name = str + 11;
        if (str_to_uint64(reg_name, (uint64_t *) reg) == RETURN_SUCCESS)
        {
            *reg += CUSTOM_OFFSET;
            return RETURN_SUCCESS;
        }
        else
            return RETURN_ERROR;
    }
    else if (strcmp(str, "XTRA_TIMESTAMP") == 0)
    {
        *reg = XFSM_TIMESTAMP;
        return RETURN_SUCCESS;
    }
    else if (strcmp(str, "XTRA_CONTEXT_PARAM0") == 0)
    {
        *reg = XFSM_CONTEXT_PARAM0;
        return RETURN_SUCCESS;
    }
    else if (strcmp(str, "XTRA_CONTEXT_PARAM1") == 0)
    {
        *reg = XFSM_CONTEXT_PARAM1;
        return RETURN_SUCCESS;
    }/*
    else if (strcmp(str, "XTRA_ECHO_REPLY") == 0)
    {
        *reg = XFSM_ECHOREPLY;
        return RETURN_SUCCESS;
    }*/
    else
        return RETURN_ERROR;

}

RETURN_STATUS str_to_action_type(char *str, xfsm_action_types_t *action_type, xfsm_opcodes_t *opcode, bool *next_state) {
    *opcode = 0;
    *next_state = false;
    if (strcmp(str, "XTRA_SUM") == 0)
    {
        *action_type = XFSM_UPDATE;
        *opcode = XFSM_SUM;
    }
    else if (strcmp(str, "XTRA_MINUS") == 0)
    {
        *action_type = XFSM_UPDATE;
        *opcode = XFSM_MINUS;
    }
    else if (strcmp(str, "XTRA_MULTIPLY") == 0)
    {
        *action_type = XFSM_UPDATE;
        *opcode = XFSM_MULTIPLY;
    }
    else if (strcmp(str, "XTRA_DIVIDE") == 0)
    {
        *action_type = XFSM_UPDATE;
        *opcode = XFSM_DIVIDE;
    }
    else if (strcmp(str, "XTRA_MODULO") == 0)
    {
        *action_type = XFSM_UPDATE;
        *opcode = XFSM_MODULO;
    }
    else if (strcmp(str, "XTRA_MAX") == 0)
    {
        *action_type = XFSM_UPDATE;
        *opcode = XFSM_MAX;
    }
    else if (strcmp(str, "XTRA_MIN") == 0)
    {
        *action_type = XFSM_UPDATE;
        *opcode = XFSM_MIN;
    }
    else if (strcmp(str, "XTRA_SENDPKT") == 0)
        *action_type = XFSM_SENDPKT;
    else if (strcmp(str, "XTRA_STOREPKT") == 0)
        *action_type = XFSM_STOREPKT;
    else if (strcmp(str, "XTRA_SETTIMER") == 0)
        *action_type = XFSM_SETTIMER;
    else if (strcmp(str, "XTRA_DELETEPKT") == 0)
        *action_type = XFSM_DELETEPKT;
    else if (strcmp(str, "XTRA_SETFIELD") == 0)
        *action_type = XFSM_SETFIELD;
    else if (strcmp(str, "XTRA_STATE_TRANSITION") == 0)
    {
        *next_state = true;
        *action_type = XFSM_NEXT_STATE;
    }
    else
    {
        return RETURN_ERROR;
    }

    return RETURN_SUCCESS;
}

RETURN_STATUS str_to_event(char *str, xfsm_event_t *event)
{
    if (strcmp(str, "XTRA_PKT_RCVD") == 0)
        *event = XFSM_ACK;
    else if (strcmp(str, "XTRA_TIMEOUT") == 0)
        *event = XFSM_TIMEOUT;
    /*else if (strcmp(str, "XTRA_SACK") == 0)
        *event = XFSM_SACK;*/
    else if (strcmp(str, "XTRA_NOEVENT") == 0)
        *event = XFSM_NOEVENT;
    else
    {
        return RETURN_ERROR;
    }

    return RETURN_SUCCESS;
}

RETURN_STATUS str_to_value_type(char *str, xfsm_value_type_t *value)
{
    if (strcmp(str, "XTRA_REGISTER") == 0)
        *value = XFSM_REGISTER;
    else if (strcmp(str, "XTRA_CONSTANT") == 0)
        *value = XFSM_CONSTANT;
    else if (strcmp(str, "XTRA_HEADER_FIELD") == 0)
        *value = XFSM_HDR_FIELD;
    else
    {
        return RETURN_ERROR;
    }

    return RETURN_SUCCESS;
}

RETURN_STATUS str_to_hfield(char *str, uint64_t *value)
{
    if (strcmp(str, "XTRA_SRC_MAC") == 0)
        *value = SRC_MAC;
    else if (strcmp(str, "XTRA_DST_MAC") == 0)
        *value = DST_MAC;
    else if (strcmp(str, "XTRA_ETH_TYPE") == 0)
        *value = ETH_TYPE;
    else if (strcmp(str, "XTRA_IPv4_SRC") == 0)
        *value = IPv4_SRC;
    else if (strcmp(str, "XTRA_IPv4_DST") == 0)
        *value = IPv4_DST;
    else if (strcmp(str, "XTRA_IPv4_TTL") == 0)
        *value = IPv4_TTL;
    else if (strcmp(str, "XTRA_IPv4_PROTO") == 0)
        *value = IPv4_PROTO;
    else if (strcmp(str, "XTRA_IPv4_CHKSUM") == 0)
        *value = IPv4_CHKSUM;
    else if (strcmp(str, "XTRA_TCP_SPORT") == 0)
        *value = TCP_SPORT;
    else if (strcmp(str, "XTRA_TCP_DPORT") == 0)
        *value = TCP_DPORT;
    else if (strcmp(str, "XTRA_TCP_SEQNO") == 0)
        *value = TCP_SEQNO;
    else if (strcmp(str, "XTRA_TCP_WIN") == 0)
        *value = TCP_WIN;
    else if (strcmp(str, "XTRA_TCP_FLAGS") == 0)
        *value = TCP_FLAGS;
    else if (strcmp(str, "XTRA_TCP_ACKNO") == 0)
        *value = TCP_ACKNO;
    else if (strcmp(str, "XTRA_TCP_CHKSUM") == 0)
        *value = TCP_CHKSUM;
    else if (strcmp(str, "XTRA_TCP_TIMESTAMP") == 0)
        *value = TCP_TIMESTAMP;
    else if (strcmp(str, "XTRA_TCP_TS_ECHOREPLY") == 0)
        *value = TCP_TS_ECHOREPLY;
    else if (strcmp(str, "XTRA_TCP_WIN_SCALE") == 0)
        *value = TCP_WIN_SCALE;
    else if (strcmp(str, "XTRA_TCP_MAX_MSS") == 0)
        *value = TCP_MAX_MSS;
    else if (strcmp(str, "XTRA_UDP_SPORT") == 0)
        *value = UDP_SPORT;
    else if (strcmp(str, "XTRA_UDP_DPORT") == 0)
        *value = UDP_DPORT;
    else if (strcmp(str, "XTRA_UDP_LENGTH") == 0)
        *value = UDP_LENGTH;
    else if (strcmp(str, "XTRA_UDP_CHECKSUM") == 0)
        *value = UDP_CHKSUM;
    else if (strcmp(str, "XTRA_UDP_P1") == 0)
        *value = UDP_P1;
    else if (strcmp(str, "XTRA_UDP_P2") == 0)
        *value = UDP_P2;
    else if (strcmp(str, "XTRA_UDP_P3") == 0)
        *value = UDP_P3;
    else if (strcmp(str, "XTRA_UDP_P4") == 0)
        *value = UDP_P4;
    else
    {
        return RETURN_ERROR;
    }

    return RETURN_SUCCESS;
}

RETURN_STATUS getOperand(cJSON *opType, cJSON *op, xfsm_value_type_t *type, uint64_t *val)
{

    if (!cJSON_IsString(opType) || opType->valuestring == NULL ||
        str_to_value_type(opType->valuestring, type) != RETURN_SUCCESS)
    {
        return RETURN_ERROR;
    }

    if(*type == XFSM_REGISTER)
    {
        if (!cJSON_IsString(op) || op->valuestring == NULL ||
            str_to_register64(op->valuestring, val) != RETURN_SUCCESS) {
            return RETURN_ERROR;
        }
    }
    else if (*type == XFSM_CONSTANT)
    {
        if (!cJSON_IsString(op) || op->valuestring == NULL ||
                str_to_uint64(op->valuestring, val) != RETURN_SUCCESS) {
            return RETURN_ERROR;

        }
    } else
    {
        if (!cJSON_IsString(op) || op->valuestring == NULL ||
            str_to_hfield(op->valuestring, val) != RETURN_SUCCESS) {
            return RETURN_ERROR;
        }
    }
    return RETURN_SUCCESS;
}


xfsm_condition_t ** json_to_conditions(cJSON *conditions, int *num)
{
    cJSON *condition = NULL;
    *num = 0;
    cJSON_ArrayForEach(condition, conditions) {
        (*num)++;
    }
    if(*num >= UINT8_MAX)
    {
        fprintf(stderr, "[LOADER_ERROR] Too many conditions defined! Max number is %d, got %d\n", UINT8_MAX, *num);
        exit(EXIT_FAILURE);
    }

    xfsm_condition_t **condition_array = malloc((*num)*sizeof(xfsm_condition_t *));
    if (condition_array == NULL)
    {
        perror("[LOADER_ERROR] Error in malloc");
        exit(EXIT_FAILURE);
    }

    cJSON_ArrayForEach(condition, conditions) {
        cJSON *id = cJSON_GetObjectItemCaseSensitive(condition, "id");
        cJSON *opcode = cJSON_GetObjectItemCaseSensitive(condition, "opcode");
        cJSON *op0 = cJSON_GetObjectItemCaseSensitive(condition, "op0");
        cJSON *op0Type = cJSON_GetObjectItemCaseSensitive(condition, "op0Type");
        cJSON *op1 = cJSON_GetObjectItemCaseSensitive(condition, "op1");
        cJSON *op1Type = cJSON_GetObjectItemCaseSensitive(condition, "op1Type");

        int id_val;
        if (!cJSON_IsString(id) || id->valuestring == NULL ||
                str_to_uint32(id->valuestring, &id_val) != RETURN_SUCCESS)
        {
            fprintf(stderr, "[LOADER_ERROR]: id must be a string representing an integer!\n");
            exit(EXIT_FAILURE);
        }
        //xfsm_condition_t **parsed_condition = condition_array+id_val;
        condition_array[id_val] = malloc(sizeof(xfsm_condition_t));
        if (condition_array[id_val] == NULL)
        {
            perror("[LOADER_ERROR] Error in malloc");
            exit(EXIT_FAILURE);
        }

        if (!cJSON_IsString(opcode) || opcode->valuestring == NULL ||
            str_to_condition_type(opcode->valuestring, &(condition_array[id_val]->condition_type)) != RETURN_SUCCESS)
        {
            fprintf(stderr, "[LOADER_ERROR]: opcode must be a string representing a condition_type!\n");
            exit(EXIT_FAILURE);
        }

        if (getOperand(op0Type, op0, &(condition_array[id_val]->type1), &(condition_array[id_val]->value1)) == RETURN_ERROR)
        {
            fprintf(stderr, "[LOADER_ERROR]: unable to parse op0 [%s] as a [%s]!\n", op0->valuestring, op0Type->valuestring);
            exit(EXIT_FAILURE);
        }
        if (getOperand(op1Type, op1, &(condition_array[id_val]->type2), &(condition_array[id_val]->value2)) == RETURN_ERROR)
        {
            fprintf(stderr, "[LOADER_ERROR]: unable to parse op1!\n");
            exit(EXIT_FAILURE);
        }

        cJSON_free(id);
        cJSON_free(opcode);
        cJSON_free(op0);
        cJSON_free(op1);
        cJSON_free(op0Type);
        cJSON_free(op1Type);
    }

    return condition_array;
}

xfsm_action_t **json_to_actions(cJSON *actions, uint16_t *num, bool *next_state_valid, uint32_t *next_state)
{
    cJSON *action = NULL;
    *num = 0;
    cJSON_ArrayForEach(action, actions)
    {
        (*num)++;
    }
    fprintf(stderr, "actions found: %d, allocating %lu\n", *num, (*num)*sizeof(xfsm_action_t *));

    // fail --<>
    xfsm_action_t **parsed_actions = malloc((*num)*sizeof(xfsm_action_t *));//malloc((*num)*sizeof(xfsm_action_t *));
    fprintf(stderr, "size is %lu\n", sizeof(xfsm_action_t *));
    //xfsm_action_t **ptr  = parsed_actions;

    if (parsed_actions == NULL)
    {
        perror("[LOADER_ERROR] Error in malloc");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    cJSON_ArrayForEach(action, actions) {
        parsed_actions[i] = malloc(sizeof(xfsm_action_t));
        if (parsed_actions[i] == NULL) {
            perror("[LOADER_ERROR] Error in malloc");
            exit(EXIT_FAILURE);
        }
        *next_state_valid = false;
        cJSON *opcode = cJSON_GetObjectItemCaseSensitive(action, "opcode");
        if (!cJSON_IsString(opcode) || opcode->valuestring == NULL ||
            str_to_action_type(opcode->valuestring, &((parsed_actions[i])->type),
                               &((parsed_actions[i])->opcode), next_state_valid) != RETURN_SUCCESS) {
            fprintf(stderr, "[LOADER_ERROR]: opcode must be a string representing an operation_type, found %s!\n",
                    opcode->valuestring);
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "next_state_valid is %d\n", *next_state_valid);

        cJSON *op0 = cJSON_GetObjectItemCaseSensitive(action, "op0");
        cJSON *op0Type = cJSON_GetObjectItemCaseSensitive(action, "op0Type");
        cJSON *op1 = cJSON_GetObjectItemCaseSensitive(action, "op1");
        cJSON *op1Type = cJSON_GetObjectItemCaseSensitive(action, "op1Type");
        cJSON *op2 = cJSON_GetObjectItemCaseSensitive(action, "op2");
        cJSON *op2Type = cJSON_GetObjectItemCaseSensitive(action, "op2Type");


        //fprintf(stderr, "(*ptr)->type is %d\n", (parsed_actions[i])->type);
        if (!*next_state_valid)
        {
            switch ((parsed_actions[i])->type) {
            case XFSM_UPDATE: {
                if (getOperand(op0Type, op0, &((parsed_actions[i])->type1), &((parsed_actions[i])->value1)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_UPDATE unable to parse op0!\n");
                    exit(EXIT_FAILURE);
                }
                if (getOperand(op1Type, op1, &((parsed_actions[i])->type2), &((parsed_actions[i])->value2)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_UPDATE unable to parse op1!\n");
                    exit(EXIT_FAILURE);
                }

                if (!cJSON_IsString(op2) || op2->valuestring == NULL ||
                    str_to_register32(op2->valuestring, &((parsed_actions[i])->output)) != RETURN_SUCCESS) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_UPDATE Invalid register name %s, "
                            "op2 must be a string representing a registers_type!\n", op2->valuestring);
                    exit(EXIT_FAILURE);
                }
                break;
            }

            case XFSM_SETTIMER: {
                if (getOperand(op0Type, op0, &((parsed_actions[i])->type1), &((parsed_actions[i])->value1)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SETTIMER unable to parse op0!\n");
                    exit(EXIT_FAILURE);
                }
                if (getOperand(op1Type, op1, &((parsed_actions[i])->type2), &((parsed_actions[i])->value2)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SETTIMER unable to parse op1!\n");
                    exit(EXIT_FAILURE);
                }
                if (getOperand(op2Type, op2, &((parsed_actions[i])->type3), &((parsed_actions[i])->value3)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SETTIMER unable to parse op2!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }

            case XFSM_SENDPKT: {
                if (getOperand(op0Type, op0, &((parsed_actions[i])->type1), &((parsed_actions[i])->value1)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SENDPKT unable to parse op0!\n");
                    exit(EXIT_FAILURE);
                }
                if (getOperand(op1Type, op1, &((parsed_actions[i])->type2), &((parsed_actions[i])->value2)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SENDPKT unable to parse op1!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }

            case XFSM_STOREPKT: {
                if (getOperand(op0Type, op0, &((parsed_actions[i])->type1), &((parsed_actions[i])->value1)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_STOREPKT unable to parse op0!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }

            case XFSM_SETFIELD: {
                if (getOperand(op0Type, op0, &((parsed_actions[i])->type1), &((parsed_actions[i])->value1)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SETFIELD unable to parse op0!\n");
                    exit(EXIT_FAILURE);
                }
                if (getOperand(op1Type, op1, &((parsed_actions[i])->type2), &((parsed_actions[i])->value2)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SETFIELD unable to parse op1!\n");
                    exit(EXIT_FAILURE);
                }
                if (getOperand(op2Type, op2, &((parsed_actions[i])->type3), &((parsed_actions[i])->value3)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_SETFIELD unable to parse op2!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }

            case XFSM_DELETEPKT: {
                if (getOperand(op0Type, op0, &((parsed_actions[i])->type1), &((parsed_actions[i])->value1)) ==
                    RETURN_ERROR) {
                    fprintf(stderr, "[LOADER_ERROR]: XFSM_DELETEPKT unable to parse op0!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }
        }
            i++;
    } else {
            if (!cJSON_IsString(op0) || op0->valuestring == NULL ||
                    str_to_uint32(op0->valuestring, next_state) != RETURN_SUCCESS) {
                fprintf(stderr, "[LOADER_ERROR]: Invalid register name %s, "
                        "op0 must be a string representing a state (unsigned integer)!\n", op0->valuestring);
                exit(EXIT_FAILURE);
            }
            (*num)--;
        }

        cJSON_free(opcode);
        cJSON_free(op0);
        cJSON_free(op1);
        cJSON_free(op2);
        cJSON_free(op0Type);
        cJSON_free(op1Type);
        cJSON_free(op2Type);
    }

    return parsed_actions;
}

xfsm_table_entry_t *json_to_table_entry(cJSON *json, int num_conditions) {
    cJSON *current_state = cJSON_GetObjectItemCaseSensitive(json, "state");
    cJSON *event = cJSON_GetObjectItemCaseSensitive(json, "event");
    cJSON *conditions = cJSON_GetObjectItemCaseSensitive(json, "condition_results");

    xfsm_table_entry_t *table_entry = malloc(sizeof(xfsm_table_entry_t));
    if (table_entry == NULL) {
        perror("[LOADER_ERROR] Error in malloc");
        exit(EXIT_FAILURE);
    }

    if (!cJSON_IsString(current_state) || current_state->valuestring == NULL ||
            str_to_uint32(current_state->valuestring, &(table_entry->current_state)) != RETURN_SUCCESS) {
        fprintf(stderr, "[LOADER_ERROR]: state must be a string representing an unsigned integer!\n");
        exit(EXIT_FAILURE);
    }

    table_entry->next_state = table_entry->current_state; // if no next_state_action is defined, stay in current state

    table_entry->priority = 0; // for now unused!

    if (!cJSON_IsString(event) || event->valuestring == NULL ||
        str_to_event(event->valuestring, &(table_entry->event)) != RETURN_SUCCESS) {
        fprintf(stderr, "[LOADER_ERROR]: event must be a string representing an event_type!\n");
        exit(EXIT_FAILURE);
    }
    table_entry->results = malloc(sizeof(xfsm_results_t) * num_conditions);
    if (table_entry->results == NULL) {
        perror("[LOADER_ERROR] Error in malloc");
        exit(EXIT_FAILURE);
    }
    // GRAVISSSIMO <=
    // hai ragione! ahhahahahah
    fprintf(stderr, "num_conditions is %d\n", num_conditions);
    for(int j = 0; j < num_conditions; j++)
    {
        table_entry->results[j] = XFSM_DONT_CARE;
        fprintf(stderr, "%d <<---\n", j);
    }
    //memset(table_entry->results, 2, num_conditions*sizeof(xfsm_results_t));
    cJSON *condition = NULL;
    cJSON_ArrayForEach(condition, conditions) {
        cJSON *id = cJSON_GetObjectItemCaseSensitive(condition, "id");
        cJSON *result = cJSON_GetObjectItemCaseSensitive(condition, "result");
        uint32_t id_val;
        if (!cJSON_IsString(id) || id->valuestring == NULL ||
                str_to_uint32(id->valuestring, &id_val) != RETURN_SUCCESS) {
            fprintf(stderr, "[LOADER_ERROR]: id must be a string representing an integer!\n");
            exit(EXIT_FAILURE);
        }
        if (id_val >= num_conditions) {
            fprintf(stderr, "[LOADER_ERROR]: id must be between 0 and the number of conditions!\n");
            exit(EXIT_FAILURE);
        }

        if (!cJSON_IsString(result) || result->valuestring == NULL ||
            str_to_result(result->valuestring, &(table_entry->results[id_val])) != RETURN_SUCCESS) {
            fprintf(stderr, "[LOADER_ERROR]: result must be a string representing a condition result!\n");
            exit(EXIT_FAILURE);
        }
    }

    cJSON *actions = cJSON_GetObjectItemCaseSensitive(json, "actions");
    bool next_state_valid = false;
    xfsm_action_t **ptr = json_to_actions(actions, &(table_entry->actions_num), &next_state_valid, &(table_entry->next_state));
    table_entry->actions = ptr;
    if (!next_state_valid)
        table_entry->next_state = table_entry->current_state;
    return table_entry;
}

xfsm_table_t *json_to_table(cJSON *table, timer_globals_t *globals, xfsm_pktio_params_t *pktout, lookup_key_t *lookup_key) {

    cJSON *conditions_json = cJSON_GetObjectItemCaseSensitive(table, "conditions");
    int conditions_number;
    xfsm_condition_t **conditions = json_to_conditions(conditions_json, &conditions_number);
    cJSON_free(conditions_json);

    cJSON *initial_state_json = cJSON_GetObjectItemCaseSensitive(table, "initial_state");
    uint32_t initial_state;
    if (!cJSON_IsString(initial_state_json) || initial_state_json->valuestring == NULL ||
            str_to_uint32(initial_state_json->valuestring, &initial_state) != RETURN_SUCCESS) {
        fprintf(stderr, "[LOADER_ERROR]: initial_state must be a string representing an integer!\n");
        exit(EXIT_FAILURE);
    }
    cJSON_free(initial_state_json);

    cJSON *table_rows = cJSON_GetObjectItemCaseSensitive(table, "table_rows");
    cJSON *table_row_json;
    int table_size = 0;
    cJSON_ArrayForEach(table_row_json, table_rows)
    {
        table_size++;
    }

    xfsm_table_entry_t **table_entries = calloc(table_size, sizeof(xfsm_table_entry_t *));
    if (table_entries == NULL) {
        perror("[LOADER_ERROR] Error in malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    cJSON_ArrayForEach(table_row_json, table_rows)
    {
        table_entries[i] = json_to_table_entry(table_row_json, conditions_number);
        i++;
    }

    xfsm_table_t *xfsm = xfsm_table_create(table_entries, table_size, initial_state, conditions, conditions_number, globals, pktout, lookup_key);

    /*cJSON_ArrayForEach(table_row_json, table_rows)
    {
        free(table_entries[i]);
    }*/

    cJSON_free(table_rows);
    //free(table_entries);

    return xfsm;
}

cJSON* str_to_json(char *str)
{
    return cJSON_Parse(str);
}

char** file_to_str(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if(f == NULL)
    {
        perror("Error in fopen");
        exit(EXIT_FAILURE);
    }
}
