#include "odp_api.h"
#include "example_debug.h"

#ifndef XFSM_TIMER_H
#define XFSM_TIMER_H

#define NUM_TMOS    100000
#define MAX_TIMERS  100000
#define TIMER_RES   2


typedef struct xfsm_timer {
    odp_timer_t tim;
    odp_event_t ev;
    uint32_t sequence_number;
    uint8_t retx_count;
    uint32_t *flow_key;
} xfsm_timer_t;

typedef struct timer_globals {
    odp_pool_t pool;
    odp_timer_pool_t timer_pool;
    xfsm_timer_t tt[MAX_TIMERS];
    uint32_t last_timer_id;
} timer_globals_t;

/**
 * Create a timer and schedule a timeout
 * @param ptr
 * @param timer_p
 * @param seq
 * @param retx_c
 * @param time_us
 * @return 0 -> ok; >0 ->error
 */
int set_timer(void *ptr, xfsm_timer_t *timer_p, uint32_t seq, uint8_t retx_c, uint32_t time_us, uint32_t *flow_key);

#endif //ODP_XFSM_XFSM_TIMER_H
