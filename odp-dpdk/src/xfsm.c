#include <stdlib.h>
#include <stdio.h>
#include <odp/helper/odph_api.h>
#include <assert.h>
#include <inttypes.h>
#include <odp/helper/eth.h>
#include <odp/helper/ip.h>
#include <odp/helper/tcp.h>
#include <odp/helper/udp.h>
#include "xfsm.h"
#include "tcp-ops.h"
#include "session.h"


xfsm_table_t *
xfsm_table_create(xfsm_table_entry_t **entries, uint8_t table_size, uint32_t initial_state,
                  xfsm_condition_t **conditions, uint8_t conditions_number, timer_globals_t *globals,
                  xfsm_pktio_params_t *pktio_params, lookup_key_t *lookup_key)
{
    odp_shm_t shm = ODP_SHM_INVALID;
    key_node_t *list = malloc(sizeof(key_node_t));
    key_node_t *head = list;
    list->key.state = (uint32_t) STATE_NULL;
    list->size = 0;
    list->next = NULL;
    memset(list->entries, 0, sizeof(int)*MAX_ENTRIES_SE_PAIR);
    xfsm_search_key_t *key = malloc(sizeof(xfsm_search_key_t));

    uint8_t key_num = 0;
    // Populate keys_list
    for (int i=0; i<table_size; i++) {
        key->state = entries[i]->current_state;
        key->event = entries[i]->event;
        
        if (!search_key(key, list)){
            insert_key(key, list);
            insert_entry(i, list, key);
            key_num++;
        } else {
            insert_entry(i, list, key);
        }
    }
    
    key_node_t *ptr = head;

    /*
     * TODO: change hashtable
     */
    /* Create hashtable to hold keys */
    odph_table_t table;
    odph_table_ops_t *hash_ops = &odph_hash_table_ops;

    table = hash_ops->f_create("table", key_num,
                          sizeof(xfsm_search_key_t),
                          KEY_SIZE); // size of the key buffer


    while (ptr != NULL){
        odph_table_t newTab;
        int res = -10;

        char buf[KEY_SIZE] = "0";
        sprintf(buf, "%3u,%3u", ptr->key.state, ptr->key.event);

        newTab = hash_ops->f_create(buf, MAX_ENTRIES_SE_PAIR, sizeof(uint32_t), sizeof(xfsm_table_entry_t));
        // for each node on the list we create an array of entries
        for (uint32_t i=0; i<ptr->size; i++){
            res = hash_ops->f_put(newTab, &i, entries[ptr->entries[i]]);
        }

        res = hash_ops->f_put(table, &ptr->key, buf);

        EXAMPLE_DBG("Created new state|event pair table entry: %s\n", buf);


        xfsm_table_entry_t *ent = malloc(sizeof(xfsm_table_entry_t));
        uint32_t index = 0;
        newTab = hash_ops->f_lookup(buf);
        res = hash_ops->f_get(newTab, &index, ent, sizeof(xfsm_table_entry_t));

        ptr = ptr->next;
    }

	// Xfsm configuration and initialization
	xfsm_table_t *xfsm;

    shm = odp_shm_reserve("xfsm_instance", sizeof(xfsm_table_t),
                          ODP_CACHE_LINE_SIZE, 0);
    if (ODP_SHM_INVALID == shm) {
        EXAMPLE_ERR("Error: shared mem reserve failed for xfsm.\n");
        exit(EXIT_FAILURE);
    }

    xfsm = odp_shm_addr(shm);
    if (NULL == xfsm) {
        EXAMPLE_ERR("Error: shared mem alloc failed for xfsm.\n");
        exit(EXIT_FAILURE);
    }

    xfsm->conditions = conditions;
    xfsm->conditions_number = conditions_number;
    xfsm->timer_gbls = globals;
    xfsm->pktios = pktio_params;
    xfsm->lookup_key = lookup_key;
    setup_pkt_fields(xfsm);

    // Create the hash table to store all the xFSMs
    create_instance_db(lookup_key);

    EXAMPLE_DBG("XFSM Table successfully created\n");

	return xfsm;
}

