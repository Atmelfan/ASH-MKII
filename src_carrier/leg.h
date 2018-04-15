//
// Created by atmelfan on 2017-09-25.
//

#ifndef SRC_CARRIER_LEG_H
#define SRC_CARRIER_LEG_H

#include <stdbool.h>
#include "math/linalg.h"
#include "fdt/dtb_parser.h"
#include "ik/ik_3dof.h"
#include "platforms/pwm.h"

#define LEG_STATUS_ONFLOOR  0x01
#define LEG_STATUS_MOVING   0x02
#define LEG_STATUS_ENABLED  0x04

#define E_FACTOR 0.1f

typedef struct {
    mat4 transform;//Applies translation/rotation to convert an absolute pos to a leg relative pos.

    /*  */
    vec4 home_position;//Default position with maximum movement range
    vec4 current_position;//Current position
    vec4 offset_position;//Current position
    vec4 target_position;//Position to move towards

    /* Servo offsets from IK in degrees*100 */
    int32_t servo_offsets_100[3];
    uint32_t servo_index[3];
    int32_t lengths[3];
    uint8_t invert[3];
    uint32_t scale;

    pwm_dev_t* pwm_dev;

    /*  */
    uint8_t status;

    /*  */
    bool ready;
} leg_t;

/**
 * Initializes a leg with basic parameters
 * @param l, leg to initialize
 * @param scale, pulse width required for a 90 deg rotation in us
 */
void leg_init(leg_t *l, uint32_t scale);

/**
 * Reads leg parameters from fdt node
 * @param l, leg struct
 * @param fdt, fdt header
 * @param node, fdt node to read from
 * @return true if leg fdt is valid, false otherwise
 */
bool leg_from_node(leg_t* l, fdt_header_t* fdt, fdt_token* node);

/**
 * Move leg to local coordinates without interpolation
 * @param l, leg to move
 * @param loc, local coordinates in mm
 */
void leg_move_to_local(leg_t* l, vec4* loc);

/**
 * Move leg to global coordinates without interpolation
 * @param l, leg to move
 * @param vec, global coordinates in mm
 */
void leg_move_to_vec(leg_t* l, vec4* vec);

/**
 * Sets interpolation target
 * @param l, leg to move
 * @param target, global coordinates in mm
 * @param rate, velocity in mm/s
 */
void leg_target(leg_t* l, vec4* target, float rate);

/**
 * Interpolates
 * @param l
 * @param target
 * @param dt
 * @return true if the leg is at target
 */
bool leg_interpolate(leg_t* l, float t);

#endif //SRC_CARRIER_LEG_H
