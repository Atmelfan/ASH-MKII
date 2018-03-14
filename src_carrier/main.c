#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include "math/linalg.h"

extern void initialise_monitor_handles(void);


/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
    rcc_clock_setup_hse_3v3(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    //Enable peripheral clocks
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_TIM2);
}

static void gpio_setup(void)
{
    //Configure gpio
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);//PA8

}

static void timer_setup(){

    /** TIMER 2 **/
    nvic_enable_irq(NVIC_TIM2_IRQ);//Enable interrupt
    rcc_periph_reset_pulse(RST_TIM2);//Reset timer

}

int main(void)
{

    initialise_monitor_handles();
    clock_setup();
    gpio_setup();


    int i;
    printf("Hello world!");
    scanf("%i", &i);

    mat4 a = MAT4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    mat4 b = MAT4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    mat4 c = MAT4_ZERO();


    mat_matmul((matxx*)&a, (matxx*)&b, (matxx*)&c);
    printf("Hello world1!");

    for(int j = 0; j < 4; ++j){
        printf("RSULT: %f %f %f %f\n", c.members[j*4+0], c.members[j*4+1], c.members[j*4+2], c.members[j*4+3]);
    }
    scanf("%i", &i);



    /* Set two LEDs for wigwag effect when toggling. */
    gpio_set(GPIOA, GPIO8);

    /* Blink the LEDs (PA8) on the board. */
    while (1) {
        /* Toggle LEDs. */
        gpio_toggle(GPIOA, GPIO8);
        for (i = 0; i < 6000000; i++) { /* Wait a bit. */
            __asm__("nop");
        }
    }

    return 0;
}