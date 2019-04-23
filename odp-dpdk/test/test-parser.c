#include "../src/xfsm.h"
#include "../src/parse-args.h"
#include "teb-tcp.h"
#include "../src/pkt-parser.h"

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
    unsigned int max_us = 1000000;         // 1 seconds


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

    for (int i=0; i<MAX_TIMERS; i++){
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

    printf("MAC addr of pktio 0: %x:%x:%x:%x:%x:%x\n",
           eth.addr[0], eth.addr[1], eth.addr[2],
           eth.addr[3], eth.addr[4], eth.addr[5]);


    xfsm_table_t *xfsm = xfsm_table_create(entries, TABLE_SIZE, 0, conditions, CONDITIONS, gbls, xfsm_pktio_params1);

    init_xfsm(xfsm);

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
    odp_pktin_queue_t pktin;
    odp_pktout_queue_t pktout;
    odp_pktio_t pktio;
    xfsm_table_t *xfsm = NULL;
    odp_shm_t shm = ODP_SHM_INVALID;
    uint32_t timestamp = 0;
    int MAX_ACKS = 16;
    odp_packet_t recv;
    uint32_t *buf;
    xfsm_timer_t *ctp;

    pktio = odp_pktio_lookup("0");

    odp_pktout_queue(pktio, &pktout, 1);
    if (NULL == &pktout) {
        EXAMPLE_ABORT("Error cannot retrieve pktout queue\n");
    }

    odp_pktin_queue(pktio, &pktin, 1);
    if (NULL == &pktin) {
        EXAMPLE_ABORT("Error cannot retrieve pktin queue\n");
    }

    // Lookup xfsm
    shm = odp_shm_lookup("xfsm_instance");
    if (ODP_SHM_INVALID == shm)
        EXAMPLE_ERR("No xfsm_instance in shared mem\n");

    xfsm = (xfsm_table_t *) odp_shm_addr(shm);
    if (NULL == xfsm)
        EXAMPLE_ERR("XFSM remains NULL\n");

    int num = 0;
    while (1) {
        num = odp_pktin_recv(pktin, &recv, 1);

        if (num == 0)
            continue;
        else {
            // print parse packet statistics
            printf("Packet received. Print header fields (pointer %p): \n", xfsm->pkt_fields);

            odp_packet_prefetch(recv, 0, ODPH_ETHHDR_LEN + ODPH_IPV4HDR_LEN + ODPH_TCPHDR_LEN);

            parse_pkt_header(xfsm, recv);

            char *byte = odp_packet_data(recv);

            for (int i=0; i<64; i++){
                printf("\\%2X", (unsigned int) byte[i] & 0xff);
            } printf("\n");

            for (int i=0; i<NUM_HDR_FIELDS; i++){
                uint64_t val = get_hdr_value(xfsm->pkt_fields[i]);
                printf("\t%s: %lu\n", xfsm->pkt_fields[i].field->name, val);
            }

        }
    }

    return 0;
}