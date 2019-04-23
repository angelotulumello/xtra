#ifndef ODP_XFSM_XFSM_H
#define ODP_XFSM_XFSM_H

#include <odp/helper/odph_hashtable.h>
#include <odp_api.h>
#include "xfsm-timer.h"
#include "pktio.h"
#include "pkt-parser.h"

#define NUM_REGISTERS 24

#define TABLE_MAX_ENTRIES 64
#define MAX_CONDITIONS 16
#define STATE_NULL (-1)

#define MAX_ENTRIES_SE_PAIR 10

#define TIMEOUT_TOO_SHORT (-10)

#define DATA_PKT_REF 0
#define EMPTY_PKT_REF 1

#define KEY_SIZE                8
#define PKT_STORE_SIZE          32
#define LOOKUP_KEY_SIZE         8

#define CURRENT_PACKET          255
#define FIELD_NULL 			    255


typedef enum {
	XFSM_REGISTER = 0,
	XFSM_CONSTANT,
	XFSM_HDR_FIELD
} xfsm_value_type_t;

typedef enum {
    XFSM_NULL = 0,
    XFSM_CWND,
    XFSM_SSTHRESH,
    XFSM_SEGMENTSIZE,
    XFSM_LASTACKEDSEQ,
    XFSM_HIGHTXMARK,
    XFSM_NEXTTXSEQ,
    XFSM_RCVTIMESTAMPVALUE,
    XFSM_ECHOREPLY,
    XFSM_RTT,
    XFSM_AVAILABLE_WIN,
    XFSM_CUSTOM0,
    XFSM_CUSTOM1,
    XFSM_CUSTOM2,
    XFSM_CUSTOM3,
    XFSM_CUSTOM4,
    XFSM_CUSTOM5,
    XFSM_CONTEXT_PARAM0, // our three arguments used for passing parameters to an action
    XFSM_CONTEXT_PARAM1,
    XFSM_ARG0,
    XFSM_ARG1,
    XFSM_TIMESTAMP,
    XFSM_ZERO,
    XFSM_ONE
} xfsm_registers_t;

typedef enum {
    XFSM_SUM = 0,
    XFSM_MINUS,
    XFSM_MULTIPLY,
    XFSM_DIVIDE,
    XFSM_MODULO,
    XFSM_MAX,
    XFSM_MIN
} xfsm_opcodes_t;

typedef enum {
    XFSM_LESS = 0,
    XFSM_LESS_EQ,
    XFSM_GREATER,
    XFSM_GREATER_EQ,
    XFSM_EQUAL,
    XFSM_NOT_EQUAL
} xfsm_condition_types_t;

typedef enum {
    XFSM_UPDATE=0,
    XFSM_SENDPKT,
    XFSM_SETTIMER,
    XFSM_STOREPKT,
    XFSM_SETFIELD,
    XFSM_DELETEPKT
} xfsm_action_types_t;

typedef enum {
    XFSM_FALSE = 0,
    XFSM_TRUE,
    XFSM_DONT_CARE
} xfsm_results_t;

typedef enum {
    XFSM_ACK = 0,
    XFSM_TIMEOUT,
    XFSM_PACKET,
    XFSM_NOEVENT
} xfsm_event_t;

typedef struct xfsm_condition {
	xfsm_value_type_t 		type1;
	xfsm_value_type_t 		type2;
	uint64_t				value1;
	uint64_t				value2;
	xfsm_condition_types_t 	condition_type;
} xfsm_condition_t;

typedef struct lookup_key {
    uint8_t size;
    hdr_fields fld[LOOKUP_KEY_SIZE];
    uint8_t biflow_no;
} lookup_key_t;

typedef struct xfsm_table {
	/* Static parameters, initialized at configuration time */
	xfsm_condition_t			**conditions;					// array of conditions pointers
	uint8_t 					conditions_number;				// overall number of conditions
	timer_globals_t             *timer_gbls;                    // global parameters for timers
    xfsm_pktio_params_t         *pktios;                        // global pktio parameters
    xfsm_fields_tlv_t           pkt_fields[NUM_HDR_FIELDS];
    lookup_key_t                *lookup_key;
    uint8_t                     in_port;
} xfsm_table_t;

