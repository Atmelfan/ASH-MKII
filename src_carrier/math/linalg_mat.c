//
// Created by atmelfan on 2017-10-12.
//
#include "linalg.h"

/**MOVES CONTENT OF r INTO l**/
void mat_mov(matxx* l, const matxx* r){
    //const uint8_t W = l->w;
    const uint8_t t = l->w*l->h;
    for(int i = 0; i < t; ++i){
        l->members[i] = r->members[i];
    }
}

/**ADDITION**/
void mat_add(const matxx* l, const matxx* r, matxx* o){
    const uint8_t t = l->w*l->h;
    for(int i = 0; i < t; ++i){
        o->members[i] = l->members[i] + r->members[i];
    }
}


/**SUBTRACTION**/
void mat_sub(const matxx* l, const matxx* r, matxx* o){
    const uint8_t t = l->w*l->h;
    for(int i = 0; i < t; ++i){
        o->members[i] = l->members[i] - r->members[i];
    }
}


/**MULTIPLICATION**/
void mat_mul(const matxx* l, const matxx* r, matxx* o){
    const uint8_t t = l->w*l->h;
    for(int i = 0; i < t; ++i){
        o->members[i] = l->members[i] * r->members[i];
    }
}


/**DIVISION**/
void mat_div(const matxx* l, const matxx* r, matxx* o){
    const uint8_t t = l->w*l->h;
    for(int i = 0; i < t; ++i){
        o->members[i] = l->members[i] / r->members[i];
    }
}


