//
// Created by atmelfan on 2017-09-25.
//

#include <stdio.h>
#include "leg.h"
#include "math/linalg.h"
#include "fdt/dtb_parser.h"

void leg_set_relative(leg_t* l, const vec4* position){
    vec_mov((vecx*)&l->target_position, (vecx*)&position);
}

void leg_set_absolute(leg_t* l, const vec4* position){
    vec4 rel = VEC4( );
    vecmat_mul((matxx*)&l->transform, (vecx*)&position, (vecx*)&rel);
    leg_set_relative(l, &rel);
}

void leg_get_relative(leg_t* l, vec4* position){
    vec_mov((vecx*)&l->current_position, (vecx*)position);
}

void leg_update(leg_t* l, float dt){
    vec4 pos_error = VEC4();
    //vec4 pos_actual = read_pos(); //TODO: get actual feedback instead of assuming instantaneous travel between updates
    vec_sub((vecx*)&l->target_position, (vecx*)&l->current_position, (vecx*)&pos_error);//E = S - P
    vec_scalel((vecx*)&pos_error, E_FACTOR);//E = E*Ke
    vec_addl((vecx*)&l->current_position, (vecx*)&pos_error);//P[n+1] = P[n] + (S[n] - P[n])*Ke

    //Do IK calculations from P[n+1]
    //Update servos
}

uint8_t leg_get(const leg_t* l, uint8_t mask){
    return l->status & mask;
}

bool leg_from_node(leg_t* l, fdt_header_t* fdt, fdt_token* node){
    if(fdt_token_get_type(node) != FDT_BEGIN_NODE)
        return false;

    /*Read transform matrix*/
    fdt_token* matrix = fdt_node_get_prop(fdt, node, "transform", false);
    l->transform = MAT4_ZERO();
    if(matrix == NULL){
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
    l->home_position = VEC4_ZERO();
    for(int i = 0; i < 3; ++i){
        l->home_position.members[i] = ((int32_t)home->cells[i])/1000.0f;
    }
    l->home_position.members[3] = 1.0f;

    l->initialized = true;
    return true;
}
