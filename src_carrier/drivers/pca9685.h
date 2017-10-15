//
// Created by atmelfan on 2017-10-13.
//

#ifndef SRC_CARRIER_PCA9685_H
#define SRC_CARRIER_PCA9685_H

#include "i2c/i2c_generic.h"

typedef uint16_t (*pca9685_callback)(uint8_t);

#define PCA9685_MODE1_ALLCALL    0x01 //Enable call all address
#define PCA9685_MODE1_SUBCALL1   0x02 //Enable sub address 1
#define PCA9685_MODE1_SUBCALL2   0x04 //Enable sub address 2
#define PCA9685_MODE1_SUBCALL3   0x08 //Enable sub address 3
#define PCA9685_MODE1_SLEEP      0x10 //Enter sleep mode
#define PCA9685_MODE1_AUTOINCR   0x20
#define PCA9685_MODE1_EXTCLK     0x40
#define PCA9685_MODE1_RESTART    0x80

#define PCA9685_MODE2_OUTNE0     0x00
#define PCA9685_MODE2_OUTNE1     0x01
#define PCA9685_MODE2_OUTNE2     0x02
#define PCA9685_MODE2_OUTDRV     0x04
#define PCA9685_MODE2_OCH        0x08
#define PCA9685_MODE2_INVRT      0x10

#define PCA9685_LED_FULL    0x1000 //LED full on/off bit

/**
 * Initializes PCA9685
 * @param addr Address of pca9685
 * @param mode1 Initial mode1 register
 * @param mode2 Initial mode2 register
 */
void pca9685_init(const i2c_device* dev, uint8_t mode1, uint8_t mode2);

/**
 * Clock prescaler
 * @param addr Address of pca9685
 * @param prescale Sets prescale value, clk = (25MHz/(4096*update_rate)) - 1
 */
void pca9685_prescale(const i2c_device* dev, uint8_t prescale);

/**
 * Enter sleep mode
 * @param addr Address of pca9685
 * @param wake Will enter sleep if wake == 0, wake up otherwise
 */
void pca9685_sleep(const i2c_device* dev, uint8_t wake);

/**
 * Update a LED pwm output
 * @param addr Address of pca9685
 * @param led Index of LED to update
 * @param on On value (see datasheet)
 * @param off Off value (see datasheet)
 */
void pca9685_update(const i2c_device* dev, uint8_t led, uint16_t on, uint16_t off);

/**
 * Update multiple LED outputs with callback function
 * Callback function should be of uint16_t callback(uint8_t i), where i is index of output
 * @param addr Address of pca9685
 * @param start First index
 * @param end Last index
 * @param led_on Callback, should return on value (see datasheet)
 * @param led_off Callback, should return off value (see datasheet)
 */
void pca9685_update_cb(const i2c_device* dev, uint8_t start, uint8_t end, pca9685_callback led_on, pca9685_callback led_off);

#endif //SRC_CARRIER_PCA9685_H
