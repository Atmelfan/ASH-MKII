#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <stdio.h>
#include <assert.h>
#include "math/linalg.h"
#include "fdt/dtb_parser.h"
#include "jetson.h"
#include "leg.h"
#include "platforms/board.h"
#include "platforms/log.h"
#include "body.h"

#define MAX_NUM_APPENDAGES 64

#ifdef DEBUG
extern void initialise_monitor_handles(void);
#endif

extern fdt_header_t octapod;


/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
    rcc_clock_setup_hse_3v3(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
}

void sys_tick_handler(){
    dev_systick();
}

leg_t* legs = NULL;

/**
 * Set a servo to the requested angle
 * @param pca, pwm device
 * @param index, servo channel
 * @param degres_10, degrees scaled by 10
 * @param scale, number of us to turn 90 degrees
 */
void set_servo(pwm_dev_t *pca, uint32_t index, int32_t degres_10, uint32_t scale) {
    uint32_t period = ((pwm_driver_t*)pca->dev.drv)->period;

    uint32_t us = (uint32_t) (1500 + (int32_t)(degres_10*scale)/900);

    if(us < 250)
        us = 250;
    else if(us > 2750)
        us = 2750;

    uint32_t counts = (us*period)/20000;

    set_pwm(pca, (uint16_t) index, 0, counts);
}

int main(void)
{

#ifdef DEBUG
    initialise_monitor_handles();
#endif
    clock_setup();
    jetson_batocp(false);

    printf("Hello world!\n");

    fdt_header_t* fdt = &octapod;

    fdt_token* root = fdt_get_tokens(fdt);
    fdt_token* bootmsg = fdt_node_get_prop(fdt, root, "bootmsg", false);
    printf("%s\n", bootmsg->prop_str);

    /*Override board init file with "/platform" node*/
    board_init_fdt(fdt, fdt_find_subnode(fdt, root, "platform"));

    char buffer[64];
    fdt_token* legs_node = fdt_find_subnode(fdt, root, "legs");
    uint32_t num_legs = fdt_node_get_u32(fdt, legs_node, "#num-legs", 0);
    uint32_t servo_scale = fdt_node_get_u32(fdt, legs_node, "servo-scale", 0);
    logd_push("legs");
    if(!num_legs || (num_legs >= MAX_NUM_APPENDAGES)){
        logd_printfs(LOG_ERROR, "property '#num-legs' missing or out of range\n");
        //assert(false && "Property '#num-legs' missing or 0");
    }else{
        /* Allocate appendages*/
        legs = malloc(sizeof(leg_t)*num_legs);
        assert(legs && "Out of memory");

        /* Initialize appendages */
        for(int j = 0; j < num_legs; ++j){
            leg_init(&legs[j], servo_scale);
        }

        /*Read appendages from FDT*/
        for(fdt_token* l = fdt_token_next(fdt, legs_node); fdt_token_get_type(l) != FDT_END_NODE ; l = fdt_token_next(fdt, l)){
            /*Check for nodes*/
            if(fdt_token_get_type(l) == FDT_BEGIN_NODE){
                //printf("-> %s:\n", fdt_trace(fdt, l, buffer));
                logd_push(l->name);

                uint32_t reg = fdt_node_get_u32(fdt, l, "reg", num_legs);

                /* Check leg index within range*/
                if(reg >= num_legs){
                    logd_printf(LOG_ERROR, "reg out of range", reg);
                    logd_pop();
                    l = fdt_node_end(fdt, l);
                    continue;
                }

                /* Read leg parameters*/
                if(!leg_from_node(&legs[reg], fdt, l)){
                    logd_pop();
                    l = fdt_node_end(fdt, l);
                    continue;
                }

                if(legs[reg].pwm_dev){
                    //leg_move_to_vec(&ik_appendages[reg], &ik_appendages[reg].home_position);
                    vec4 s = legs[reg].home_position;
                    logd_printf(LOG_DEBUG, "home at %f, %f, %f\n", s.members[0], s.members[1], s.members[2]);
                }else{
                    logd_printfs(LOG_WARNING, "no servo driver\n");
                }

                /* Read ik parameters */
                //TODO: Remove this shit
                fdt_token* ik = fdt_find_subnode(fdt, l, "inverse-kinematics");
                if(ik){
                    fdt_token* test = fdt_node_get_prop(fdt, ik, "test", false);

                    if(test){
                        fdt_token* invert = fdt_node_get_prop(fdt, ik, "invert", false);
                        fdt_token* servos = fdt_node_get_prop(fdt, ik, "servos", false);

                        /* Find servo driver */
                        uint32_t servo_phandle = fdt_read_u32(&servos->cells[0]);
                        pwm_dev_t* pca = (pwm_dev_t *) dev_find_device_phandle(servo_phandle);

                        if(pca){
                            int32_t s[3];
                            for (int i = 0; i < 3; ++i) {
                                uint32_t index = fdt_read_u32(&servos->cells[1 + 2*i + 0]);
                                s[i] = ((int32_t)fdt_read_u32(&test->cells[i]) - (int32_t)fdt_read_u32(&servos->cells[1 + 2*i + 1])) * (invert ? -1 : 1);
                                set_servo(pca, index, (int32_t) s[i], servo_scale);
                            }
                            logd_printf(LOG_DEBUG, "positioned to %d, %d, %d\n", s[0]/10, s[1]/10, s[2]/10);
                        }

                    }

                }else{
                    logd_printfs(LOG_INFO, "node 'inverse-kinematics' missing\n");
                }

                /*Exit node*/
                logd_pop();
                l = fdt_node_end(fdt, l);
            }
        }
    }
    logd_pop();
    logd_printfs(LOG_INFO, "ready!\n");


    //TODO: (in order) commands+remote, gait, light

    /* Default to home positions */
    for(int k = 0; k < num_legs; ++k){
        legs[k].current_position = legs[k].home_position;
    }

    /* Get application parameters node */
    fdt_token* chosen = fdt_find_subnode(fdt, root, "chosen");
    fdt_token* powerup = fdt_find_phandle(fdt, fdt_node_get_u32(fdt, chosen, "gait-powerup", 0));

    /* Merged model matrix */
    mat4 model;

    /* Body struct keeps track of body rotation and translation */
    body_t body;
    body_reset(&body);

    while(true){
        model = MAT4_IDENT();

        /* Update body */
        //body_rotate(&body, NULL);
        //body_translate(&body, NULL);
        mat_matmull((matxx*)&model, (matxx*)&body.model);

        /* Update gait */
        //gait_update(gait, );

        /* Update legs */
        for(int i = 0; i < num_legs; ++i){
            vec4 l = legs[i].current_position;
            vec4 target = VEC4_ZERO();

            /* Rotate & translate all leg vectors with model matrix*/
            vecmat_mul((matxx*)&model, (vecx*)&l, (vecx*)&target);

            /* Move to target */
            leg_move_to_vec(&legs[i], &target);
        }

        dev_systick_wait(100);
    }

    return 0;
}