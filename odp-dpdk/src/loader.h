#ifndef ODP_XFSM_LOADER_H
#define ODP_XFSM_LOADER_H

#include "cJSON.h"
#include "xfsm.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#define RETURN_ERROR -1
#define RETURN_SUCCESS 0
#define CUSTOM_OFFSET 1 // Where to start allocating the registers
#define XFSM_NEXT_STATE 2

xfsm_table_t *json_to_table(cJSON *table, timer_globals_t *globals, xfsm_pktio_params_t *pktout, lookup_key_t *lookup_key);

#endif //ODP_XFSM_LOADER_H
