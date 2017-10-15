//
// Created by atmelfan on 2017-10-13.
//

/**
 * Provides generic I2C interface functions
 * Use the device specific header for additional functionality/control
 */


#ifndef SRC_CARRIER_I2C_UTILS_H
#define SRC_CARRIER_I2C_UTILS_H

#include <stdint.h>

typedef uint8_t i2c_addr;


#define I2C_ADDR8(addr) (addr)          //8bit address with R/W bit
#define I2C_ADDR7(addr) ((addr) << 1)   //7bit address without R/W bit

typedef enum {
    /**
     * Device responded with ACk/No error
     */
            I2C_ACK = 0,

    /**
     * Device responded with NACK
     */
            I2C_NACK,

    /**
     * Something went wrong with the bus/invalid state
     */
            I2C_BUSERROR,

    /**
     * Unknown/device specific error
     */
            I2C_OTHER
} i2c_status;

/**
 * Device specific, implemented in relevant device .c file
 */
struct i2c_bus_struct;
typedef struct i2c_bus_struct i2c_bus;

/**
 * Device struct, containing address and i2c bus handle 
 */
typedef struct {
    i2c_addr addr;
    i2c_bus* bus;
} i2c_device;

/**
 * Initializes i2c system
 */
void i2c_init_all(void);

/**
 * Initializes specific i2c master unit to defaults, use device specific header for better control
 * @param i2c i2c master handle, device specific
 */
void i2c_init(i2c_bus* i2c);

/**
 * Initiates a connection with $addr
 * @param i2c I2c unit to use
 * @param addr i2c device to connect to
 * @return I2C_ACK if successfull
 */
i2c_status i2_start(i2c_bus* i2, i2c_addr addr);

/**
 * Writes a byte to $addr (must have called i2c_start first)
 * @param i2c I2c unit to use
 * @param data Byte to send
 * @return I2C_ACK if successfull
 */
i2c_status i2c_write(i2c_bus* i2c, uint8_t data);

/**
 * Reads a byte and responds with ACk
 * @param i2c I2c unit to use
 * @return Byte read
 */
uint8_t i2c_read_ack(i2c_bus* i2c);

/**
 * Reads a byte and responds with NACk
 * @param i2c I2c unit to use
 * @return Byte read
 */
uint8_t i2c_read_nack(i2c_bus* i2c);

/**
 * Closes connection
 * @param i2c I2c unit to use
 */
void i2_stop(i2c_bus* i2c);

/**
 * Send data to $addr
 * @param i2c I2c unit to use
 * @param len Number of bytes to send
 * @param data Pointer to bytes
 */
void i2c_wburst(i2c_bus* i2c, i2c_addr addr, uint8_t len, uint8_t* data);

/**
 * Read data from $addr
 * @param i2c I2c unit to use
 * @param len Number of bytes to read
 * @param data Pointer to bytes
 */
void i2c_rburst(i2c_bus* i2c, i2c_addr addr, uint8_t len, uint8_t* data);

/**
 * Write then read to/from $addr
 * @param i2c I2c unit to use
 * @param wlen Number of bytes to send
 * @param wdata Pointer to bytes to send
 * @param rlen Number of bytes to read
 * @param rdata Pointer to bytes to read
 */
void i2c_rwburst(i2c_bus* i2c, i2c_addr addr, uint8_t wlen, uint8_t* wdata, uint8_t rlen, uint8_t* rdata);

#endif //SRC_CARRIER_I2C_UTILS_H
