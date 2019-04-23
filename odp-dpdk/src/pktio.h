//
// Created by angelo on 26/04/18.
//

#ifndef ODP_XFSM_PKTIO_H
#define ODP_XFSM_PKTIO_H

#include <odp/helper/odph_api.h>

#define MAX_DEV 4
#define POOL_NUM_PKT           8192  /* Number of packets in packet pool */
#define POOL_PKT_LEN           1856  /* Max packet length */
#define POOL_NAME              "packet_pool"
#define POOL_NAME_SZ           13
#define DEV_NAME_SZ            2

#define STORAGE_POOL_NAME      "storage_pool"

typedef struct xfsm_pktio_params {
    uint8_t num_pktios;
    char devs[MAX_DEV][DEV_NAME_SZ];
    char pkt_pools[MAX_DEV][POOL_NAME_SZ];
} xfsm_pktio_params_t;

static void pktio_params_configure(xfsm_pktio_params_t *xfsm_pktio_params, uint8_t num_pktio);

void create_pktio(xfsm_pktio_params_t *xfsm_pktio_params, uint8_t num_pktios);

static void create_pkt_pool(xfsm_pktio_params_t *xfsm_pktio_params);

static void create_storage_pool();

#endif //ODP_XFSM_PKTIO_H
