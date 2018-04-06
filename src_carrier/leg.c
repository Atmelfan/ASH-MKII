//
// Created by atmelfan on 2017-09-25.
//

#include <stdio.h>
#include "leg.h"
#include "math/linalg.h"
#include "fdt/dtb_parser.h"
#include "platforms/log.h"

bool leg_from_node(leg_t* l, fdt_header_t* fdt, fdt_token* node){
    if(fdt_token_get_type(node) != FDT_BEGIN_NODE)
        return false;

    /*Read transform matrix*/
    fdt_token* matrix = fdt_node_get_prop(fdt, node, "transform", false);


    if(matrix == NULL){
        logd_printfs(LOG_ERROR, "no transform");
        return false;
    }
    for(int i = 0; i < 16; ++i){
        l->transform.members[i] = ((int32_t)matrix->cells[i])/1000.0f;
    }

    /*Read home vector*/
    fdt_token* home = fdt_node_get_prop(fdt, node, "home", false);
    if(home == NULL){
        return false;
    }

    for(int i = 0; i < 3; ++i){
        l->home_position.members[i] = ((int32_t)fdt_read_u32(&home->cells[i]))/1000.0f;
    }
    l->home_position.members[3] = 1.0f;

    l->ready = true;
    return true;
}

void leg_init(leg_t *l) {
    /* Initialize leg */
    l->home_position = VEC4_ZERO();
    l->transform = MAT4_ZERO();
    for (int j = 0; j < 3; ++j) {
        l->servo_offsets_100[j] = 0;
    }
    l->ready = false;
}
