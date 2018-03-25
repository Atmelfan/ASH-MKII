//
// Created by atmelfan on 2018-03-21.
//

#ifndef SRC_CARRIER_I2C_H
#define SRC_CARRIER_I2C_H

#include <stdint.h>
#include <stdbool.h>
#include "dev.h"


typedef struct {
    device_t dev;
    struct {
        uint32_t sda, scl;
        uint32_t port;
    } pins;
    uint8_t own_address;
} i2c_ctr_dev_t;

typedef struct {
    device_t dev;
    uint8_t master;

} i2c_sub_dev_t;

bool i2c_init(i2c_ctr_dev_t* i2c);



#endif //SRC_CARRIER_I2C_H

