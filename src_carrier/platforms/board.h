//
// Created by atmelfan on 2018-03-24.
//

#ifndef SRC_CARRIER_BOARD_H
#define SRC_CARRIER_BOARD_H

#include "../fdt/dtb_parser.h"

void board_init(void);

void board_init_fdt(const fdt_token* root);

#endif //SRC_CARRIER_BOARD_H
