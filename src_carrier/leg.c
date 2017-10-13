//
// Created by atmelfan on 2017-09-25.
//

#include "leg.h"

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