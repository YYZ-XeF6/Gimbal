//
// Created by user on 2025/8/1.
//
#include "LED.hpp"

void LED_R_ON(){
    HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
}

void LED_G_ON(){
    HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin,GPIO_PIN_SET);
}

void LED_B_ON(){
    HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_SET);
}

void LED_R_OFF(){
    HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET);
}

void LED_G_OFF(){
    HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin,GPIO_PIN_RESET);
}

void LED_B_OFF(){
    HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_RESET);
}

void Layser_On(){
    HAL_GPIO_WritePin(Layser_GPIO_Port,Layser_Pin,GPIO_PIN_SET);
}
void  Layser_Off(){
    HAL_GPIO_WritePin(Layser_GPIO_Port, Layser_Pin,GPIO_PIN_RESET);
}