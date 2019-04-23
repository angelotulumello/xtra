#include <stdlib.h>
#include <stdio.h>
#include <odp/helper/odph_api.h>
#include <inttypes.h>
#include "../src/xfsm-timer.h"


static int run_thread(void *ptr);

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
    unsigned int resolution_us = 10;       // 1000 micros
    unsigned int min_us = 10;              // 1000 micros
    unsigned int max_us = 50000000;         // 5 seconds


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

    odp_queue_destroy(queue);
    odp_pool_destroy(gbls->pool);
    odp_timer_pool_destroy(gbls->timer_pool);
    odp_shm_free(shm);


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
    odp_queue_t queue;
    timer_globals_t *gbls;
    uint32_t seq=0, mss=1000;
    uint8_t retx_count = 10;
    int res;

    xfsm_timer_t *ctp;

    gbls = ptr;

    queue = odp_queue_lookup("timer_queue");

    //configure timers

    for (int i=0; i<200; i++){
        set_timer(ptr, &gbls->tt[i], seq, retx_count, 10000*((uint)i+1));
        seq += mss;
    }

    while (1) {
        odp_event_t ev;
        odp_timeout_t timeout;

        uint64_t sched_tmo = odp_schedule_wait_time(2000000000ULL);

        ev = odp_schedule(&queue, sched_tmo);
        if (ev == ODP_EVENT_INVALID)
            break;

        timeout = odp_timeout_from_event(ev);
        ctp = odp_timeout_user_ptr(timeout);
        if (ctp==NULL)
            EXAMPLE_DBG("ctp is null\n");
        uint64_t ticks = odp_timer_current_tick(gbls->timer_pool);
        uint64_t now = odp_timer_tick_to_ns(gbls->timer_pool, ticks);

        EXAMPLE_DBG("TIMEOUT received at %"PRIu64"us, seqNo=%"PRIu32", retx_count="PRIu8"\n",
                    now/1000, ctp->sequence_number, ctp->retx_count);
    }



    return 0;
}

