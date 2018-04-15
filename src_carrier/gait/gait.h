//
// Created by atmelfan on 2018-03-20.
//

#ifndef SRC_CARRIER_GAIT_H
#define SRC_CARRIER_GAIT_H

#include "../fdt/dtb_parser.h"
#include "../leg.h"

typedef struct{
    char* name;
    fdt_token* node;
    fdt_token* current;

    /* Used for home-all, value corresponds to time before next node*/
    uint32_t home;

    /* Rotation */
    vec4 current_rot, target_rot, delta_rot;
    float period_rot;
    uint32_t time_rot;
    bool rotate;


    /* Translation */
    vec4 current_tra, target_tra, delta_tra;
    float period_tra;
    uint32_t time_tra;
    bool translate;

    /* True if gait has ended */
    bool done;

    /* True if gait has been asked to end */
    bool end;

    /* Number of legs used by gait */
    uint32_t num_legs;

    uint32_t last_time;


} gait_descriptor;

void gait_translate(gait_descriptor* gait, vec4* target, float rate);

void gait_init(gait_descriptor *gait);

/**
 * Load a gait and automatically go to start step.
 * @param gait, gait descriptor
 * @param fdt, fdt header
 * @param node, fdt node containing
 * @return
 */
bool gait_begin(gait_descriptor *gait, fdt_header_t *fdt, fdt_token *node, leg_t *legs);

bool gait_update(gait_descriptor *gait, fdt_header_t *fdt, leg_t *legs);

bool gait_next(gait_descriptor *gait, fdt_header_t *fdt, leg_t *legs);

void gait_end(gait_descriptor *gait);

bool gait_home_all(leg_t* l, uint32_t num);


#endif //SRC_CARRIER_GAIT_H
