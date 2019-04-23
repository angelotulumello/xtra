#include <stdlib.h>
#include <stdio.h>
#include <odp/helper/odph_api.h>
#include <inttypes.h>
#include <odp/helper/tcp.h>
#include "../src/tcp-ops.h"
#include "../src/xfsm-timer.h"
#include "teb-tcp.h"
#include <math.h>

#define POOL_NUM_PKT           2048  /* Number of packets in packet pool */
#define POOL_PKT_LEN           1856  /* Max packet length */


static int run_thread(void *ptr);

static odp_pktio_t create_pktio()
{
    char dev[] = "0";

    odp_pool_t pool;
    odp_pool_param_t params;
    odp_pool_param_init(&params);

    params.pkt.seg_len = POOL_PKT_LEN;
    params.pkt.len     = POOL_PKT_LEN;
    params.pkt.num     = POOL_NUM_PKT;
    params.type        = ODP_POOL_PACKET;
    pool = odp_pool_create("packet_pool", &params);
    if (pool == ODP_POOL_INVALID) {
        EXAMPLE_ERR("Error: packet pool create failed.\n");
        exit(EXIT_FAILURE);
    }
    odp_pool_print(pool);

    odp_pktio_t pktio;
    int ret;
    odp_pktio_param_t pktio_param;
    odp_pktin_queue_param_t pktin_param;
    odp_pktio_param_init(&pktio_param);
    pktio_param.in_mode = ODP_PKTIN_MODE_DIRECT;

    /* Open a packet IO instance */
    pktio = odp_pktio_open(dev, pool, &pktio_param);
    if (pktio == ODP_PKTIO_INVALID)
        EXAMPLE_ABORT("Error: pktio create failed for %s\n", dev);

    odp_pktin_queue_param_init(&pktin_param);
    pktin_param.op_mode = ODP_PKTIO_OP_MT_UNSAFE;

    if (odp_pktin_queue_config(pktio, &pktin_param))
        EXAMPLE_ABORT("Error: pktin config failed for %s\n", dev);


    odp_pktout_queue_param_t pktout_param;

    odp_pktout_queue_param_init(&pktout_param);
    unsigned int num_tx_queues = 1;
    pktout_param.num_queues = num_tx_queues;
    pktout_param.op_mode = ODP_PKTIO_OP_MT_UNSAFE;

    if (odp_pktout_queue_config(pktio, &pktout_param)) {
        EXAMPLE_ERR("Error: pktout queue config failed for %s\n", dev);
        exit(EXIT_FAILURE);
    }

    if (odp_pktin_queue_config(pktio, &pktin_param))
        EXAMPLE_ABORT("Error: pktin config failed for %s\n", dev);
    ret = odp_pktio_start(pktio);
    if (ret != 0)
        EXAMPLE_ABORT("Error: unable to start %s\n", dev);

    return pktio;
}


int main(int argc, char *argv[]) {
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
     * Create pktio
     */
    create_pktio();



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



static int run_thread(void *ptr){
    odp_pktin_queue_t pktin;
    odp_pktout_queue_t pktout;
    odp_pktio_t pktio;
    xfsm_table_t *xfsm = NULL;
    odp_shm_t shm = ODP_SHM_INVALID;
    uint32_t timestamp = 0;
    int MAX_ACKS = 16;
    odp_packet_t recv[MAX_ACKS];
    uint32_t *buf;
    uint32_t ackNo = 0, ts_echo = 0;
    xfsm_timer_t *ctp;
    timer_globals_t *gbls;

    gbls = ptr;

    pktio = odp_pktio_lookup("0");

    odp_pktout_queue(pktio, &pktout, 1);
    if (NULL == &pktout){
        EXAMPLE_ABORT("Error cannot retrieve pktout queue\n");
    }

    odp_pktin_queue(pktio, &pktin, 1);
    if (NULL == &pktin){
        EXAMPLE_ABORT("Error cannot retrieve pktin queue\n");
    }

    // calculate initial timestamp
    odp_time_t time = odp_time_global();
    uint64_t now = odp_time_to_ns(time);
    timestamp = now/1000000;


    set_timer(gbls, &gbls->tt[0], 1, 0, 1000000);
    EXAMPLE_DBG("setting timer\n");

    int events = 0;

    uint32_t time_us = 0;
    uint32_t n_pkts = 0;
    uint32_t init_rtt = 10000;
    uint32_t seq = 1;

    odp_pool_t packet_pool = odp_pool_lookup("packet_pool");
    odp_packet_t data_pkt = setup_tcp_pkt_ref(packet_pool, 1);

    while (1) {
        odp_event_t ev;
        odp_timeout_t timeout;

        // uint64_t sched_tmo = odp_schedule_wait_time(2000000000ULL);

        ev = odp_schedule(NULL, ODP_SCHED_NO_WAIT);

        if (ev == ODP_EVENT_INVALID) { continue; }
        else if (odp_event_type(ev) == ODP_EVENT_TIMEOUT) {
            /*
             * Process xfsm wake up on timer
             */
            timeout = odp_timeout_from_event(ev);
            ctp = odp_timeout_user_ptr(timeout);

            if (ctp == NULL)
                EXAMPLE_ERR("ctp is null\n");

            // send pkt

            odp_packet_t pkt = odp_packet_ref_static(data_pkt);

            pack_tcp_data(pkt, seq, timestamp);
            odp_pktout_send(pktout, &pkt, 1);
            seq+=1400;

            odp_timeout_free(timeout);

            // alloc new timeout

            odp_timeout_t tmo = odp_timeout_alloc(gbls->pool);
            ctp->ev = odp_timeout_to_event(tmo);

            // calculate pacing time
            n_pkts++;

            time_us = (uint32_t) (log2(1.0 + 1.0/n_pkts) * (double) init_rtt);

            EXAMPLE_DBG("\n Pacing time: %"PRIu32"\n", time_us);

            uint64_t ticks = odp_timer_ns_to_tick(gbls->timer_pool, time_us * ODP_TIME_USEC_IN_NS);
            odp_timer_set_rel(ctp->tim, ticks, &ctp->ev);

        }
    }

    return 0;
}