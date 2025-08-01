//
// Created by zhangzhiwen on 25-5-23.
//

#include "SG90.hpp"
#include "tim.h"

void SG90_init(void)
{
    HAL_TIM_Base_Start_IT(&htim8);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
//    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    SG90_set_angle(0);
//    HAL_Delay(1000);
//    SG90_set_angle(180);
//    HAL_Delay(1000);
//    SG90_set_angle(0);
}

void SG90_set_angle(float angle)
{
    if (angle < 0) {angle = 0;}
    if (angle > 180) {angle = 180;}
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, (int)(angle * 1000 * 2 / 180 + 500));
}

void SG90_hit(void)
{
    // SG90_set_angle(45);
    HAL_Delay(1000);
    SG90_set_angle(0);
    HAL_Delay(1000);
    SG90_set_angle(60);
}