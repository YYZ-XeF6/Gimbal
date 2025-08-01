//
// Created by user on 2025/8/1.
//

#ifndef YUNTAI_USER_MAIN_HPP
#define YUNTAI_USER_MAIN_HPP

#ifdef __cplusplus
extern "C" {
#endif

/*
 * BlueTooth:USART1
 * MaixCam:USART6
 * Yaw:
 *      TIM1_CH4
 *      Yaw_Direction
 * Pitch:
 *      TIM3_CH1
 *      Pitch_Direction
 * Servo:
 *      TIM8_CH2
 * LED:
 *      LED_R
 *      LED_G
 *      LED_B
 * layser:
 *      layser
 */

#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"

#include "LED.hpp"
#include "SG90.hpp"
#include "step_motor.hpp"
#include "VelocitySolve.hpp"

#include "perspective.hpp"
#include "uart_io.hpp"


void User_Init();
void User_Loop();
void Finding();
void Tracking();
float get_compensation(uint16_t diff);

#ifdef __cplusplus
}
#endif

#endif //YUNTAI_USER_MAIN_HPP
