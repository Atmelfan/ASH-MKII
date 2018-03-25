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

void board_init_fdt(const fdt_token* root){
    dev_init_drivers();

    for(fdt_token* d = fdt_get_tokens(board); fdt_token_get_type(d) != FDT_END_NODE ; d = fdt_token_next(board, d)){
        /*Check for nodes*/
        if(fdt_token_get_type(d) == FDT_BEGIN_NODE){
            char* compatible = fdt_node_get_str(board, d, "compatible", NULL);
            dev_driver* drv = dev_find_driver(compatible);
            if(drv){
                if(drv->probe(board, d)){

                }else{
                    char buf[100];
                    printf("[ERROR] Failed to probe %s!", fdt_trace(board, d, buf));
                }
            }else{

            }


            /*Exit node*/
            d = fdt_node_end(board, d);
        }
    }

    board_init();
}