xfsm_instance_t *xfsm_instance_create(xfsm_table_t *xfsm, uint32_t init_state, uint32_t *flow_key){
    xfsm_instance_t *xfsmi = malloc(sizeof(xfsm_instance_t));

    memset(xfsmi->registers, 0, sizeof(uint64_t) * NUM_REGISTERS);
    xfsmi->registers[XFSM_ONE] = 1;
    xfsmi->results = calloc(xfsm->conditions_number, sizeof(xfsm_results_t));
    xfsmi->wakeup_event = XFSM_NOEVENT;
    xfsmi->current_state = init_state;
    xfsmi->flow_key = malloc(sizeof(uint32_t) * xfsm->lookup_key->size);
    memcpy(xfsmi->flow_key, flow_key, sizeof(uint32_t) * xfsm->lookup_key->size);
    xfsmi->curr_pkt = ODP_PACKET_INVALID;
    xfsm->in_port = 0;

    for (int i=0; i<PKT_STORE_SIZE; i++)
        xfsmi->pkts_tmp[i] = ODP_PACKET_INVALID;

    xfsmi->table = xfsm;

    return xfsmi;
}

static void setup_pkt_fields(xfsm_table_t *xfsmi){
    for (int i=0; i<NUM_HDR_FIELDS; i++){
        xfsmi->pkt_fields[i].field = &xfsm_header_fields[i];
        xfsmi->pkt_fields[i].value = NULL;
    }
}

void
xfsm_table_destroy(xfsm_table_t *xfsm) {
    // destroy tables
    odph_table_t table;
    odph_table_ops_t *hash_ops = &odph_hash_table_ops;

    table = hash_ops->f_lookup("table");

    hash_ops->f_des(table);
}

xfsm_table_entry_t*
xfsm_table_lookup(xfsm_instance_t *xfsm) {
	odph_table_ops_t *ops = &odph_hash_table_ops;	
	char key[KEY_SIZE];

    sprintf(key, "%3d,%3d", xfsm->current_state, xfsm->wakeup_event);

    EXAMPLE_DBG("\n\nLookup. KEY: %s\n\n", key);

    odph_table_t tab = ops->f_lookup(key);
    if (tab != NULL) {
        // while loop scanning all hash table entries
        xfsm_table_entry_t *match = malloc(sizeof(xfsm_table_entry_t));
        uint32_t index;
        for (index=0; index < MAX_ENTRIES_SE_PAIR; index++) {
            if (ops->f_get(tab, &index, match, sizeof(xfsm_table_entry_t)) >= 0) {
                // found entry; now look for matching conditions
                for (int i=0; i<xfsm->table->conditions_number; i++) {
                    // don't care: go on
                    if (match->results[i] == XFSM_DONT_CARE) {
                        if (i == xfsm->table->conditions_number - 1)
                            return match;
                        else
                            continue;
                    }
                    // there is something
                    else {
                        // if I match a condition, continue
                        if (match->results[i] == xfsm->results[i]) {
                            // if even the last condition matches, return the result
                            if (i == xfsm->table->conditions_number - 1)
                                return match;
                            else
                                continue;
                        } else
                            break;
                    }
                }
            } else {
                EXAMPLE_DBG("Entry not found\n");
                return NULL;
            }
        }
        return NULL;
    } else {
        EXAMPLE_DBG("No matching |state|event| pairs found\n");
        return NULL;
    }
}

/**
 * Evaluate a single condition
 * @param cond          structure describing the condition
 * @param reg[in,out]   array of xfsm registers passed by reference
 * @return
 */
