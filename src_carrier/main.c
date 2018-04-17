#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <libopencm3/stm32/usart.h>
#include "math/linalg.h"
#include "fdt/dtb_parser.h"
#include "jetson.h"
#include "leg.h"
#include "platforms/board.h"
#include "platforms/i2c.h"
#include "platforms/log.h"
#include "body.h"
#include "ik/ik_3dof.h"
#include "math/linalg_util.h"
#include "gait/gait.h"

#define MAX_NUM_APPENDAGES 64

#define RECV_BUF_SIZE 128
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

uint8_t recv_buf[RECV_BUF_SIZE];
volatile int recv_ndx_nxt = 0, ready = 0;

void usart2_isr(void)
{
    uint32_t	reg;
    int			i;

    do {
        reg = USART_SR(USART2);
        if (reg & USART_SR_RXNE) {
            recv_buf[recv_ndx_nxt] = (uint8_t) USART_DR(USART2);
            if(recv_buf[recv_ndx_nxt] == '\n')
                ready = 1;

            i = (recv_ndx_nxt + 1) % RECV_BUF_SIZE;
            recv_ndx_nxt = i;

        }
    } while ((reg & USART_SR_RXNE) != 0); /* can read back-to-back
						 interrupts */
}

void usart2_setup(){
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);

    /* Actual Alternate function number (in this case 7) is part
     * depenedent, CHECK THE DATA SHEET for the right number to
     * use.
     */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);


    /* This then enables the clock to the USART1 peripheral which is
     * attached inside the chip to the APB1 bus. Different peripherals
     * attach to different buses, and even some UARTS are attached to
     * APB1 and some to APB2, again the data sheet is useful here.
     */
    rcc_periph_clock_enable(RCC_USART2);

    /* Set up USART/UART parameters using the libopencm3 helper functions */
    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_enable(USART2);

    /* Enable interrupts from the USART */
    nvic_enable_irq(NVIC_USART2_IRQ);

    /* Specifically enable recieve interrupts */
    usart_enable_rx_interrupt(USART2);
}

leg_t* legs = NULL;
gait_target* targets = NULL;

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
    //usart2_setup();

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
        targets = malloc(sizeof(gait_target)*num_legs);
        assert(legs && targets && "Out of memory");

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

    while(!targets || !legs);

    //TODO: (in order) gait, commands+remote, light

    /* Get application parameters node */
    fdt_token* chosen = fdt_find_subnode(fdt, root, "chosen");
    fdt_token* tstep = fdt_find_subnode(fdt, root, "step@test");

    /* Timekeeping */
    uint32_t last = 0, dt = 0, now = 0, t = 0;

    /* Body struct keeps track of body rotation and translation */
    body_t body;
    body_init(&body);


    mat4 gait_mat = MAT4_IDENT();
    vec4 a = VEC4_ZERO(), mov = VEC4(0, 40, 0, 0);


    uint8_t  index = 0;
    bool x [8] = {true, false, false, true, true, false, false, true};
    for (int k = 0; k < 8; ++k) {
        targets[k].initial = VEC4_ZERO();
        targets[k].target = mov;
        vec_scalel((vecx *)&targets[k].target, x[k] ? 1 : -1);
    }


    while(true){

        /* Calculate delta-t */
        now = dev_systick_get();
        dt = now - last;

        if((t + dt < 5000)){
            t = t + dt;
        }else{
            t = 5000;
        }
        body.offset.members[2] = 50;
        body.model = MAT4_IDENT();

        /* Body rotation */
        //mat4 temp = MAT4_IDENT();
        //mat4_rotz((float) (0.25f * (t / 5000.0f) * M_PI_4), &temp);//YAW
        //mat_matmull((matxx *) &body.model, (matxx *) &temp);
        //temp = MAT4_IDENT();
        //mat4_rotz((float) (0.25f * (t / 5000.0f) * M_PI_4), &temp);//PITCH
        //mat_matmull((matxx *) &body.model, (matxx *) &temp);
        //temp = MAT4_IDENT();
        //mat4_roty((float) (0.125f * (t / 5000.0f) * M_PI_4), &temp);//ROLL
        //mat_matmull((matxx *) &body.model, (matxx *) &temp);

        /* TEST */




        /* Update legs */
        for(int i = 0; i < num_legs; ++i){
            vec4 l = legs[i].home_position;
            vec4 tmp = VEC4(0, 0, 0, 1), target;

            /* Subtract body offset */
            vec_sub((vecx *) &l, (vecx *) &body.offset, (vecx*) &tmp);
            target = tmp;

            /* TODO:  and translate with gait */
            //mat4 gait_model = MAT4_IDENT();
            //vecmat_mul((matxx*)&gait_model, (vecx*)&target, (vecx*)&tmp);
            //target = tmp;
            vec4 c = VEC4_ZERO();
            vec_sub((vecx *) &targets[i].target, (vecx *) &targets[i].initial, (vecx *) &c);
            vec_scalel((vecx *)&c, t / 5000.0f);
            vec_addl((vecx *) &c, (vecx *) &targets[i].initial);

            gait_mat.MAT4_M(3,0) = c.members[0];
            gait_mat.MAT4_M(3,1) = c.members[1];
            gait_mat.MAT4_M(3,2) = c.members[2] + 100.0f*sinf(3.14f*t/5000.0f) * (x[i] ? 1 : 0);
            vecmat_mul((matxx*)&gait_mat, (vecx*)&target, (vecx*)&tmp);
            target = tmp;

            /* Rotate & translate all leg vectors with model matrix*/
            vecmat_mul((matxx*)&body.model, (vecx*)&target, (vecx*)&tmp);
            target = tmp;

            /* Move to target */
            leg_move_to_vec(&legs[i], &target);
        }

        /* Save last time */
        last = now;
        //dev_systick_wait(10);
    }

    return 0;
}