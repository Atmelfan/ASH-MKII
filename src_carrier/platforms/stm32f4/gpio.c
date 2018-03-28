//
// Created by atmelfan on 3/28/18.
//

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdio.h>
#include "../dev.h"

struct {
    uint32_t port;
    enum rcc_periph_clken rcc;
} gpio_ports[] = {
        {GPIOA, RCC_GPIOA},
        {GPIOB, RCC_GPIOB},
        {GPIOC, RCC_GPIOC},
        {GPIOD, RCC_GPIOD},
        {GPIOE, RCC_GPIOE},
        {GPIOF, RCC_GPIOF}
};


DEV_PROBE_CALLBACK(stm32f4_gpio, fdt, node, parent){



    return PROBE_FAIL;
}

DEV_COMPAT_LIST(stm32f4_gpio) = {
        "stm32f4,port",
        NULL
};

DEV_DRIVER_STRUCT(stm32f4_gpio) = {
        .compatible = stm32f4_gpio_compat,
        .probe = stm32f4_gpio_probe,
        .close = NULL
};

DEV_REGISTER_CALLBACK(stm32f4_gpio){
    dev_register_driver(&stm32f4_gpio_drv);
}
