//
// Created by user on 2025/8/1.
//
#include "User_main.hpp"

void User_Init(){
    HAL_UART_Init(&huart1);
    HAL_UART_Init(&huart6);

//    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
//    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    gimbal_pitch.Start();
    gimbal_yaw.Start();

    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);

    HAL_Delay(10);
}

void User_Loop(){
    LED_R_ON();
    gimbal_pitch.SetSpeed(15.0);
    gimbal_yaw.SetSpeed(15.0);
    HAL_Delay(1000);
    LED_R_OFF();
    gimbal_pitch.SetSpeed(-15.0);
    gimbal_yaw.SetSpeed(-15.0);
    HAL_Delay(1000);

}