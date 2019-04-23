#include "../src/xfsm.h"
#include "../src/parse-args.h"
#include "test-proxy.h"
#include "../src/pkt-parser.h"
#include "../src/session.h"
#include "../src/cJSON.h"
#include "../src/loader.h"
#include <stdio.h>

#define PKT_BATCH_SIZE      16

static int run_thread(void *ptr);

int main (int argc, char *argv[]){
    odph_odpthread_t thread_tbl[1];
    odp_instance_t instance;

    odp_queue_t queue;

    odp_queue_param_t queue_param;
    odp_pool_param_t pool_param;
    odp_timer_pool_param_t timer_pool_param;

    odph_odpthread_params_t thr_params;

    // something we have to pass to the worker thread
    timer_globals_t *gbls = NULL;
    odp_shm_t shm = ODP_SHM_INVALID;

    //pktio
    odp_pktio_t pktio;
    odp_pktout_queue_t pktout;

    // assign worker to second cpu
    odp_cpumask_t mask;
    odp_cpumask_from_str(&mask, "0x4");

    // timer option parameters
    unsigned int resolution_us = TIMER_RES;
    unsigned int min_us = TIMER_RES;
    unsigned int max_us = 10000000;         // 1 seconds


    appl_args = (appl_args_t*) malloc(sizeof(appl_args_t));

    parse_args(argc, argv, appl_args);

    int ret = 0;

    ret = odp_init_global(&instance, NULL, NULL);
    if (ret != 0) {
        EXAMPLE_DBG("Init global failed\n");
        exit(EXIT_FAILURE);
    }
    ret = odp_init_local(instance, ODP_THREAD_WORKER);
    if (ret != 0) {
        EXAMPLE_DBG("Init local failed\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Reserve memory for timer_globals_t from shared mem
     */
    shm = odp_shm_reserve("shm_globals", sizeof(timer_globals_t),
                          ODP_CACHE_LINE_SIZE, 0);
    if (ODP_SHM_INVALID == shm) {
        EXAMPLE_ERR("Error: shared mem reserve failed.\n");
        exit(EXIT_FAILURE);
    }

    // Manual definition of lookup key: {IP_SRC, IP_DST, TCP_SPORT, TCP_DPORT, PROTO}

    /*
     * Biflow key fields ***MUST*** be grouped 2by2 and at the first positions... like below
     */
    lookup_key_t *lookup_key = malloc(sizeof(lookup_key_t));
    lookup_key->fld[0] = IPv4_SRC;
    lookup_key->fld[1] = IPv4_DST;
    lookup_key->fld[2] = TCP_SPORT;
    lookup_key->fld[3] = TCP_DPORT;
    lookup_key->fld[4] = IPv4_PROTO;

    lookup_key->size = 5;
    lookup_key->biflow_no = 2;


    gbls = odp_shm_addr(shm);
    if (NULL == gbls) {
        EXAMPLE_ERR("Error: shared mem alloc failed.\n");
        exit(EXIT_FAILURE);
    }

    memset(gbls, 0, sizeof(timer_globals_t));
    gbls->pool          = ODP_POOL_INVALID;
    gbls->timer_pool    = ODP_TIMER_POOL_INVALID;

    memset(thread_tbl, 0, sizeof(thread_tbl));

    /*
	 * Create pool for timeouts
	 */
    odp_pool_param_init(&pool_param);
    pool_param.tmo.num   = NUM_TMOS;
    pool_param.type      = ODP_POOL_TIMEOUT;

    gbls->pool = odp_pool_create("msg_pool", &pool_param);

    if (gbls->pool == ODP_POOL_INVALID) {
        EXAMPLE_ERR("Pool create failed.\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Setting timer pool parameters and creating it
     */
    timer_pool_param.res_ns     = resolution_us * ODP_TIME_USEC_IN_NS;
    timer_pool_param.min_tmo    = min_us * ODP_TIME_USEC_IN_NS;
    timer_pool_param.max_tmo    = max_us * ODP_TIME_USEC_IN_NS;
    timer_pool_param.num_timers = MAX_TIMERS;                           /* One timer per worker */
    timer_pool_param.priv       = 0;                                    /* Shared */
    timer_pool_param.clk_src    = ODP_CLOCK_CPU;

    gbls->timer_pool = odp_timer_pool_create("timer_pool", &timer_pool_param);

    if (gbls->timer_pool == ODP_TIMER_POOL_INVALID){
        EXAMPLE_ERR("Timer Pool create failed.\n");
        exit(EXIT_FAILURE);
    }
    // starts all timer pools configured.. from now on we can register timeouts
    odp_timer_pool_start();

    for (int i=0; i<MAX_TIMERS; i++) {
        gbls->tt[i].ev = ODP_EVENT_INVALID;
        gbls->tt[i].tim = ODP_TIMER_INVALID;
    }

    /*
     * Create the queue that will store our timeouts
     */
    odp_queue_param_init(&queue_param);
    queue_param.type        = ODP_QUEUE_TYPE_SCHED;
    queue_param.sched.prio  = ODP_SCHED_PRIO_DEFAULT;
    queue_param.sched.sync  = ODP_SCHED_SYNC_ATOMIC;
    queue_param.sched.group = ODP_SCHED_GROUP_ALL;

    queue = odp_queue_create("timer_queue", &queue_param);

    if (queue == ODP_QUEUE_INVALID) {
        EXAMPLE_ERR("Timer queue create failed.\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Create pktios
     */
    /*
     * Reserve memory for timer_globals_t from shared mem
     */
    xfsm_pktio_params_t *xfsm_pktio_params1;
    shm = odp_shm_reserve("shm_pktios", sizeof(xfsm_pktio_params_t),
                          ODP_CACHE_LINE_SIZE, 0);
    if (ODP_SHM_INVALID == shm) {
        EXAMPLE_ERR("Error: shared mem reserve failed.\n");
        exit(EXIT_FAILURE);
    }

    xfsm_pktio_params1 = odp_shm_addr(shm);
    if (NULL == xfsm_pktio_params1) {
        EXAMPLE_ERR("Error: shared mem alloc failed.\n");
        exit(EXIT_FAILURE);
    }

    create_pktio(xfsm_pktio_params1, appl_args->num_pktios);

    /*
     * **************************************************
     * Creating xFSM
     * **************************************************
     */

    pktio = odp_pktio_lookup("0");

    // get address
    appl_args->src_mac = malloc(18);

    odph_ethaddr_t eth;

    odp_pktio_mac_addr(pktio, &eth, ODP_PKTIO_MACADDR_MAXSIZE);


    EXAMPLE_DBG("MAC addr of pktio 0: %x:%x:%x:%x:%x:%x\n",
           eth.addr[0], eth.addr[1], eth.addr[2],
           eth.addr[3], eth.addr[4], eth.addr[5]);

    sprintf(appl_args->src_mac, "%2x:%2x:%2x:%2x:%2x:%2x",
            eth.addr[0], eth.addr[1], eth.addr[2],
            eth.addr[3], eth.addr[4], eth.addr[5]);

    EXAMPLE_DBG("Appl mac addr of pktio 0: %s\n",
                appl_args->src_mac);

    FILE *f = NULL;
    long len = 0;
    char *data = NULL;

    /* open in read binary mode */
    f = fopen("output.json","rb");
    if(f == NULL)
    {
        EXAMPLE_ERR("Unable to find 'output.json'\n");
        exit(EXIT_FAILURE);
    }
    /* get the length */
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    data = (char*)malloc(len + 1);

    if(fread(data, 1, len, f) < len)
    {
        EXAMPLE_ERR("Unable to read 'output.json'\n");
        exit(EXIT_FAILURE);
    }
    data[len] = '\0';
    fclose(f);

    cJSON *json = NULL;

    json = cJSON_Parse(data);
    if (!json)
    {
        fprintf(stderr, "[LOADER]: Invalid input file: error before: [%s]\n", cJSON_GetErrorPtr());
        exit(EXIT_FAILURE);
    }

    json_to_table(json, gbls, xfsm_pktio_params1, lookup_key);
    cJSON_free(json);

    /*xfsm_table_create(entries, TABLE_SIZE, 0, conditions, CONDITIONS,
        gbls, xfsm_pktio_params1, lookup_key);*/

    /*
     * **************************************************
     * Create worker thread (in our case only one worker)
     * **************************************************
     */
    memset(&thr_params, 0, sizeof(thr_params));
    thr_params.start    = run_thread;
    thr_params.arg      = gbls;
    thr_params.thr_type = ODP_THREAD_WORKER;
    thr_params.instance = instance;

    odph_odpthreads_create(thread_tbl, &mask, &thr_params);

    odph_odpthreads_join(thread_tbl);

    // then free resources

    EXAMPLE_DBG("All test finished success!!\n");

    if (odp_term_local()) {
        exit(EXIT_FAILURE);
    }

    if (odp_term_global(instance)) {
        exit(EXIT_FAILURE);
    }

    return 0;
}

static int run_thread(void *ptr) {
    odp_pktin_queue_t *pktins;
    odp_pktout_queue_t *pktouts;
    odp_pktio_t *pktios;
    xfsm_table_t *xfsm = NULL;
    odp_shm_t shm = ODP_SHM_INVALID;
    odp_packet_t recv[PKT_BATCH_SIZE];
    odph_table_t instance_table;
    odph_table_ops_t *ops = &odph_hash_table_ops;
    xfsm_instance_t *xfsmi;
    uint32_t *key;
    uint32_t num_pktios;
    odp_event_t ev[4];
    odp_timeout_t timeout;
    int events = 0;
    xfsm_timer_t *ctp;
    uint64_t now = 0;

    // Lookup xfsm
    shm = odp_shm_lookup("xfsm_instance");
    if (ODP_SHM_INVALID == shm)
        EXAMPLE_ERR("No xfsm_instance in shared mem\n");

    xfsm = (xfsm_table_t *) odp_shm_addr(shm);
    if (NULL == xfsm)
        EXAMPLE_ERR("XFSM remains NULL\n");

    // Retrieve pktios
    num_pktios = xfsm->pktios->num_pktios;
    pktios = malloc(sizeof(odp_pktio_t) * num_pktios);
    pktins = malloc(sizeof(odp_pktin_queue_t) * num_pktios);
    pktouts = malloc(sizeof(odp_pktout_queue_t) * num_pktios);

    for (int i=0; i<num_pktios; i++){
        pktios[i] = odp_pktio_lookup(xfsm->pktios->devs[i]);

        odp_pktout_queue(pktios[i], &pktouts[i], 1);
        if (NULL == &pktouts[i])
            EXAMPLE_ABORT("Error cannot retrieve pktout queue\n");

        odp_pktin_queue(pktios[i], &pktins[i], 1);
        if (NULL == &pktins[i])
            EXAMPLE_ABORT("Error cannot retrieve pktin queue\n");
    }

    // Lookup instance hash table
    instance_table = ops->f_lookup(INSTANCE_DB_NAME);

    key = malloc(sizeof(uint32_t)*5);
    key[0] = appl_args->src_addr;
    key[1] = appl_args->dst_addr;
    key[2] = appl_args->src_port;
    key[3] = appl_args->dst_port;
    key[4] = 6;

    biflow(key, xfsm->lookup_key);
    xfsmi = xfsm_instance_create(xfsm, 0, key);

    for (int i=0; i<5; i++){
        EXAMPLE_DBG("\tfield %d: %u\n", i, key[i]);
    }


    odp_pool_t storage_pool = odp_pool_lookup(STORAGE_POOL_NAME);

    xfsmi->pkts_tmp[1] = setup_tcp_empty_pkt_ref(storage_pool);
    xfsmi->pkts_tmp[2] = setup_tcp_pkt_ref(storage_pool, 0);

    insert_xfsm_instance(key, xfsmi, instance_table);

    execute_xfsm_stage(xfsmi, XFSM_ACK, 0, 0);

    int num = 0;
    while (1) {
        // cycle over all the ports
        for (int p=0; p<num_pktios; p++) {
            num = odp_pktin_recv(pktins[p], recv, PKT_BATCH_SIZE);

            if (num == 0)
                continue;
            else {
                for (int n=0; n<num; n++) {
                    // print parse packet statistics
                    EXAMPLE_DBG("\n\nPACKET received\n\n");

                    odp_packet_prefetch(recv[n], 0, ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN + ODPH_TCPHDR_LEN);

                    parse_pkt_header(xfsm, recv[n]);

                    key = key_extract(xfsm);
                    if (key == NULL)
                        continue;

                    xfsmi = lookup_xfsm_instance(key, instance_table);

                    if (xfsmi == NULL) {
                        EXAMPLE_DBG("Instance not found. Creating it\n");

                        xfsmi = xfsm_instance_create(xfsm, 0, key);
                        EXAMPLE_DBG("instance: (pointer: %lu)\n", (uint64_t) xfsmi);

                        if (xfsmi == NULL) {
                            EXAMPLE_DBG("Cannot create instance: (pointer: %lu)\n", (uint64_t) xfsmi);
                            break;
                        }
                        odp_pool_t storage_pool = odp_pool_lookup(STORAGE_POOL_NAME);

                        xfsmi->pkts_tmp[1] = setup_tcp_pkt_ref(storage_pool, 0);

                        insert_xfsm_instance(key, xfsmi, instance_table);
                    } else
                        EXAMPLE_DBG("\nInstance found!!!!\n");

                    // we have to update the current packet reference in the instance
                    xfsmi->curr_pkt = recv[n];
                    xfsm->in_port = (uint8_t) (p + 1);

                    free(key);

                    EXAMPLE_DBG("\n incoming IF is %d\n", xfsm->in_port);
                    execute_xfsm_stage(xfsmi, XFSM_ACK, xfsm->in_port, 121212);

                    /*char *byte = odp_packet_data(recv);

                    for (int i = 0; i < 64; i++) {
                        EXAMPLE_DBG()("\\%2X", (unsigned int) byte[i] & 0xff);
                    }
                    EXAMPLE_DBG()("\n");

                    for (int i = 0; i < NUM_HDR_FIELDS; i++) {
                        uint64_t val = get_hdr_value(xfsm->pkt_fields[i]);
                        EXAMPLE_DBG("\t%s: %lu\n", xfsm->pkt_fields[i].field->name, val);
                    }*/

                }
                odp_packet_free_multi(recv, num);
            }
        }

        events = odp_schedule_multi(NULL, ODP_SCHED_NO_WAIT, ev, 4);

        for (int i=0; i<events; i++) {
            if (ev[i] == ODP_EVENT_INVALID)
                continue;
            else if (odp_event_type(ev[i]) == ODP_EVENT_TIMEOUT) {
                /*
                 * Process xfsm wake up on timer
                 */
                timeout = odp_timeout_from_event(ev[i]);
                ctp = odp_timeout_user_ptr(timeout);

                xfsmi = lookup_xfsm_instance(ctp->flow_key, instance_table);

                if (xfsmi == NULL) {
                    EXAMPLE_ERR("Instance not found. Who is firing this timer?\n");
                    continue;
                }

                if (ctp == NULL)
                    EXAMPLE_ERR("Timer user pointer is null\n");

                odp_time_t time1 = odp_time_global();
                now = odp_time_to_ns(time1);

                EXAMPLE_DBG("\n\nTIMEOUT received\n\n");

                // execution of xfsm stage
                execute_xfsm_stage(xfsmi, XFSM_TIMEOUT, ctp->sequence_number, ctp->retx_count);

                odp_timer_free(ctp->tim);
                odp_timeout_free(timeout);
            }
        }
    }

    return 0;
}
