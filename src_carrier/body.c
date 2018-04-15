//
// Created by atmelfan on 2018-04-15.
//

#include "body.h"

void body_reset(body_t* body){
    body->temp = MAT4_IDENT();
}

void body_commit(body_t* body){
    body->model = body->temp;
}

void body_rotate(body_t* body, vec4* rot){

}

void body_translate(body_t* body, vec4* tra){

}

void body_init(body_t* body){
    body->model = MAT4_IDENT();
    body_reset(body);
}
