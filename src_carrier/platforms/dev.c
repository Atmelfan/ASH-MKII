//
// Created by atmelfan on 2018-03-21.
//

#include "dev.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct dev_driver* drivers = NULL;
static struct device_t* devices = NULL;

#define REG_DRIVER(name) extern void reg_##name##_(struct dev_driver*); reg_##name##_(drivers)

void dev_register_driver(dev_driver* d){
    if(d == NULL)
        return;

    if(drivers == NULL){
        drivers = d;
    }else{
        dev_driver* t = drivers;
        for ( ; t->next; t = t->next) { }
        t->next = d;
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
    REG_DRIVER(stm32f4_gpio);
    REG_DRIVER(stm32f4_i2c);
    REG_DRIVER(pca9685);
}

dev_driver* dev_find_driver(char* compatible){
    if(!compatible)
        return NULL;

    for(struct dev_driver* c = drivers; c ; c = c->next){
        for(char** compat = c->compatible; *compat; compat++){
            if(!strcmp(*compat, compatible))
                return c;
        }
    }
    return NULL;
}

void dev_init_from_fdt( device_t* d, fdt_header_t* fdt, fdt_token* node, dev_type typ){
    if(d == NULL || fdt == NULL)
        return;

    d->name = node->name;
    d->reg = fdt_node_get_u32(fdt, node, "reg", NULL);
    d->phandle = fdt_node_get_u32(fdt, node, "phandle", 0);

    d->type = typ;

}