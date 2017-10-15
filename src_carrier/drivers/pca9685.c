//
// Created by atmelfan on 2017-10-13.
//

#include "pca9685.h"
#include "i2c/i2c_generic.h"

/**
 * Initializes PCA9685
 * @param addr Address of pca9685
 * @param mode1 Initial mode1 register
 * @param mode2 Initial mode2 register
 */
void pca9685_init(const i2c_device* dev, uint8_t mode1, uint8_t mode2){
    uint8_t tx[3] = {0x00, mode1, mode2};
    i2c_wburst(dev->bus, dev->addr, 3, tx);
}

/**
 * Clock prescaler
 * @param addr Address of pca9685
 * @param prescale Sets prescale value, clk = (25MHz/(4096*update_rate)) - 1
 */
void pca9685_prescale(const i2c_device* dev, uint8_t prescale){
    i2c_wburst(dev->bus, dev->addr, 1, &prescale);
}

/**
 * Enter sleep mode
 * @param addr Address of pca9685
 * @param wake Will enter sleep if wake == 0, wake up otherwise
 */
void pca9685_sleep(const i2c_device* dev, uint8_t wake){

}

/**
 * Update a LED pwm output
 * @param addr Address of pca9685
 * @param led Index of LED to update
 * @param on On value (see datasheet)
 * @param off Off value (see datasheet)
 */
void pca9685_update(const i2c_device* dev, uint8_t led, uint16_t on, uint16_t off){

}

/**
 * Update multiple LED outputs with callback function
 * Callback function should be of uint16_t callback(uint8_t i), where i is index of output
 * @param addr Address of pca9685
 * @param start First index
 * @param end Last index
 * @param led_on Callback, should return on value (see datasheet)
 * @param led_off Callback, should return off value (see datasheet)
 */
void pca9685_update_cb(const i2c_device* dev, uint8_t start, uint8_t end, pca9685_callback led_on, pca9685_callback led_off){

}