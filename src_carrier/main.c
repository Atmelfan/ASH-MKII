#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include <assert.h>
#include <libopencm3/stm32/i2c.h>
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

#define MAX_NUM_APPENDAGES 64

#ifdef DEBUG
extern void initialise_monitor_handles(void);
#endif

extern fdt_header_t octapod;


/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
    rcc_clock_setup_hse_3v3(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    //Enable peripheral clocks
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    rcc_periph_clock_enable(RCC_TIM2);
}

static void gpio_setup(void)
{
    //Configure gpio
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);//PA8

    //I2C2
    //PB10, AF4
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO10);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO10);

    //PB11, AF4
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO11);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO11);
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
    gpio_setup();
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
            leg_init(&ik_appendages[j]);
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

                /* Read ik parameters*/
                fdt_token* ik = fdt_find_subnode(fdt, l, "inverse-kinematics");
                if(ik){
                    fdt_token* servos = fdt_node_get_prop(fdt, ik, "servos", false);
                    fdt_token* test = fdt_node_get_prop(fdt, ik, "test", false);
                    fdt_token* length = fdt_node_get_prop(fdt, ik, "length", false);

                    uint32_t servo_phandle = fdt_read_u32(&servos->cells[0]);
                    pwm_dev_t* pca = (pwm_dev_t *) dev_find_device_phandle(servo_phandle);

                    if(pca && length && fdt_prop_len(fdt, length) == 3*sizeof(uint32_t)){

                        if(test){
                            float s[3];
                            for (int i = 0; i < 3; ++i) {
                                uint32_t index = fdt_read_u32(&servos->cells[1 + 2*i + 0]);
                                s[i] = (int32_t)fdt_read_u32(&test->cells[i]) - (int32_t)fdt_read_u32(&servos->cells[1 + 2*i + 1]);
                                set_servo(pca, index, (int32_t) s[i], servo_scale);
                            }
                            logd_printf(LOG_DEBUG, "positioned to %f, %f, %f\n", s[0]/10, s[1]/10, s[2]/10);
                        }else{
                            float x = ik_appendages[reg].home_position.members[0],
                                    y = ik_appendages[reg].home_position.members[1],
                                    z = ik_appendages[reg].home_position.members[2];

                            const float L1 = fdt_read_u32(&length->cells[0])/10.0f;
                            const float L2 = fdt_read_u32(&length->cells[1])/10.0f;
                            const float L3 = fdt_read_u32(&length->cells[2])/10.0f;
                            float d_sq = x*x + y*y;
                            float d = sqrtf(d_sq) - L1;
                            float v = sqrtf(d*d + z*z);
                            float s1 = (v*v + L2*L2 - L3*L3)/(2*v*L2);
                            float s2 = (L2*L2 + L3*L3 - v*v)/(2*L2*L3);

                            float S0 = (1800.0f/(float)M_PI)*(atan2f(y, x));
                            float S1 = (1800.0f/(float)M_PI)*(acosf(s1) + atan2f(z, d));
                            float S2 = (1800.0f/(float)M_PI)*(acosf(s2));

                            uint32_t index = fdt_read_u32(&servos->cells[1 + 2*0 + 0]);
                            int32_t degre = (int32_t)fdt_read_u32(&servos->cells[1 + 2*0 + 1]);
                            set_servo(pca, index, (int32_t)S0 - degre, servo_scale);

                            index = fdt_read_u32(&servos->cells[1 + 2*1 + 0]);
                            degre = (int32_t)fdt_read_u32(&servos->cells[1 + 2*1 + 1]);
                            set_servo(pca, index, ((int32_t)-S1 - degre), servo_scale);

                            index = fdt_read_u32(&servos->cells[1 + 2*2 + 0]);
                            degre = (int32_t)fdt_read_u32(&servos->cells[1 + 2*2 + 1]);
                            set_servo(pca, index, ((int32_t)S2 - degre), servo_scale);
                            logd_printf(LOG_DEBUG, "homed to %f, %f, %f\n", S0/10, S1/10, S2/10);
                        }





                    }else{
                        logd_printfs(LOG_INFO, "node 'inverse-kinematics' malformed\n");
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

    while(true){

    }

    return 0;
}