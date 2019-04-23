//
// Created by angelo on 30/05/18.
//

#include "session.h"


void
biflow(uint32_t *key, lookup_key_t *lookup_flds) {
    uint32_t tmp;

    EXAMPLE_DBG("\n\tBiflow handling...\n");

    for (int i=0; i<lookup_flds->biflow_no * 2; i+=2){
        if (key[i] > key[i+1]) {
            tmp = key[i];
            key[i] = key[i+1];
            key[i+1] = tmp;
            EXAMPLE_DBG("\t\tSwapped fields\n");
        }
    }
}


uint32_t
*key_extract( xfsm_table_t *xfsm) {
    uint32_t *key;
    hdr_fields fld;

    EXAMPLE_DBG("\nExtracting key... key size is: %d\n", xfsm->lookup_key->size);

    key = malloc(sizeof(uint32_t) * xfsm->lookup_key->size);

    for (int i=0; i<xfsm->lookup_key->size; i++){
        fld = xfsm->lookup_key->fld[i];
        if (xfsm->pkt_fields[fld].value == NULL){
            EXAMPLE_DBG("Lookup fields not found in packet header\n");
            return NULL;
        } else {
            key[i] = (uint32_t) get_hdr_value(xfsm->pkt_fields[fld]);
            EXAMPLE_DBG("\tfield %d: %u\n", i, key[i]);
        }
    }

    if (xfsm->lookup_key->biflow_no > 0)
        biflow(key, xfsm->lookup_key);

    EXAMPLE_DBG("\nKey extracted\n");

    return key;
}

xfsm_instance_t
*lookup_xfsm_instance(uint32_t *key, odph_table_t table) {
    odph_table_ops_t *ops = &odph_hash_table_ops;
    uint64_t out = 0;
    EXAMPLE_DBG("\nLookup xfsm instance in table\n");
    int ret = 0;

    ret = ops->f_get(table, key, &out, sizeof(uint64_t));

    EXAMPLE_DBG("Return value is %d, instance is %lu\n", ret, out);

    if (ret < 0)
        return NULL;

    return (xfsm_instance_t *) out;
}


int
insert_xfsm_instance(uint32_t *key, xfsm_instance_t *xfsmi, odph_table_t table){
    odph_table_ops_t *ops = &odph_hash_table_ops;
    int res = 0;

    EXAMPLE_DBG("\nInserting xfsm instance in table, xfsmi = %lu\n", (uint64_t) xfsmi);

    res = ops->f_put(table, key, &xfsmi);

    if (res < 0){
        EXAMPLE_ERR("Problems with inserting xfsm instance\n");
        return -1;
    }

    EXAMPLE_DBG("Inserted xfsm instance in table, res = %d\n", res);

    return 0;
}

odph_table_t
create_instance_db(lookup_key_t *lookup_key){
    odph_table_t table;
    odph_table_ops_t *ops = &odph_hash_table_ops;

    EXAMPLE_DBG("\nCreating instance database\n");

    table = ops->f_create(INSTANCE_DB_NAME, INSTANCE_DB_CAPA,
                              sizeof(uint32_t) * lookup_key->size, sizeof(xfsm_instance_t *));

    EXAMPLE_DBG("sizeof xfsm instance is %lu\n", sizeof(xfsm_instance_t*));

    return table;
}

