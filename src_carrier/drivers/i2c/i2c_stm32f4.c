//
// Created by atmelfan on 2017-10-13.
//

#include "i2c_generic.h"
#include "i2c_stm32f4.h"
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

struct i2c_bus_struct {
    uint32_t i2c;//Libopencm3 I2C handles can be cast to i2c_bus
};


/**
 * Initializes i2c system
 */
void i2c_init_all(){

}

/**
 * Initializes specific i2c master unit
 * @param i2c i2c master handle, device specific
 */
void i2c_init(i2c_bus *i2c){
    i2c_reset(i2c->i2c);
    i2c_peripheral_disable(i2c->i2c);
    //configure ANFOFF DNF[3:0] in CR1
    i2c_set_speed(i2c->i2c, I2C_DEFAULT_SPEED, I2C_DEFAULT_CLK_FREQ_MHZ);//100kHz @ clk=8MHz
    //configure No-Stretch CR1 (only relevant in slave mode)
    i2c_disable_dual_addressing_mode(i2c->i2c);
    i2c_peripheral_enable(i2c->i2c);
}

/**
 * Initiates a connection with $addr
 * @param i2c I2c unit to use
 * @param addr i2c device to connect to
 * @return I2C_ACK if successfull
 */
i2c_status i2_start(i2c_bus* i2, i2c_addr addr){
    /*Not implemented*/
}

/**
 * Writes a byte to $addr (must have called i2c_start first)
 * @param i2c I2c unit to use
 * @param data Byte to send
 * @return I2C_ACK if successfull
 */
i2c_status i2c_write(i2c_bus* i2c, uint8_t data){
    /*Not implemented*/
}

/**
 * Reads a byte and responds with ACk
 * @param i2c I2c unit to use
 * @return Byte read
 */
uint8_t i2c_read_ack(i2c_bus* i2c){
    /*Not implemented*/
}

/**
 * Reads a byte and responds with NACk
 * @param i2c I2c unit to use
 * @return Byte read
 */
uint8_t i2c_read_nack(i2c_bus* i2c){
    /*Not implemented*/
}

/**
 * Closes connection
 * @param i2c I2c unit to use
 */
void i2_stop(i2c_bus* i2c){
    /*Not implemented*/
}


/**
 * Send data to $addr
 * @param i2c I2c unit to use
 * @param len Number of bytes to send
 * @param data Pointer to bytes
 */
void i2c_wburst(i2c_bus* i2c, i2c_addr addr, uint8_t len, uint8_t* data){
    i2c_transfer7(i2c->i2c, addr, data, len, NULL, 0);
}

/**
 * Read data from $addr
 * @param i2c I2c unit to use
 * @param len Number of bytes to read
 * @param data Pointer to bytes
 */
void i2c_rburst(i2c_bus* i2c, i2c_addr addr, uint8_t len, uint8_t* data){
    i2c_transfer7(i2c->i2c, addr, NULL, 0, data, len);
}

/**
 * Write then read to/from $addr
 * @param i2c I2c unit to use
 * @param wlen Number of bytes to send
 * @param wdata Pointer to bytes to send
 * @param rlen Number of bytes to read
 * @param rdata Pointer to bytes to read
 */
void i2c_rwburst(i2c_bus* i2c, i2c_addr addr, uint8_t wlen, uint8_t* wdata, uint8_t rlen, uint8_t* rdata){
    i2c_transfer7(i2c->i2c, addr, wdata, wlen, rdata, rlen);
}
