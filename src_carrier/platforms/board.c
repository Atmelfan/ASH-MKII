//
// Created by atmelfan on 2018-03-24.
//

#include <stdio.h>
#include "board.h"
#include "dev.h"

/**
 * Defined inside plaform/board_name/board_name.c
 */
extern fdt_header_t* board;

void board_init_fdt(fdt_header_t *fdt, fdt_token *root) {
    dev_init_drivers();

    if(!fdt){
        fdt = board;
        root = fdt_get_tokens(board);
    }

    for(fdt_token* d = fdt_token_next(fdt, root); fdt_token_get_type(d) != FDT_END_NODE ; d = fdt_token_next(fdt, d)){
        /*Check for nodes*/
        if(fdt_token_get_type(d) == FDT_BEGIN_NODE){
            char* compatible = fdt_node_get_str(fdt, d, "compatible", NULL);
            dev_driver* drv = dev_find_driver(compatible);
            if(drv){
                if(drv->probe(fdt, d, 0) == PROBE_SUCCESS){

                }else{
                    char buf[100];
                    printf("[ERROR] Failed to probe %s!\n", fdt_trace(fdt, d, buf));
                }
            }else{
                char buf[100];
                printf("[ERROR] No driver for %s!\n", fdt_trace(fdt, d, buf));
            }


            /*Exit node*/
            d = fdt_node_end(fdt, d);
        }
    }

    board_init();
}