xfsm_results_t evaluate_condition(xfsm_condition_t cond, uint64_t *reg, xfsm_fields_tlv_t *fields) {
	xfsm_condition_types_t opcode = cond.condition_type;
	bool result = false;
	uint64_t param1 = 0, param2 = 0;

    switch (cond.type1){
        case XFSM_REGISTER:
            param1 = reg[cond.value1];
            break;
        case XFSM_CONSTANT:
            param1 = cond.value1;
            break;
        case XFSM_HDR_FIELD:
            param1 = get_hdr_value(fields[cond.value1]);
            break;
        default:
            EXAMPLE_ERR("Invalid register 1 type\n");
            break;
    }

    switch (cond.type2){
        case XFSM_REGISTER:
            param2 = reg[cond.value2];
            break;
        case XFSM_CONSTANT:
            param2 = cond.value2;
            break;
        case XFSM_HDR_FIELD:
            param2 = get_hdr_value(fields[cond.value2]);
            break;
        default:
            EXAMPLE_ERR("Invalid register 2 type\n");
            break;
    }
	

	switch (opcode) {
		case XFSM_LESS:
		{
			result = param1 < param2;
			break;
		}
		case XFSM_LESS_EQ:
		{
			result = param1 <= param2;
			break;
		}
		case XFSM_GREATER:
		{
			result = param1 > param2;
			break;
		}
		case XFSM_GREATER_EQ:
		{
			result = param1 >= param2;
			break;
		}
		case XFSM_EQUAL:
		{
			result = param1 == param2;
			break;
		}
		case XFSM_NOT_EQUAL:
		{
			result = param1 != param2;
			break;
		}
	}

	if (result)
		return XFSM_TRUE;
	else 
		return XFSM_FALSE;
}

void
xfsm_evaluate_conditions(xfsm_instance_t *xfsm) {
	for (int i=0; i<xfsm->table->conditions_number; i++)
		xfsm->results[i] = evaluate_condition(*xfsm->table->conditions[i], xfsm->registers, xfsm->table->pkt_fields);
}

/**
 * Action update: calculate operation and update registers
 * @param reg1              first register of the operation
 * @param reg2              first register of the operation. Could be XFSM_NULL
 * @param ext_param         if reg2 = XFSM_NULL param2 would be the second parameter
 * @param opcode            operation type
 * @param output            output register
 * @param xfsm_regs[out,in] reference to xfsm registers array
 */
static inline void
update(uint64_t reg1, uint64_t reg2, xfsm_opcodes_t opcode,
       xfsm_registers_t output, uint64_t *xfsm_regs) {
    uint64_t result = 0, param1 = 0, param2 = 0;

    switch(opcode)
    {
        case(XFSM_SUM):
            result = reg1 + reg2;
            break;
        case(XFSM_MINUS):
            if (reg2 > reg1)
                result = 0;
            else
                result = reg1 - reg2;
            break;
        case(XFSM_MULTIPLY):
            result = reg1 * reg2;
            break;
        case(XFSM_DIVIDE):
        {
            if (reg2==0)
                result = reg1;
            else
                result = reg1 / reg2;
        }
            break;
        case (XFSM_MODULO):
            result = reg1 % reg2;
            break;
        case(XFSM_MAX):
            result = reg1 > reg2 ? reg1 : reg2;
            break;
        case(XFSM_MIN):
            result = reg1 < reg2 ? reg1 : reg2;
            break;
        default:
            break; // Some warning log
    }
    xfsm_regs[output] = result;
}

