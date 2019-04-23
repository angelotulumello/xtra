//
// Created by angelo on 26/04/18.
//

#include <odp.h>
#include "pktio.h"
#include "example_debug.h"
#include "session.h"


static void pktio_params_configure(xfsm_pktio_params_t *xfsm_pktio_params, uint8_t num_pktio) {
    xfsm_pktio_params->num_pktios = num_pktio;

    for (int i=0; i<MAX_DEV; i++){
        if (i<num_pktio) {
            char j = (char) ('0' + i);
            sprintf(xfsm_pktio_params->devs[i], "%c", j);
            sprintf(xfsm_pktio_params->pkt_pools[i], POOL_NAME "%c", j);
        } else {
            memset(xfsm_pktio_params->devs[i], 0, DEV_NAME_SZ);
            memset(xfsm_pktio_params->pkt_pools[i], 0, POOL_NAME_SZ);
        }
    }
}

static void create_pkt_pool(xfsm_pktio_params_t *xfsm_pktio_params) {
    for (int i=0; i<xfsm_pktio_params->num_pktios; i++){
        odp_pool_t pool;
        odp_pool_param_t params;

        odp_pool_param_init(&params);

        params.pkt.seg_len = POOL_PKT_LEN;
        params.pkt.len     = POOL_PKT_LEN;
        params.pkt.num     = POOL_NUM_PKT;
        params.type        = ODP_POOL_PACKET;
        pool = odp_pool_create(xfsm_pktio_params->pkt_pools[i], &params);

        if (pool == ODP_POOL_INVALID) {
            EXAMPLE_ERR("Error: packet pool create failed.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void create_pktio(xfsm_pktio_params_t *xfsm_pktio_params, uint8_t num_pktios) {
    pktio_params_configure(xfsm_pktio_params, num_pktios);

    create_pkt_pool(xfsm_pktio_params);

    create_storage_pool();

    for (int i=0; i<xfsm_pktio_params->num_pktios; i++){
        odp_pktio_t pktio;
        odp_pool_t pool;
        int ret;
        odp_pktio_param_t pktio_param;
        odp_pktin_queue_param_t pktin_param;
        odp_pktio_param_init(&pktio_param);
        pktio_param.in_mode = ODP_PKTIN_MODE_DIRECT;

        pool = odp_pool_lookup(xfsm_pktio_params->pkt_pools[i]);
        if (pool == ODP_POOL_INVALID)
            EXAMPLE_ABORT("Error: pool invalid\n");

        /* Open a packet IO instance */
        pktio = odp_pktio_open(xfsm_pktio_params->devs[i], pool, &pktio_param);
        if (pktio == ODP_PKTIO_INVALID)
            EXAMPLE_ABORT("Error: pktio create failed for %s\n", xfsm_pktio_params->devs[i]);

        odp_pktin_queue_param_init(&pktin_param);
        pktin_param.op_mode = ODP_PKTIO_OP_MT_UNSAFE;

        if (odp_pktin_queue_config(pktio, &pktin_param))
            EXAMPLE_ABORT("Error: pktin config failed for %s\n", xfsm_pktio_params->devs[i]);

        odp_pktout_queue_param_t pktout_param;
        odp_pktio_config_t config;

        odp_pktio_config_init(&config);

        /*
         * Create pktout queues
         */
        odp_pktout_queue_param_init(&pktout_param);
        unsigned int num_tx_queues = 1;
        pktout_param.num_queues = num_tx_queues;
        pktout_param.op_mode = ODP_PKTIO_OP_MT_UNSAFE;
        /*
        config.pktout.bit.ipv4_chksum_ena = 1;
        config.pktout.bit.tcp_chksum_ena = 1;
        config.pktout.bit.udp_chksum_ena = 1;
        */
        if (odp_pktio_config(pktio, &config)) {
            EXAMPLE_ERR("Error: cannot set ip and tcp chksum offload %s\n", xfsm_pktio_params->devs[i]);
            exit(EXIT_FAILURE);
        }

        if (odp_pktout_queue_config(pktio, &pktout_param)) {
            EXAMPLE_ERR("Error: pktout queue config failed for %s\n", xfsm_pktio_params->devs[i]);
            exit(EXIT_FAILURE);
        }

        if (odp_pktin_queue_config(pktio, &pktin_param))
            EXAMPLE_ABORT("Error: pktin config failed for %s\n", xfsm_pktio_params->devs[i]);
        ret = odp_pktio_start(pktio);
        if (ret != 0)
            EXAMPLE_ABORT("Error: unable to start %s\n", xfsm_pktio_params->devs[i]);

    }

}

static void create_storage_pool(){
    odp_pool_t pool;
    odp_pool_param_t params;

    odp_pool_param_init(&params);

    params.pkt.seg_len = POOL_PKT_LEN;
    params.pkt.len     = POOL_PKT_LEN;
    params.pkt.num     = INSTANCE_DB_CAPA * PKT_STORE_SIZE;
    params.type        = ODP_POOL_PACKET;
    pool = odp_pool_create(STORAGE_POOL_NAME, &params);

    if (pool == ODP_POOL_INVALID) {
        EXAMPLE_ERR("Error: packet pool create failed.\n");
        exit(EXIT_FAILURE);
    }
}

