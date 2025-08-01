//
// Created by zhangzhiwen on 25-7-26.
//

#include "step_motor.hpp"

#include "cmath"

StepMotorC::StepMotorC(TIM_HandleTypeDef* htim, uint32_t TIM_CHANNEL,
                       GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
                       GPIO_TypeDef* DR_PORT, uint16_t DR_PIN)
    : htim_(htim), TIM_CHANNEL_(TIM_CHANNEL),
      EN_PORT_(EN_PORT), EN_PIN_(EN_PIN),
      DR_PORT_(DR_PORT), DR_PIN_(DR_PIN)
{
    __HAL_TIM_SET_PRESCALER(htim_, PRESCALER_);
}

void StepMotorC::Enable()
{
    HAL_GPIO_WritePin(EN_PORT_, EN_PIN_, GPIO_PIN_SET);
    is_enable = true;
}

void StepMotorC::Disable()
{
    HAL_GPIO_WritePin(EN_PORT_, EN_PIN_, GPIO_PIN_RESET);
    is_enable = false;
}

void StepMotorC::Start()
{
    HAL_TIM_PWM_Start_IT(htim_, TIM_CHANNEL_);
    HAL_TIM_PWM_Start(htim_, TIM_CHANNEL_);
    is_start = true;
}

void StepMotorC::Stop()
{
    HAL_TIM_PWM_Stop_IT(htim_, TIM_CHANNEL_);
    HAL_TIM_PWM_Stop(htim_, TIM_CHANNEL_);
    is_start = false;
}

void StepMotorC::Turn(int8_t dir)
{
    if (dir == direction) { return; }

    if (direction == TURN_LEFT) { direction = TURN_RIGHT; }
    else if (direction == TURN_RIGHT) { direction = TURN_LEFT; }
    HAL_GPIO_TogglePin(DR_PORT_, DR_PIN_);
}

// degree pre sec
void StepMotorC::SetSpeed(float target_speed)
{
    if(target_speed < 0){
        Turn(-1);
        speed = - target_speed;
    } else{
        Turn(1);
        speed = target_speed;
    }
    if (speed > MAX_SPEED_) { speed = MAX_SPEED_; }
    if (speed < MIN_SPEED_) { speed = MIN_SPEED_; }
//    counter = (uint16_t)(FREQUENCY_ * step / 360.0f / speed);
    counter = (uint16_t)(FREQUENCY_ * step / speed);
    __HAL_TIM_SET_COUNTER(htim_, counter);
    __HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_, counter/2);
}

void StepMotorC::MoveToTarget(float target_angle)
{
    target = target_angle;
    if (target < angle) { Turn(TURN_LEFT); }
    else if (target > angle) { Turn(TURN_RIGHT); }

    if (abs(target - angle) > step) { Start(); }
    else { Stop(); }
}

void StepMotorC::MoveAngle(float delta_angle)
{
    if (delta_angle < step && delta_angle > -step) { return; }
    target += delta_angle;
    MoveToTarget(target);
}

void StepMotorC::UpdateAngleCallback()
{
    if (is_enable && is_start)
    {
        if (direction == TURN_LEFT) { angle -= step; }
        else if (direction == TURN_RIGHT) { angle += step; }
    }
    if (abs(target - angle) <= step) { Stop(); }
}

StepMotorC gimbal_yaw(&htim1, TIM_CHANNEL_4,DIRECTION_1_GPIO_Port, DIRECTION_1_Pin,
    DIRECTION_1_GPIO_Port, DIRECTION_1_Pin);

StepMotorC gimbal_pitch(&htim3, TIM_CHANNEL_1,DIRECTION_2_GPIO_Port, DIRECTION_2_Pin,
    DIRECTION_2_GPIO_Port, DIRECTION_2_Pin);

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM1)
    {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            gimbal_yaw.UpdateAngleCallback();
        }
        else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            gimbal_pitch.UpdateAngleCallback();
        }
    }
}
