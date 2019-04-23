#include <stdlib.h>
#include <stdio.h>
#include <odp/helper/odph_api.h>
#include <inttypes.h>
#include "../src/tcp-ops.h"
#include "../src/xfsm-timer.h"


static int run_thread(void *ptr);

static odp_pktio_t create_pktio()
{
    char dev[] = "0";

    odp_pool_t pool;
    odp_pool_param_t params;
    odp_pool_param_init(&params);
#define POOL_NUM_PKT           2048  /* Number of packets in packet pool */
#define POOL_PKT_LEN           1856  /* Max packet  length */
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
    // pktio_param.in_mode = ODP_PKTIN_MODE_QUEUE;
    pktio_param.in_mode = ODP_PKTIN_MODE_SCHED;

    /* Open a packet IO instance */
    pktio = odp_pktio_open(dev, pool, &pktio_param);
    if (pktio == ODP_PKTIO_INVALID)
        EXAMPLE_ABORT("Error: pktio create failed for %s\n", dev);
    odp_pktin_queue_param_init(&pktin_param);
    pktin_param.queue_param.sched.sync = ODP_SCHED_SYNC_ATOMIC;
    if (odp_pktin_queue_config(pktio, &pktin_param))
        EXAMPLE_ABORT("Error: pktin config failed for %s\n", dev);

    odp_pktio_capability_t capa;
    if (odp_pktio_capability(pktio, &capa))
        EXAMPLE_ABORT("Error: Failed to get interface capabilities %s\n", dev);

    pktin_param.num_queues = capa.max_input_queues;
    pktin_param.queue_param.sched.sync = ODP_SCHED_SYNC_ATOMIC;

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
    printf("  created pktio:%02" PRIu64
                   ", dev:%s, queue mode (ATOMIC queues)\n"
                   "  \tdefault pktio%02" PRIu64 "\n",
           odp_pktio_to_u64(pktio), dev,
           odp_pktio_to_u64(pktio));

    return pktio;
}


int main(int argc, char *argv[]) {
    odph_odpthread_t thread_tbl[2];
    odp_instance_t instance;

    odp_queue_t queue;

    odp_queue_param_t queue_param;
    odp_pool_param_t pool_param;
    odp_timer_pool_param_t timer_pool_param;

    odph_odpthread_params_t thr_params;

    // something we have to pass to the worker thread
    timer_globals_t *gbls = NULL;
    odp_shm_t shm = ODP_SHM_INVALID;

    // assign worker to second cpu
    odp_cpumask_t mask;
    odp_cpumask_from_str(&mask, "0x2");

    // timer option parameters
    unsigned int resolution_us = 100;       // 100 micros
    unsigned int min_us = 100;              // 100 micros
    unsigned int max_us = 10000000;         // 10 seconds


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
    odp_queue_t queue, pktin;
    odp_pktout_queue_t pktout;
    odp_pktio_t pktio;
    timer_globals_t *gbls;
    uint32_t seq=0, mss=1000;
    uint8_t retx_count = 10;

    int res;

    xfsm_timer_t *ctp;

    pktio = odp_pktio_lookup("0");

    odp_pktout_queue(pktio, &pktout, 1);
    if (NULL == &pktout){
        EXAMPLE_ABORT("Error cannot retrieve pktout queue\n");
    }

    gbls = ptr;

    queue = odp_queue_lookup("timer_queue");

    //configure timers

    for (int i=0; i<20; i++){
        set_timer(ptr, &gbls->tt[i], seq, retx_count, 1000000*((uint)i+1));
        seq += mss;
    }
    odp_pool_t packet_pool = odp_pool_lookup("packet_pool");
    odp_packet_t pkt = setup_tcp_pkt_ref(packet_pool, 0);
    odp_packet_print(pkt);
    uint32_t pkts_recvd = 0;

    while (1) {
        odp_event_t ev;
        odp_timeout_t timeout;

        uint64_t sched_tmo = odp_schedule_wait_time(2000000000ULL);
        odp_event_t events[32] = {};
        int num_ev = odp_schedule_multi(NULL, ODP_SCHED_NO_WAIT, events, 32);
        if (events[0] == ODP_EVENT_INVALID)
            continue;
        if (odp_event_type(events[0]) == ODP_EVENT_TIMEOUT) {

            timeout = odp_timeout_from_event(events[0]);
            ctp = odp_timeout_user_ptr(timeout);
            if (ctp == NULL)
                EXAMPLE_DBG("ctp is null\n");
            uint64_t ticks = odp_timer_current_tick(gbls->timer_pool);
            uint64_t now = odp_timer_tick_to_ns(gbls->timer_pool, ticks);

            /*EXAMPLE_DBG("TIMEOUT received at %"PRIu64" micros, seqNo=%"PRIu32", retx_count="PRIu8"\n",
                        now / 1000, ctp->sequence_number, ctp->retx_count, ctp->retx_count);*/
            EXAMPLE_DBG("Pkts received = %"PRIu32"\n", pkts_recvd);

            odph_tcphdr_t *tcp = (odph_tcphdr_t *) odp_packet_l4_ptr(pkt, NULL);

            odph_udp_tcp_chksum(pkt, ODPH_CHKSUM_GENERATE, &tcp->cksm);
            odp_pktout_send(pktout, &pkt, 1);

            EXAMPLE_DBG("Sending pkt number %d\n", ctp->sequence_number/mss);
        } else if (odp_event_type(events[0]) == ODP_EVENT_PACKET){
            //pkt = odp_packet_from_event(ev);
            //EXAMPLE_DBG("received a packet!!\n");
            //odp_packet_print(pkt);
            pkts_recvd+=num_ev;
            odp_packet_t *pkts;
            pkts = malloc(sizeof(odp_packet_t)*num_ev);
            for (int j=0; j<num_ev; j++){
                pkts[j] = odp_packet_from_event(events[j]);
            }
            odp_packet_free_multi(pkts, num_ev);
        }
    }

    return 0;
}

