//
// Created by atmelfan on 2018-03-20.
//

#include "gait.h"
#include "../leg.h"
#include "../fdt/dtb_parser.h"
#include "../platforms/log.h"
#include "../ik/ik_3dof.h"
#include "../fdt_mathutil.h"

void gait_init(gait_descriptor *gait) {
    gait->current_tra = VEC4_ZERO();
    gait->translate = false;
    gait->current_rot = VEC4_ZERO();
    gait->rotate = false;
    gait->home = 0;
}

void gait_eval_next(gait_descriptor *gait, fdt_header_t *fdt, leg_t *legs) {/* Home-all command */
    logd_printf(LOG_DEBUG, "current: %s", gait->current->name);
    fdt_token* home = fdt_node_get_prop(fdt, gait->current, "home-all", false);
    if(home){
        gait->home = fdt_prop_len(fdt, home) ? fdt_read_u32(&home->cells[0]) : 0;
        gait_home_all(legs, gait->num_legs);
        logd_printf(LOG_DEBUG, "home: %d", gait->home);
    }

    /* Translate command */
    fdt_token* translate = fdt_node_get_prop(fdt, gait->current, "translate", false);
    if(translate && fdt_prop_len(fdt, translate) == 4*sizeof(uint32_t)){
        vec4 t = VEC4_ZERO();
        for (int i = 0; i < 3; ++i) {
            t.members[i] = fdt_read_u32(&translate->cells[i])/1000.0f;
        }
        t.members[3] = 1;
        gait_translate(gait, &t, fdt_read_u32(&translate->cells[3])/1000.0f);
    }
}

bool gait_begin(gait_descriptor *gait, fdt_header_t *fdt, fdt_token *node, leg_t *legs) {
    /* Save number of legs required for compatibility check */

    gait->done = false;
    gait->end = false;

    /*  */
    if(node){
        gait->name = node->name;
        gait->num_legs = fdt_node_get_u32(fdt, node, "#num-legs", 0);
        gait->node = node;
        gait->current = fdt_find_subnode(fdt, node, "start");
        gait_eval_next(gait, fdt, legs);
    }else{
        gait->name = "MissingNo";
        gait->num_legs = 0;
        gait->node = NULL;
        gait->current = NULL;
    }
    logd_printfs(LOG_DEBUG, "begin");

    gait->last_time = dev_systick_get();

    /* TODO: This might be discriminating against legless robots */
    return (gait->num_legs != 0) && gait->current;
}

bool gait_home_all(leg_t *l, uint32_t num) {
    /* Move all legs to their home vector*/
    for (int i = 0; i < num; ++i) {
        leg_move_to_vec(&l[i], &l[i].home_position);
    }
    logd_printfs(LOG_DEBUG, "home-all");
    return 0;
}

bool gait_update(gait_descriptor *gait, fdt_header_t *fdt, leg_t *legs) {
    if(!gait->current)
        return false;
    uint32_t t = dev_systick_get();
    uint32_t dt = t - gait->last_time;
    gait->last_time = t;

    /* home-all timeout */
    if(gait->home){
        if(gait->home >= dt){
            gait->home -= dt;
        }else{
            gait->home = 0;
            return true;
        }
        return false;
    }

    /* Translate */
    if(gait->translate){
        gait->time_tra += dt;
        if(gait->time_tra/1000.0f > gait->period_tra){
            gait->current_tra = gait->target_tra;
            gait->translate = false;
            return true;
        }else{
            vec_scale((vecx *) &gait->delta_tra, gait->time_tra / 1000.0f / gait->period_tra,
                      (vecx *) &gait->current_tra);
        }
        return false;

    }

    return true;
}



bool gait_next(gait_descriptor *gait, fdt_header_t *fdt, leg_t *legs) {
    if(!gait->current)
        return false;

    fdt_token* end = fdt_node_get_prop(fdt, gait->current, "end", 0);
    if(gait->end && end){
        /* Go to end node if end has been requested */
        gait->current = fdt_find_phandle(fdt, fdt_read_u32(&end->cells[0]));
    }else{
        /* Go to next node */
        gait->current = fdt_find_phandle(fdt, fdt_node_get_u32(fdt, gait->current, "next", 0));
    }

    /* No next node, done*/
    if(!gait->current){
        gait->done = true;
        logd_printfs(LOG_DEBUG, "the end");
        return false;
    }
    gait_eval_next(gait, fdt, legs);
    return true;
}

void gait_end(gait_descriptor *gait) {
    gait->end = true;
}

void gait_translate(gait_descriptor *gait, vec4 *target, float rate) {
    /* Copy target */
    vec_mov((vecx *) &gait->target_tra, (vecx *) target);

    gait->delta_tra = VEC4_ZERO();
    vec_sub((vecx *) target, (vecx *) &gait->current, (vecx *) &gait->delta_tra);

    gait->period_tra = vec_length((vecx *) &gait->delta_tra) / rate;
    gait->translate = true;

    logd_printf(LOG_DEBUG, "%f,%f,%f -> %f,%f,%f / %f",
                gait->current_tra.members[0], gait->current_tra.members[1], gait->current_tra.members[2],
                target->members[0], target->members[1], target->members[2],
                rate);
}
