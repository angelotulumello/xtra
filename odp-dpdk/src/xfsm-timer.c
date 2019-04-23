#include "odp_api.h"
#include "xfsm-timer.h"

int set_timer(void *ptr, xfsm_timer_t *timer_p, uint32_t seq, uint8_t retx_c, uint32_t time_us, uint32_t *flow_key) {
    odp_queue_t queue;
    timer_globals_t *gbls;
    odp_timeout_t tmo;

    gbls = ptr;

    // fetch the queue;
    queue = odp_queue_lookup("timer_queue");

    timer_p->tim = odp_timer_alloc(gbls->timer_pool, queue, timer_p);
    if (timer_p->tim == ODP_TIMER_INVALID) {
        //EXAMPLE_ERR("Failed to allocate timer\n");
        return -1;
    }
    tmo = odp_timeout_alloc(gbls->pool);
    if (tmo == ODP_TIMEOUT_INVALID) {
        EXAMPLE_ERR("Failed to allocate timeout\n");
        return -1;
    }

    timer_p->ev = odp_timeout_to_event(tmo);

    uint64_t ticks = odp_timer_ns_to_tick(gbls->timer_pool, time_us * ODP_TIME_USEC_IN_NS);
    odp_timer_set_rel(timer_p->tim, ticks, &timer_p->ev);

    timer_p->retx_count = retx_c;
    timer_p->sequence_number = seq;
    timer_p->flow_key = flow_key;

    return 0;
}
