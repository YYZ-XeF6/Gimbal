//
// Created by zhangzhiwen on 25-5-23.
//

#ifndef SG90_H
#define SG90_H

#ifdef __cplusplus
extern "C" {
#endif


#include "tim.h"
#include "main.h"

void SG90_init(void);
void SG90_set_angle(float angle);
void SG90_hit(void);

#ifdef __cplusplus
}
#endif

#endif //SG90_H
