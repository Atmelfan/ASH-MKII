//
// Created by atmelfan on 2018-03-12.
//

#define GAIT_START 0
#define GAIT_STOP 1
#define GAIT_STEP(x) (2+(x))

typedef struct {
    char* comment;
} gait_step;

const gait_step steps[] = {
    #include "test.gdf"
};





