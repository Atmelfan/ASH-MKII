//
// Created by atmelfan on 2017-10-13.
//

/**
 * Provides generic I2C interface functions
 * Use the device specific header for additional functionality
 */

#include <sched.h>

#ifndef SRC_CARRIER_I2C_UTILS_H
#define SRC_CARRIER_I2C_UTILS_H

typedef uint8_t i2c_addr;

#define I2C_ADDR8(addr) (addr)          //8bit address with R/W bit
#define I2C_ADDR7(addr) ((addr) << 1)   //7bit address without R/W bit

typedef uint8t i2c_type;

/**
 * Initializes i2c system
 */
void i2c_init_all();

void i2c_init(i2c_type* i2c);

#endif //SRC_CARRIER_I2C_UTILS_H
