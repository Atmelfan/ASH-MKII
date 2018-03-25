//
// Created by atmelfan on 2018-03-24.
//

#include <math.h>
#include "linalg_util.h"
#include "linalg.h"

bool transform_valid(const mat4* mat){
    for(int i = 0; i < 9; ++i){
        float f = mat->members[i%3 + 3*(i/3)];
        if(fabs(f) > 1.0f){
            return false;
        }
    }

    return mat->members[15] == 1.0f
            && mat->members[14] == 0.0f
            && mat->members[13] == 0.0f
            && mat->members[12] == 0.0f;
}