int xfsm_execute_actions(xfsm_table_entry_t *match, xfsm_instance_t *xfsm) {
    int tts = 0;
    uint64_t reg1 = 0, reg2 = 0, reg3 = 0;

	for (int i=0; i<match->actions_num; i++){
		xfsm_action_t *act = match->actions[i];

        switch (act->type1){
            case XFSM_REGISTER:
                reg1 = xfsm->registers[act->value1];
                break;
            case XFSM_CONSTANT:
                reg1 = act->value1;
                break;
            case XFSM_HDR_FIELD:
                if (act->type == XFSM_SETFIELD)
                    reg1 = act->value1;
                else
                    reg1 = get_hdr_value(xfsm->table->pkt_fields[act->value1]);
                break;
            default:
                EXAMPLE_ERR("Invalid field 1 type\n");
                break;
        }

        switch (act->type2){
            case XFSM_REGISTER:
                reg2 = xfsm->registers[act->value2];
                break;
            case XFSM_CONSTANT:
                reg2 = act->value2;
                break;
            case XFSM_HDR_FIELD:
                reg2 = get_hdr_value(xfsm->table->pkt_fields[act->value2]);
                break;
            default:
                EXAMPLE_ERR("Invalid field 2 type\n");
                break;
        }

        if (act->type == XFSM_SETFIELD || act->type == XFSM_SETTIMER) {
            switch (act->type3) {
                case XFSM_REGISTER:
                    reg3 = xfsm->registers[act->value3];
                    break;
                case XFSM_CONSTANT:
                    reg3 = act->value3;
                    break;
                case XFSM_HDR_FIELD:
                    reg3 = get_hdr_value(xfsm->table->pkt_fields[act->value3]);
                    break;
                default:
                    EXAMPLE_ERR("Invalid field 3 type\n");
                    break;
            }
        }

        EXAMPLE_DBG("\nreg1: %lu, reg2: %lu, reg3: %lu\n", reg1, reg2, reg3);

		switch(act->type) {
			case XFSM_UPDATE:
			{
				update(reg1, reg2, act->opcode, act->output, xfsm->registers);
				break;
			}
			//                reg1    const|reg2
			// send_packet(packet_ref, out_port);
    		case XFSM_SENDPKT:
    		{

    		    EXAMPLE_DBG("\n\nSending packet\n\n");
                uint64_t out_port, packet_ref;
    		    odp_pktio_t pktio;
                odp_pktout_queue_t pktout;
                odp_packet_t packet;

    		    packet_ref = reg1;
    		    out_port = reg2;

    		    // NOTE: ports go from 1 to 4
    		    char *pktio_name = xfsm->table->pktios->devs[out_port-1];
    		    pktio = odp_pktio_lookup(pktio_name);

    		    odp_pktout_queue(pktio, &pktout, 1);

    		    if (packet_ref != CURRENT_PACKET)
    		        packet = odp_packet_ref(xfsm->pkts_tmp[packet_ref], 0);
                else
                    packet = odp_packet_ref(xfsm->curr_pkt, 0);

    		    if (packet == ODP_PACKET_INVALID)
    		        EXAMPLE_ERR("Packet invalid\n");

    		    odph_ipv4_csum_update(packet);

                //odp_packet_has_tcp_set(packet, 1);
                /*odp_packet_has_l4_set(packet, 1);
                odp_packet_has_l2_set(packet, 1);*/
                //odp_packet_has_ipv4_set(packet, 1);

                /**
                 * HW support for IPv4 checksum calculation &  configured
                   -          ip->csum  set to 0
                   -          L2, L3, L4 offsets set
                   -          IPv4 flag set (odp_packet_has_ipv4_set())
                 */

    		    if (xfsm->table->pkt_fields[TCP_CHKSUM].value != NULL)
    		        odph_udp_tcp_chksum(packet, ODPH_CHKSUM_GENERATE,
                                        (uint16_t *) xfsm->table->pkt_fields[TCP_CHKSUM].value);
                else if (xfsm->table->pkt_fields[UDP_CHKSUM].value != NULL) {
                    /*odph_udp_tcp_chksum(packet, ODPH_CHKSUM_GENERATE,
                                        (uint16_t *) xfsm->table->pkt_fields[UDP_CHKSUM].value);*/
                    EXAMPLE_DBG("NO Checksum\n");

                }


    		    //odp_packet_print(packet);
    		    odp_pktout_send(pktout, &packet, 1);


                odp_packet_free(packet);

    			break;
    		}
    		case XFSM_SETTIMER:
            {
                printf("\n\nSet timer!!\n");
                /*
                 *
                 * Timers are set in a type of circular buffer. When there's no more space in tt[]
                 * restart from 0. 1000 timers should be enough for our use cases
                 * set_timer(reg1,      reg2,       reg3)
                 *           data1(32)  data2(8b)  sched_time
                 */


                EXAMPLE_DBG("Arming timer with seq = %lu, at time %lu, retx=%lu\n", reg1, reg3, reg2);

                timer_globals_t *globals = xfsm->table->timer_gbls;
                uint32_t tid = globals->last_timer_id;
                assert(tid < MAX_TIMERS);

                uint64_t sched_time = reg3;
                /*
                 * If scheduling time is less than resolution,
                 */
                if (sched_time < TIMER_RES){
                    // don't schedule a timer: just execute another xfsm stage after the actions.
                    tts = TIMEOUT_TOO_SHORT;
                } else {
                    uint64_t time = 0, carry = 0;

                    // calculate scheduling time as a multiple of timer resolution
                    carry = sched_time % TIMER_RES;
                    if (carry > TIMER_RES / 2)
                        time = reg3 - carry + TIMER_RES;
                    else
                        time = reg3 - carry;

                    if (tid < MAX_TIMERS - 1) {
                        // set timer
                        set_timer(xfsm->table->timer_gbls, &globals->tt[tid + 1], (uint32_t) reg1,
                                  (uint8_t) reg2, (uint32_t) time, xfsm->flow_key);
                        // increase last timer id
                        globals->last_timer_id++;
                    } else {
                        // overwrite first timer pointer block
                        globals->last_timer_id = 0;
                        // set timer to first block
                        set_timer(xfsm->table->timer_gbls, &globals->tt[0], (uint32_t) reg1,
                                 (uint8_t) reg2, (uint32_t) time, xfsm->flow_key);
                    }
                }

                EXAMPLE_DBG("Exiting set timer\n");
    			break;
    		}

            case XFSM_STOREPKT: {
                odp_pool_t storage_pool = odp_pool_lookup(STORAGE_POOL_NAME);
                uint64_t id = reg1;

                EXAMPLE_DBG("Copying packet\n");

                xfsm->pkts_tmp[id] = odp_packet_copy(xfsm->curr_pkt, storage_pool);

                if (EXAMPLE_DEBUG_PRINT) {
                    EXAMPLE_DBG("Data is: \n");
                    uint8_t *byte = odp_packet_data(xfsm->pkts_tmp[id]);
                    for (int j = 0; j < 64; j++) {
                        EXAMPLE_DBG("\\%2X", (unsigned int) byte[j] & 0xff);
                    }
                    EXAMPLE_DBG("\n");
                }
                break;
            }
            // set_field(field_name, pkt_ref, value)
            //              reg1      const    reg3
            case XFSM_SETFIELD:
            {
                uint64_t field_name, pkt_ref, value;
                odp_packet_t pkt;

                field_name = reg1;
                pkt_ref = reg2;
                value = reg3;

                if (pkt_ref == CURRENT_PACKET)
                    pkt = xfsm->curr_pkt;
                else if (pkt_ref >= 0 && pkt_ref < PKT_STORE_SIZE) {
                    pkt = xfsm->pkts_tmp[pkt_ref];
                } else {
                    EXAMPLE_ERR("Error on packet reference: overflowing packet id\n");
                    break;
                }

                if (pkt == ODP_PACKET_INVALID){
                    EXAMPLE_ERR("No packet reference found in database... doing nothing\n");
                    break;
                }

                if (pkt_ref != CURRENT_PACKET)
                    parse_pkt_header(xfsm->table, pkt);

                switch (xfsm->table->pkt_fields[field_name].field->len){
                    case 6:
                        value <<= 16;
                        value = odp_cpu_to_be_64(value);
                        break;
                    case 4:
                        value = odp_cpu_to_be_32( (uint32_t) value);
                        break;
                    case 2:
                        value = odp_cpu_to_be_16( (uint16_t) value);
                        break;
                    default:
                        break;
                }

                if (xfsm->table->pkt_fields[field_name].value != NULL) {
                    // value of packet fields are pointers to the packet field
                    // this memcpy copies the value directly in the packet header

                    memcpy(xfsm->table->pkt_fields[field_name].value, &value,
                           xfsm->table->pkt_fields[field_name].field->len);

                    EXAMPLE_DBG("Executing set field: field: %s, value: %lu\n",
                                xfsm->table->pkt_fields[field_name].field->name,
                                get_hdr_value(xfsm->table->pkt_fields[field_name]));
                } else
                    EXAMPLE_ERR("Field not found in packet header... doing nothing %lu\n", field_name);

                EXAMPLE_DBG("Exiting set field\n");

                break;
            }

            case XFSM_DELETEPKT:
            {
                uint64_t id = reg1;

                xfsm->pkts_tmp[id] = ODP_PACKET_INVALID;
                break;
            }
		}
	}
    if (tts<0) {
        execute_xfsm_stage(xfsm, XFSM_TIMEOUT, reg1, reg2);
    }
    return 0;
}


