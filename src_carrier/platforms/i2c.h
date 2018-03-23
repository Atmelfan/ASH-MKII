//
// Created by atmelfan on 2018-03-21.
//

#ifndef SRC_CARRIER_I2C_H
#define SRC_CARRIER_I2C_H

#include <stdint.h>
#include <stdbool.h>
#include "dev.h"


typedef struct {
    dev_t dev;
    struct {
        uint32_t sda, scl;
        uint32_t port;
    } pins;
    uint8_t own_address;
} i2c_ctr_dev_t;

typedef struct {
    dev_t dev;
    struct {
        uint32_t sda, scl;
        uint32_t port;
    } pins;
    uint8_t own_address;
} i2c_sub_dev_t;



#endif //SRC_CARRIER_I2C_H

