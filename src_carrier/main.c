#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>
#include "math/linalg.h"
#include "fdt/dtb_parser.h"
#include "jetson.h"

extern void initialise_monitor_handles(void);
extern fdt_header_t gait_normal;

bool mat4_from_node(fdt_token* t, mat4* m){
    if(fdt_token_get_type(t) != FDT_PROP && fdt_read_u32(&t->len) >= 16*4){
        for(int i = 0; i < 16; ++i){
            m->members[i] = fdt_read_u32(&t->cells[i])/1000.0f;
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

void mk_indent(int lvl){
    for(int i = 0; i < lvl; ++i){
        printf("\t");
    }
}

int main(void)
{

    initialise_monitor_handles();
    clock_setup();
    gpio_setup();
    jetson_batocp(false);


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

    fdt_header_t* dtb = &gait_normal;

    fdt_token* t = fdt_get_tokens(dtb);
    int indent = 0;
    while(t){
        //printf("FDT TOKEN: %d, ", (int)fdt_token_get_type(t));

        switch(fdt_token_get_type(t)){
            case FDT_BEGIN_NODE:
                mk_indent(indent);
                if(*t->name)
                    printf("%s {\n", t->name);
                else
                    printf("/ {\n");
                indent++;
                break;
            case FDT_END_NODE:
                indent--;
                mk_indent(indent);
                printf("}\n");
                break;
            case FDT_PROP:
                mk_indent(indent);
                printf("%s : %d\n", fdt_prop_name(dtb, t), (int)fdt_prop_len(dtb, t));
                break;
            case FDT_END:
                printf("EOT\n");
                break;
            default:
                mk_indent(indent);
                printf("\n");
                break;
        }

        t = fdt_token_next(dtb, t);
    }

    char buffer[64];

    printf("\n**Searching by phandle = '4'... ");
    fdt_token* p = fdt_find_phandle(dtb, 3);
    if(p)
        printf("[OK, found %s at '%s'!]\n", p->name, fdt_trace(dtb, p, buffer));
    else
        printf("[FAIL]\n");

    printf("\n**Searching by name = 'gait_start'... ");
    fdt_token* g = fdt_find_subnode(dtb, fdt_get_tokens(dtb), "gait_start");
    if(g)
        printf("[OK, found %s at '%s'!]\n", g->name, fdt_trace(dtb, g, buffer));
    else
        printf("[FAIL]\n");

    printf("\n**Running... \n");
    int count = 0;
    int num_commands = 0;
    while(g && count < 10000){
        //printf("> %s\n", g->name);
        for(fdt_token* prop = fdt_token_next(dtb, g); prop && fdt_token_get_type(prop) == FDT_PROP; prop = fdt_token_next(dtb, prop)){
            char* prop_name = fdt_prop_name(dtb, prop);
            num_commands++;
        }
        /*Next!*/
        uint32_t next = fdt_node_get_u32(dtb, g, "next", 0);
        if(next){
            g = fdt_find_phandle(dtb, next);
        }else{
            printf("Failed to find next\n");
            g = NULL;
        }
        //printf("next = %d\n", next);
        count++;
    }

    printf("10000 iterations took -s, found %d commands\n", num_commands);

    scanf("%i", &i);

    return 0;
}