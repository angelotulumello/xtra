#include <odp_api.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/xfsm.h"

#define MSS 1000

int main(int argc, char *argv[])
{
	odp_instance_t instance;
	int ret = 0;

	ret = odp_init_global(&instance, NULL, NULL);
	if (ret != 0) {
		exit(EXIT_FAILURE);
	}
	ret = odp_init_local(instance, ODP_THREAD_WORKER);
	if (ret != 0) {
		exit(EXIT_FAILURE);
	}

	printf("test hash table:\n");

    xfsm_condition_t c1 = {.register1=XFSM_CWND, .register2=XFSM_NULL, .constant=MSS, .condition_type=XFSM_EQUAL};
    xfsm_condition_t c2 = {.register1=XFSM_CWND, .register2=XFSM_NULL, .constant=2*MSS, .condition_type=XFSM_EQUAL};

    xfsm_condition_t *conditions[2] = {&c1, &c2};

/**
 * Table entry 1
 */
    xfsm_action_t a101 = {.type=XFSM_UPDATE, .register1=XFSM_CWND, .register2=XFSM_NULL, .register3=XFSM_NULL, .param2=2,
            .opcode=XFSM_MULTIPLY, .output=XFSM_CWND};
    xfsm_action_t a102 = {.type=XFSM_SENDBUFF, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_NULL, .register3=XFSM_NULL,
            .param2=0, .opcode=XFSM_SUM, .output=XFSM_NULL};

    xfsm_action_t *a1[2] = {&a101, &a102};

    xfsm_results_t r1[2] = {XFSM_TRUE, XFSM_FALSE};

    xfsm_table_entry_t e1 = {.results=r1, .actions=a1, .actions_num=2, .current_state=0, .event=XFSM_TIMEOUT,
            .next_state=0,
            .priority=0};

    printf("created 1\n");
/**
 * Table entry 2
 */
    xfsm_action_t a201 = {.type=XFSM_UPDATE, .register1=XFSM_CWND, .register2=XFSM_NULL, .register3=XFSM_NULL, .param2=10,
            .opcode=XFSM_MULTIPLY, .output=XFSM_CWND};
    xfsm_action_t a202 = {.type=XFSM_SENDBUFF, .register1=XFSM_NEXTTXSEQ, .register2=XFSM_NULL, .register3=XFSM_NULL,
            .param2=0, .opcode=XFSM_SUM, .output=XFSM_NULL};

    xfsm_action_t *a2[2] = {&a201, &a202};

    xfsm_results_t r2[2] = {XFSM_TRUE, XFSM_DONT_CARE};

    xfsm_table_entry_t e2 = {.results=r2, .actions=a2, .actions_num=2, .current_state=0, .event=XFSM_TIMEOUT,
            .next_state=0,
            .priority=0};

    printf("created 2\n");

/**
 * Xfsm configuration
 */
    xfsm_table_entry_t *entries[2] = {&e1, &e2};

    // create test

    xfsm_table_t *xfsm = xfsm_table_create(entries, 2, 0, conditions, 2, NULL, result);

    // lookup test

    xfsm->results[0] = XFSM_FALSE;
    xfsm->results[1] = XFSM_DONT_CARE;
    xfsm->wakeup_event = XFSM_TIMEOUT;

    xfsm_table_entry_t *match = xfsm_table_lookup(xfsm);

    xfsm_table_destroy(xfsm);

    printf("all test finished success!!\n");

	if (odp_term_local()) {
		exit(EXIT_FAILURE);
	}

	if (odp_term_global(instance)) {
		exit(EXIT_FAILURE);
	}

	return 0;
}
