//
// Created by atmelfan on 2018-03-24.
//

#include <stdlib.h>
#include "pca9685.h"
#include "../../dev.h"

static char* pca9685_compat[] = {
        "nxp,pca9685",
        NULL
};

static struct dev_driver pca9685_drv = {
        .compatible = pca9685_compat,
};

void reg_pca9685_(){
    dev_register_driver(&pca9685_drv);
}
