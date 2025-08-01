//
// Created by user on 2025/8/1.
//

#ifndef YUNTAI_LED_H
#define YUNTAI_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

void LED_R_ON();
void LED_G_ON();
void LED_B_ON();
void LED_R_OFF();
void LED_G_OFF();
void LED_B_OFF();
void Layser_On();
void  Layser_Off();

#ifdef __cplusplus
}
#endif

#endif //YUNTAI_LED_H
