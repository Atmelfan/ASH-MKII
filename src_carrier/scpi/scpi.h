//
// Created by atmelfan on 4/1/18.
//

#ifndef SRC_CARRIER_SCPI_H
#define SRC_CARRIER_SCPI_H


#include <stdint.h>

#define SCPI_IDN_STR "GPA Robotics, GPA-AIX-323b, 1.0.0"

typedef struct scpi_command_t scpi_command_t;

typedef enum {
    SCPI_SUCCESS = 0,
    SCPI_NOCOMMAND
} scpi_status_t;

typedef struct {
    int16_t errcode;
    char* errstr;
} scpi_error_t;

typedef struct {
    /* Root command */
    scpi_command_t* root;
    /* Last command */
    scpi_command_t* last;

    char* buffer;
} scpi_context_t;

struct scpi_command_t{

    /*Name of command*/
    char* name;

    /**
     *
     * @param context
     * @param args
     * @return
     */
    scpi_status_t (*set)(const scpi_context_t* context, char* args);

    /**
     *
     * @param context
     * @param args
     * @return
     */
    scpi_status_t (*get)(const scpi_context_t* context, char* args);

    /* Next command in linked list */
    scpi_command_t* next;
};



#endif //SRC_CARRIER_SCPI_H
