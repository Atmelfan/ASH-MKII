//
// Created by atmelfan on 2018-03-21.
//

#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <assert.h>
#include "../i2c.h"
#include "../dev.h"

#define I2C_BAUD 100000

#define I2C_CHECK_VALID(reg) ((reg) == I2C1 || (reg) == I2C2 || (reg) == I2C3)

bool i2c_init(i2c_ctr_dev_t i2c){

    if(!I2C_CHECK_VALID(i2c.dev.reg))
        return false;



    enum rcc_periph_clken clken;
    switch(i2c.dev.reg){
        case I2C1:
            clken = RCC_I2C1;
            break;
        case I2C2:
            clken = RCC_I2C2;
            break;
        case I2C3:
            clken = RCC_I2C3;
            break;
        default:
            return false;
    }
    rcc_periph_clock_enable(clken);


    uint32_t i2c_freq_mhz = rcc_apb1_frequency / 1000000;
    uint32_t ccr = rcc_apb1_frequency / I2C_BAUD / 2 + 1;
    if (ccr < 4)
        ccr = 4;

    i2c_reset(i2c.dev.reg);
    i2c_peripheral_disable(i2c.dev.reg);
    i2c_set_clock_frequency(i2c.dev.reg, i2c_freq_mhz);
    i2c_set_trise(i2c.dev.reg, i2c_freq_mhz + 1);
    i2c_set_ccr(i2c.dev.reg, ccr);
    I2C_CR1(i2c.dev.reg) = 0;
    I2C_OAR1(i2c.dev.reg) = 0x00000000;
    i2c_peripheral_enable(i2c.dev.reg);
    return true;
}
