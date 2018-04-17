//
// Created by atmelfan on 2018-03-20.
//

#ifndef SRC_CARRIER_GAIT_H
#define SRC_CARRIER_GAIT_H

#include "../fdt/dtb_parser.h"
#include "../leg.h"


typedef struct {
    /* Name of node/step */
    char* name;

    /* Associated fdt node */
    fdt_token* node;

    /*  */
    uint32_t* targets;

    /* Interpolation value, varies between 0 to 1 */
    float t;

} gait_step;

typedef struct {
    vec4 initial, target;
} gait_target;

void gait_init_target(gait_target* t);

/**
 * Read all targets for step and store in an array of gait_target for interpolation
 * @param ts, target array
 * @param fdt, fdt header
 * @param node, fdt gait step node
 * @param num, max number of targets
 */

void gait_step_begin(gait_step *step, gait_target *ts, fdt_header_t *fdt, fdt_token *node, uint32_t num);

bool gait_calc_mat(gait_step *step, uint32_t i, float t);

bool gait_update_step(gait_step *step, fdt_header_t *fdt, float t);

#endif //SRC_CARRIER_GAIT_H
