//
// Created by atmelfan on 2018-03-21.
//

#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <assert.h>
#include <stdio.h>
#include "../i2c.h"
#include "../dev.h"
#include "../../fdt/dtb_parser.h"

#define I2C_BAUD 100000
#define I2C_CHECK_VALID(reg) ((reg) == I2C1 || (reg) == I2C2 || (reg) == I2C3)

DEV_PROBE_CALLBACK(stm32f4_i2c, fdt, node, parent){

    i2c_ctr_dev_t* i2c = malloc(sizeof(i2c_ctr_dev_t));

    /*Generic device info*/
    dev_init_from_fdt(&i2c->dev, fdt, node, DEV_TYPE_I2C);

    uint32_t addr = fdt_node_get_u32(fdt, node, "addr", 0x00);
    if(addr > UINT8_MAX){
        printf("[ERROR] I2C: addr out of range");
        free(i2c);
        return PROBE_FAIL;
    }
    i2c->own_address = (uint8_t)addr;

    /*TODO: Fix GPIO API & PINS!*/


    for(fdt_token* d = fdt_token_next(fdt, node); fdt_token_get_type(d) != FDT_END_NODE ; d = fdt_token_next(fdt, d)){
        /*Check for nodes*/
        if(fdt_token_get_type(d) == FDT_BEGIN_NODE){
            char* compatible = fdt_node_get_str(fdt, d, "compatible", NULL);
            dev_driver* drv = dev_find_driver(compatible);
            if(drv){
                if(drv->probe(fdt, d, i2c->dev.phandle) == PROBE_SUCCESS){

                }else{
                    char buf[100];
                    printf("[ERROR] Failed to probe %s!\n", fdt_trace(fdt, d, buf));
                }
            }else{
                char buf[100];
                printf("[ERROR] No driver for %s!\n", fdt_trace(fdt, d, buf));
            }


            /*Exit node*/
            d = fdt_node_end(fdt, d);
        }
    }

    if(i2c_init(i2c)){
        dev_register_device(&i2c->dev);
        return PROBE_SUCCESS;
    }

    free(i2c);
    return PROBE_FAIL;
}

DEV_COMPAT_LIST(stm32f4_i2c) = {
        "stm32f4,i2c-ctr",
        NULL
};

DEV_DRIVER_STRUCT(stm32f4_i2c) = {
        .compatible = stm32f4_i2c_compat,
        .probe = stm32f4_i2c_probe,
        .close = NULL
};

DEV_REGISTER_CALLBACK(stm32f4_i2c){
    dev_register_driver(&stm32f4_i2c_drv);
}

bool i2c_init(i2c_ctr_dev_t* i2c){

    if(!I2C_CHECK_VALID(i2c->dev.reg))
        return false;

    enum rcc_periph_clken clken;
    switch(i2c->dev.reg){
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

    i2c_reset(i2c->dev.reg);
    i2c_peripheral_disable(i2c->dev.reg);
    i2c_set_clock_frequency(i2c->dev.reg, i2c_freq_mhz);
    i2c_set_trise(i2c->dev.reg, i2c_freq_mhz + 1);
    i2c_set_ccr(i2c->dev.reg, ccr);
    I2C_CR1(i2c->dev.reg) = 0;
    I2C_OAR1(i2c->dev.reg) = 0x00000000;
    i2c_peripheral_enable(i2c->dev.reg);
    return true;
}