void xfsm_wakeup (xfsm_instance_t *xfsm, xfsm_event_t ev, uint64_t param1, uint64_t param2){
    switch (ev){
        case XFSM_ACK:
        {
            xfsm->wakeup_event = XFSM_ACK;
            xfsm->registers[XFSM_CONTEXT_PARAM0] = param1;            // ack number
            xfsm->registers[XFSM_ECHOREPLY] = param2;       // echo reply?!
            break;
        }
        case XFSM_TIMEOUT:
        {
            xfsm->wakeup_event = XFSM_TIMEOUT;
            xfsm->registers[XFSM_CONTEXT_PARAM0] = param1;            // seq no
            xfsm->registers[XFSM_CONTEXT_PARAM1] = param2;            // retx count
            break;
        }
        case XFSM_PACKET:
        {
            xfsm->wakeup_event = XFSM_PACKET;
            xfsm->registers[XFSM_CONTEXT_PARAM0] = xfsm->table->in_port;
            break;
        }
        case XFSM_NOEVENT:
        {
            //nothing
            break;
        }
    }
}

void
execute_xfsm_stage(xfsm_instance_t *xfsm, xfsm_event_t ev, uint64_t p1, uint64_t p2){
    odp_time_t time1 = odp_time_global();
    uint64_t now = odp_time_to_ns(time1);

    xfsm->registers[XFSM_TIMESTAMP] = (now / 1000000);

    if (ev == XFSM_ACK){
        xfsm->registers[XFSM_CONTEXT_PARAM1] = (xfsm->registers[XFSM_TIMESTAMP] - p2) * 1000;
    }

    xfsm_wakeup(xfsm, ev, p1, p2);
    xfsm_evaluate_conditions(xfsm);
    xfsm_table_entry_t *match = xfsm_table_lookup(xfsm);
    if (match == NULL)
        return;

    EXAMPLE_DBG("executing actions\n");
    xfsm_execute_actions(match, xfsm);
    xfsm->current_state = match->next_state;

    EXAMPLE_DBG("\t\t State is %"PRIu32"\n", xfsm->current_state);

}



