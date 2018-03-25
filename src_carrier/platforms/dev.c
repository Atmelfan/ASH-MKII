//
// Created by atmelfan on 2018-03-21.
//

#include "dev.h"
#include <stdlib.h>
#include <string.h>

static struct dev_driver* drivers = NULL;
static struct device_t* devices = NULL;

#define REG_DRIVER(name) extern void reg_##name##_(struct dev_driver*); reg_##name##_(drivers)

void dev_register_driver(dev_driver* d){
    if(d == NULL)
        return;

    if(drivers == NULL){
        drivers = d;
    }else{
        drivers->next = d;
    }
    d->next = NULL;
}

void dev_register_device( device_t* d){
    if(d == NULL)
        return;

    if(drivers == NULL){
        devices = d;
    }else{
        devices->next = d;
    }
    d->next = NULL;
}

void dev_init_drivers(){
    REG_DRIVER(pca9685);
}

dev_driver* dev_find_driver(char* compatible){
    for(struct dev_driver* c = drivers; c ; c = c->next){
        for(char** compat = c->compatible; *compat; compat++){
            if(strcmp(*compat, compatible))
                return c;
        }
    }
    return NULL;
}