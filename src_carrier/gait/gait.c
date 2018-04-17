//
// Created by atmelfan on 2018-03-20.
//

#include "gait.h"
#include "../leg.h"
#include "../fdt/dtb_parser.h"
#include "../platforms/log.h"
#include "../ik/ik_3dof.h"
#include "../fdt_mathutil.h"

bool gait_update_step(gait_step *step, fdt_header_t *fdt, float t) {
    for(fdt_token* l = fdt_token_next(fdt, step->node); fdt_token_get_type(l) != FDT_END_NODE ; l = fdt_token_next(fdt, l)){
        if(fdt_token_get_type(l) == FDT_BEGIN_NODE){



            l = fdt_node_end(fdt, l);
        }
    }
    return 0;
}

void gait_init_target(gait_target *t) {
    t->initial = VEC4_ZERO();
    t->target = VEC4_ZERO();
}

void gait_step_begin(gait_step *step, gait_target *ts, fdt_header_t *fdt, fdt_token *node, uint32_t num) {
    for (int i = 0; i < num; ++i) {
        ts[i].initial = ts[i].target;
    }

    for(fdt_token* l = fdt_token_next(fdt, step->node); fdt_token_get_type(l) != FDT_END_NODE ; l = fdt_token_next(fdt, l)){
        if(fdt_token_get_type(l) == FDT_BEGIN_NODE){
            uint32_t i = fdt_node_get_u32(fdt, node, "reg", UINT32_MAX);
            if(i < num){

            }

            l = fdt_node_end(fdt, l);
        }
    }

}
