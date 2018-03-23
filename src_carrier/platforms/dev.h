//
// Created by atmelfan on 2018-03-21.
//

#ifndef SRC_CARRIER_DEV_H
#define SRC_CARRIER_DEV_H


#include <stdint.h>

typedef enum {
    DEV_TYPE_NULL = 0,
    DEV_TYPE_SERIAL,
    DEV_TYPE_I2C,
    DEV_TYPE_I2C_DEV,
    DEV_TYPE_SPI
} dev_type;

typedef struct {
    char* name;
    dev_type type;
    uint32_t phandle;
    uint32_t reg;
} dev_t;

#define DEV_T_INIT() (dev_t){.name = NULL, type = DEV_TYPE_NULL, .phandle = 0}

#endif //SRC_CARRIER_DEV_H
