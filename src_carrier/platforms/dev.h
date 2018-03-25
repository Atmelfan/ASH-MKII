//
// Created by atmelfan on 2018-03-21.
//

#ifndef SRC_CARRIER_DEV_H
#define SRC_CARRIER_DEV_H


#include <stdint.h>
#include "../fdt/dtb_parser.h"

typedef enum {
    DEV_TYPE_NULL = 0,
    DEV_TYPE_SERIAL,
    DEV_TYPE_I2C,
    DEV_TYPE_I2C_DEV,
    DEV_TYPE_SPI
} dev_type;

typedef enum {
    PROBE_SUCCESS = 0,
    PROBE_FAIL
} dev_probe_t;

typedef struct device_t device_t;

struct device_t {
    char* name;
    dev_type type;
    uint32_t phandle;
    uint32_t reg;
    device_t* next;
};

typedef struct dev_driver dev_driver;
struct dev_driver {
    char** compatible;
    /**
     * Probes a device and if necessary
     * @param fdt
     * @param node
     * @param dev
     * @return
     */
    dev_probe_t (* probe)(fdt_header_t* fdt, fdt_token* node);
    dev_probe_t (*close)(dev_driver* dev);
    struct dev_driver* next;
};

void dev_register_driver(dev_driver* d);
void dev_register_device(device_t* d);
void dev_init_drivers(void);
dev_driver* dev_find_driver(char* compatible);

#define DEV_T_INIT() (dev_t){.name = NULL, type = DEV_TYPE_NULL, .phandle = 0}

#endif //SRC_CARRIER_DEV_H
