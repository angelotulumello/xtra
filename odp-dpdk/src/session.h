//
// Created by angelo on 30/05/18.
//

#ifndef ODP_XFSM_SESSION_H
#define ODP_XFSM_SESSION_H

#include "xfsm.h"

#define INSTANCE_DB_NAME "instance_db"
#define INSTANCE_DB_CAPA 100

uint32_t *
key_extract(xfsm_table_t *xfsm);


xfsm_instance_t *
lookup_xfsm_instance(uint32_t *key, odph_table_t table);


int
insert_xfsm_instance(uint32_t *key, xfsm_instance_t *xfsmi, odph_table_t table);


odph_table_t
create_instance_db(lookup_key_t *lookup_key);

void biflow(uint32_t *key, lookup_key_t *lookup_flds);


#endif //ODP_XFSM_SESSION_H