bool search_key(xfsm_search_key_t *key1, key_node_t *list) {
    key_node_t *keyNode;
    keyNode = list;
    //EXAMPLE_DBG("Searching key\n");

    while (keyNode != NULL) {
        xfsm_search_key_t *key2;
        key2 = &keyNode->key;
        if (memcmp(key1, key2, sizeof(xfsm_search_key_t)) == 0) {
            //EXAMPLE_DBG("key found: %u, %u\n", key1->state, key1->event);
            return true;
        }
        keyNode = keyNode->next;
    }

    return false;
}

int insert_key(xfsm_search_key_t *key, key_node_t *list) {
    key_node_t *keyNode;
    keyNode = list;

    // in case it's the first key
    if (keyNode->key.state == STATE_NULL) {
        memcpy(&keyNode->key, key, sizeof(xfsm_search_key_t));
        //EXAMPLE_DBG("inserting first key: %u,%u\n", key->state, key->event);
        return 0;
    }
    while (keyNode != NULL) {
        if (keyNode->next == NULL){
            keyNode->next = malloc(sizeof(key_node_t));
            keyNode = keyNode->next;
            memcpy(&keyNode->key, key, sizeof(xfsm_search_key_t));
            keyNode->size = 0;
            //EXAMPLE_DBG("inserting key: %u,%u\n", key->state, key->event);
            keyNode->next=NULL;
            return 0;
        } else
            keyNode = keyNode->next;
    }
    return 0;
}

int insert_entry(int id, key_node_t *list, xfsm_search_key_t *key) {
    key_node_t *keyNode;
    keyNode = list;

    while (keyNode != NULL) {
        xfsm_search_key_t *key2;
        key2 = &keyNode->key;
        if (memcmp(key, key2, sizeof(xfsm_search_key_t)) == 0) {
            keyNode->entries[keyNode->size] = id;
            keyNode->size++;
            //EXAMPLE_DBG("inserting entry: id=%u\n", id);
            return 0;
        }
        keyNode = keyNode->next;
    }

    return -1;
}

/* Network */