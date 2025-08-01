//
// Created by zhangzhiwen on 25-7-26.
//

#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

#ifdef __cplusplus
//extern "C" {
//#endif

#include "main.h"
#include "tim.h"

#define L (-1)
#define R (1)

class StepMotorC
{
public:
    StepMotorC(TIM_HandleTypeDef* htim, uint32_t TIM_CHANNEL,
               GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
               GPIO_TypeDef* DR_PORT, uint16_t DR_PIN);

    void Enable();
    void Disable();
    void Start();
    void Stop();
    void Turn(int8_t dir);
    void SetSpeed(float target_speed);
    void MoveToTarget(float target_angle);
    void MoveAngle(float delta_angle);
    void UpdateAngleCallback();

    float angle = 0;
    float angle_last = 0;
    float speed = 0;
    int8_t direction = R;
    float target = 0;
    float step = 0.05625; // 360.0f / 200.0f / 32.0f
    uint16_t period = 65535;
    bool is_enable = false;
    bool is_start = false;

private:
    TIM_HandleTypeDef* htim_;
    uint32_t TIM_CHANNEL_;
    GPIO_TypeDef* EN_PORT_;
    uint16_t EN_PIN_;
    GPIO_TypeDef* DR_PORT_;
    uint16_t DR_PIN_;
    const float FREQUENCY_ = 1000000.0f;
    const uint16_t PRESCALER_ = 167;
    const float MAX_SPEED_ = 720; // FREQUENCY_*step/(prescaler+1)/360.0f/2.0f
    const float MIN_SPEED_ = 0.85; // FREQUENCY_*step/(prescaler+1)/360.0f/65535.0f
};

extern StepMotorC gimbal_yaw;
extern StepMotorC gimbal_pitch;

//#ifdef __cplusplus
//}
#endif

#endif //STEP_MOTOR_H
