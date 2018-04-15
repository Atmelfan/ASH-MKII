#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "math/linalg.h"
#include "fdt/dtb_parser.h"
#include "jetson.h"
#include "leg.h"
#include "platforms/board.h"
#include "platforms/i2c.h"
#include "platforms/log.h"
#include "platforms/pwm.h"
#include "ik/ik_3dof.h"
#include "gait/gait.h"
#include "libopencm3/cm3/systick.h"

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

leg_t* ik_appendages = NULL;

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
    fdt_token* legs = fdt_find_subnode(fdt, root, "legs");
    uint32_t num_legs = fdt_node_get_u32(fdt, legs, "#num-legs", 0);
    uint32_t servo_scale = fdt_node_get_u32(fdt, legs, "servo-scale", 0);
    logd_push("legs");
    if(!num_legs || (num_legs >= MAX_NUM_APPENDAGES)){
        logd_printfs(LOG_ERROR, "property '#num-legs' missing or out of range\n");
        //assert(false && "Property '#num-legs' missing or 0");
    }else{
        /* Allocate appendages*/
        ik_appendages = malloc(sizeof(leg_t)*num_legs);
        assert(ik_appendages && "Out of memory");

        /* Initialize appendages */
        for(int j = 0; j < num_legs; ++j){
            leg_init(&ik_appendages[j], servo_scale);
        }

        /*Read appendages from FDT*/
        for(fdt_token* l = fdt_token_next(fdt, legs); fdt_token_get_type(l) != FDT_END_NODE ; l = fdt_token_next(fdt, l)){
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
                if(!leg_from_node(&ik_appendages[reg], fdt, l)){
                    logd_pop();
                    l = fdt_node_end(fdt, l);
                    continue;
                }

                if(ik_appendages[reg].pwm_dev){
                    //leg_move_to_vec(&ik_appendages[reg], &ik_appendages[reg].home_position);
                    vec4 s = ik_appendages[reg].home_position;
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


    //TODO: (in order) servos, IK, gait, commands, light

    fdt_token* chosen = fdt_find_subnode(fdt, root, "chosen");
    fdt_token* powerup = fdt_find_phandle(fdt, fdt_node_get_u32(fdt, chosen, "gait-powerup", 0));
    fdt_token* powerdown = fdt_find_phandle(fdt, fdt_node_get_u32(fdt, chosen, "gait-powerdown", 0));
    gait_descriptor gait;
    gait_init(&gait);

    /* Execute startup gait if defined */
    if(powerup){
        gait_begin(&gait, fdt, powerup, ik_appendages);
    }else{
        gait_begin(&gait, fdt, NULL, ik_appendages);
    }

    while(true){
        /* Advance gait if not done */
        if(!gait.done){
            /* Update, go to next if required */
            if(gait_update(&gait, fdt, ik_appendages)){
                gait_next(&gait, fdt, ik_appendages);
            }
        }
        dev_systick_wait(100);


    }

    return 0;
}