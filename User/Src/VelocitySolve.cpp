//
// Created by user on 2025/7/31.
//

/*
 *  相机:
 *      (UART)
 *      是否找到目标
 *      目标中心与实际中心的x,y方向的差值
 *  底盘:
 *      (UART)
 *      (用户输入
 *      运动阶段
 *      两电机的duty**
 *      编码器的值
 *
 *  输出:
 *      步进电机
 *          PWM*2（频率和次数）
 *          GPIO*4（使能+正反）
 *      小舵机:
 *          PWM*1
 *      激光笔开关:
 *          GPIO
 *
 *
 */

/*
 *  Step1:
 *      yaw轴巡逻一圈，找到目标则进去step2
 *  Step2:
 *      yaw,pitch 根据相对位置进行调整，让目标点在视野中心
 *
 *
 */
#include "VelocitySolve.hpp"

float yaw_solving(float x, float y,float v,uint8_t stage){
    float temp1 = (1/(x * x + y * y))*v;    //v maybe solid

    if(stage == 1) return (temp1 * x);
    else if(stage == 2) return (temp1 * y);
    else if(stage == 3) return -(temp1 * x);
    else if(stage == 4) return -(temp1 * y);
    else return 0;
}