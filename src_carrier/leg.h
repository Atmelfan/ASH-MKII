//
// Created by atmelfan on 2017-09-25.
//

#ifndef SRC_CARRIER_LEG_H
#define SRC_CARRIER_LEG_H

#include <stdbool.h>
#include "math/linalg.h"
#include "fdt/dtb_parser.h"

#define LEG_STATUS_ONFLOOR  0x01
#define LEG_STATUS_MOVING   0x02
#define LEG_STATUS_ENABLED  0x04

#define E_FACTOR 0.1f

typedef struct {
    mat4 transform;//Applies translation/rotation to convert an absolute pos to a leg relative pos.

    vec4 home_position;//Default position with maximum movement range
    vec4 current_position;//Current position
    vec4 target_position;//Position to move towards

    /* Servo offsets from IK in degrees*100 */
    uint32_t servo_offsets_100[3];

    uint8_t status;
    bool ready;
} leg_t;

void leg_init(leg_t* l);

bool leg_from_node(leg_t* l, fdt_header_t* fdt, fdt_token* node);


#endif //SRC_CARRIER_LEG_H