typedef struct xfsm_instance {
    xfsm_table_t                *table;
    odp_packet_t                pkts_tmp[PKT_STORE_SIZE];       // pkt ref array
    uint64_t 					registers[NUM_REGISTERS];		// array of registers
    uint32_t 					current_state;					// current state of xfsm
    xfsm_event_t				wakeup_event;					// event waking up the xfsm
    xfsm_results_t 				*results;						// results of current condition eval
    uint32_t                    *flow_key;                      // flow key of this xfsm
    odp_packet_t                curr_pkt;                       // reference to the current packet received
} xfsm_instance_t;

typedef struct xfsm_action {
	xfsm_action_types_t 	type;
	xfsm_opcodes_t 			opcode;
	xfsm_value_type_t 		type1;
	uint64_t                value1;
	xfsm_value_type_t 		type2;
	uint64_t 				value2;
	xfsm_value_type_t 		type3;
	uint64_t                value3;
	xfsm_registers_t 		output;
} xfsm_action_t;

typedef struct xfsm_table_entry {
	uint32_t 				current_state;
	uint32_t 				next_state;
	xfsm_results_t			*results;
	xfsm_action_t 			**actions;
	uint16_t 				actions_num;
	uint16_t 				priority;
	xfsm_event_t 			event;
} xfsm_table_entry_t;

typedef struct xfsm_search_key {
	uint32_t 			state;
	xfsm_event_t 		event;
} xfsm_search_key_t;

/*
 * Structs and functions to populate table entries
 */

typedef struct key_node {
	xfsm_search_key_t key;
	int entries[MAX_ENTRIES_SE_PAIR];
	int size;
	struct key_node *next;
} key_node_t;


bool search_key(xfsm_search_key_t *key1, key_node_t *list);

int insert_key(xfsm_search_key_t *key, key_node_t *list);

int insert_entry(int id, key_node_t *list, xfsm_search_key_t *key);


/* XFSM Functions */

/**
 * Creates an xfsm instance
 * @param entries               array of xfsm_table_entry_t
 * @param table_size            number of entries
 * @param initial_state         state in which xfsm starts
 * @param conditions            array of xfsm_condition_t
 * @param conditions_number     number of conditions
 * @return xfsm instance
 */
xfsm_table_t *
xfsm_table_create(xfsm_table_entry_t **entries, uint8_t table_size, uint32_t initial_state,
                  xfsm_condition_t **conditions, uint8_t conditions_number, timer_globals_t *globals,
                  xfsm_pktio_params_t *pktio_params, lookup_key_t *lookup_key);

xfsm_instance_t *xfsm_instance_create(xfsm_table_t *xfsm, uint32_t init_state, uint32_t *flow_key);

void
xfsm_table_destroy(xfsm_table_t *xfsm);

/**
 * Lookup for matching table entry
 * @param xfsm Xfsm instance
 * @retval match Matching table entry
 */
xfsm_table_entry_t*
xfsm_table_lookup(xfsm_instance_t *xfsm);

/**
 * Execute actions
 * @param match
 * @return
 */
int
xfsm_execute_actions(xfsm_table_entry_t *match, xfsm_instance_t *xfsm);

/**
 * Evaluate conditions and save the results
 * @param xfsm  the xfsm instance
 * @return void
 */
void
xfsm_evaluate_conditions(xfsm_instance_t *xfsm);

void
xfsm_wakeup (xfsm_instance_t *xfsm, xfsm_event_t ev, uint64_t param1, uint64_t param2);

void
execute_xfsm_stage(xfsm_instance_t *xfsm, xfsm_event_t ev, uint64_t p1, uint64_t p2);

static void
setup_pkt_fields(xfsm_table_t *xfsm);

/**
 * Parses packet header and inserts resulting values in the fields array of xfsm table.
 * Fields are saved as pointer to the packet buffer, so still in big endian.
 *
 * @param xfsm Xfsm instance
 * @param packet Packet
 * @retval -1 if packet is invalid
 * @retval -2 if packet does not have eth header
 * @retval -3 if packet does not have ipv4
 * @retval -4 if packet does not have either udp or tcp header
 * @retval PROTO_TCP if packet has tcp
 * @retval PROTO_UDP if packet has udp
 */
int
parse_pkt_header(xfsm_table_t *xfsm, odp_packet_t packet);

#endif
/* Network */
