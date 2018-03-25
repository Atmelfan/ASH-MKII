#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include <assert.h>
#include <libopencm3/stm32/i2c.h>
#include "math/linalg.h"
#include "fdt/dtb_parser.h"
#include "jetson.h"
#include "leg.h"
#include "platforms/board.h"
#include "platforms/i2c.h"

#define MAX_NUM_APPENDAGES 64

extern void initialise_monitor_handles(void);
extern fdt_header_t octapod;

bool mat4_from_node(fdt_token* t, mat4* m){
    m->mat.w = 4;
    m->mat.h = 4;
    if(fdt_token_get_type(t) == FDT_PROP && fdt_read_u32(&t->len) >= 16*4){
        for(int i = 0; i < 16; ++i){
            m->members[i] = ((int32_t)fdt_read_u32(&t->cells[i]))/1000.0f;
        }
        return true;
    }
    return false;
}

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

static void timer_setup(){

    /** TIMER 2 **/
    nvic_enable_irq(NVIC_TIM2_IRQ);//Enable interrupt
    rcc_periph_reset_pulse(RST_TIM2);//Reset timer

}

void mk_indent(int lvl){
    for(int i = 0; i < lvl; ++i){
        printf("\t");
    }
}

leg_t* ik_appendages = NULL;

int main(void)
{

    initialise_monitor_handles();
    clock_setup();
    gpio_setup();
    jetson_batocp(false);
    board_init_fdt(NULL);
    int i;
    printf("Hello world!\n");

    fdt_header_t* fdt = &octapod;

    fdt_token* root = fdt_get_tokens(fdt);
    fdt_token* bootmsg = fdt_node_get_prop(fdt, root, "bootmsg", false);
    printf("%s\n", bootmsg->prop_str);

    char buffer[64];
    fdt_token* legs = fdt_find_subnode(fdt, root, "legs");
    uint32_t num_legs = fdt_node_get_u32(fdt, legs, "#num-legs", 0);
    if(!num_legs || (num_legs >= MAX_NUM_APPENDAGES)){
        printf("[ERROR] Property '#num-legs' missing or out of range at %s\n", fdt_trace(fdt, legs, buffer));
        //assert(false && "Property '#num-legs' missing or 0");
    }else{
        /* Allocate appendages*/
        ik_appendages = malloc(sizeof(leg_t)*num_legs);
        if(!ik_appendages)
            assert(false && "Out of memory");
        for(int j = 0; j < num_legs; ++j){
            ik_appendages[j].initialized = false;
        }

        /*Read appendages from FDT*/
        for(fdt_token* l = fdt_token_next(fdt, legs); fdt_token_get_type(l) != FDT_END_NODE ; l = fdt_token_next(fdt, l)){
            /*Check for nodes*/
            if(fdt_token_get_type(l) == FDT_BEGIN_NODE){
                //printf("-> %s:\n", fdt_trace(fdt, l, buffer));

                /*Get index of appendage*/
                fdt_token* reg = fdt_node_get_prop(fdt, l, "reg", false);
                if(reg && fdt_prop_len(fdt, reg)){
                    uint32_t r = fdt_read_u32(&reg->cells[0]);

                    if(r < num_legs){
                        bool success = leg_from_node(&ik_appendages[r], fdt, l);
                        if(!success){
                            printf("[ERROR] Failed to init leg from %s, missing 'home' or 'transform'\n", fdt_trace(fdt, l, buffer));
                        }
                    }else{
                        printf("[ERROR] Property 'reg' must be less than #num-legs at %s\n", fdt_trace(fdt, l, buffer));
                    }
                }else{
                    printf("[ERROR] Property 'reg' missing or empty in %s\n", fdt_trace(fdt, l, buffer));
                }


                /*Handle IK*/
                fdt_token* ik = fdt_find_subnode(fdt, l, "inverse-kinematics");
                if(ik){

                }else{
                    printf("[ERROR] Node 'inverse-kinematics' missing in %s\n", fdt_trace(fdt, l, buffer));
                }

                /*Exit node*/
                l = fdt_node_end(fdt, l);
            }
        }
    }



    //TODO: (in order) servos, IK, gait, commands, light

    /*Init I2C2*/
    rcc_periph_clock_enable(RCC_I2C2);

    #define I2C_BAUD 100000
    uint32_t i2c_freq_mhz = rcc_apb1_frequency / 1000000;
    uint32_t ccr = rcc_apb1_frequency / I2C_BAUD / 2 + 1;
    if (ccr < 4)
        ccr = 4;

    i2c_peripheral_disable(I2C2);
    i2c_set_clock_frequency(I2C2, i2c_freq_mhz);
    i2c_set_trise(I2C2, i2c_freq_mhz + 1);
    i2c_set_ccr(I2C2, ccr);
    I2C_CR1(I2C2) = 0;
    I2C_OAR1(I2C2) = 32;
    i2c_peripheral_enable(I2C2);
    uint8_t addr = 0b10001110;

    void i2c_tx(uint8_t addr, uint8_t* buf, uint8_t n){
        // Send start condition.
        I2C_CR1(I2C2) |= I2C_CR1_START;
        while(!(I2C_SR1(I2C2) & I2C_SR1_SB)){

        }


        // Send slave address.
        I2C_DR(I2C2) = addr & (uint8_t)~0x01;
        while(!(I2C_SR1(I2C2) & I2C_SR1_ADDR)){
            if(I2C_SR1(I2C2) & I2C_SR1_AF){
                I2C_CR1(I2C2) |= I2C_CR1_STOP;
                I2C_SR1(I2C2) = ~I2C_SR1_AF;
                break;
            }
        }

        uint16_t unused;
        unused = I2C_SR1(I2C2);
        unused = I2C_SR2(I2C2);
        unused = unused;

        for (size_t t = 0; t < n; t++) {
            I2C_DR(I2C2) = buf[t];
            while (!(I2C_SR1(I2C2) & I2C_SR1_BTF)) {
            }
        }

        I2C_CR1(I2C2) |= I2C_CR1_STOP;
        fdt_node_get_u32(fdt, legs, "#num-legs", 0);
    }


    /*Test I2C2*/
    uint8_t reset[] = {0x00, 0x20};//Reset
    i2c_tx(addr, reset, 2);
    uint8_t sleep[] = {0x00, 0x30};
    i2c_tx(addr, sleep, 2);
    uint8_t prescale[] = {0xfe, 122};
    i2c_tx(addr, prescale, 2);

    i2c_tx(addr, reset, 2);
    uint8_t restart[] = {0x00, 0xA0};
    uint32_t numl = fdt_node_get_u32(fdt, legs, "#num-legs", 0);
    i2c_tx(addr, restart, 2);

    for(int j = 0; j < 16; ++j){
        uint8_t pwm[] = {0x06 + 4*j, 0x00, 0x00, ((319 >> 0) & 0xff), ((319 >> 8) & 0xff)};//Reset
        i2c_tx(addr, pwm, 5);
    }

    while(true){


    }

    return 0;
}