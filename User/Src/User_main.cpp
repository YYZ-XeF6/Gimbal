//
// Created by user on 2025/8/1.
//
#include "User_main.hpp"

void User_Init(){
//    HAL_UART_Init(&huart1);
    mc_packet.receive_packet_start();
    HAL_UART_Init(&huart6);

//    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
//    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
//    gimbal_pitch.Start();
//    gimbal_yaw.Start();


    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);



    HAL_Delay(10);
}

void User_Loop(){
/*
 * DMA自动接收串口信息，循环中解包
 *
 * 如果解到特定任务，Finding()
 *
 * 找到目标后，Tracking()
 *
 * Tracking()中根据当前的编码器的值，得到位移
 * 根据duty，得到角速度
 *
 * 计算得到yaw的目标角速度(pitch)
 * 根据底盘得到角速度修正值
 *
 * 解摄像头包+计算得到角速度补偿值
 *
 * 直到收到task=0出循环
 */
        //接收底盘信息，得到当前任务
        //N>5进入瞄准模式


}

void Finding(){
    gimbal_yaw.SetSpeed(360);
    while(1){
        if(mc_packet.payload_buffer_[0] != 0)break;
    }
    Aiming();
}

void Aiming(){  //单考虑摄像头值进行修正
    float yaw_compensation;
    float pitch_compensation;

    uint16_t corners[4][2] = {
            mc_packet.payload_buffer_[1],mc_packet.payload_buffer_[0],
            mc_packet.payload_buffer_[3],mc_packet.payload_buffer_[2],
            mc_packet.payload_buffer_[5],mc_packet.payload_buffer_[4],
            mc_packet.payload_buffer_[7],mc_packet.payload_buffer_[6]

    };
    pt.calcRectCenter(corners);
    uint16_t X_diff = abs(pt.getRawCenterX()-112);
    uint16_t Y_diff = abs(pt.getRawCenterY()-112);
    if(X_diff > 3 || Y_diff > 3){
        yaw_compensation = get_compensation(X_diff);
        pitch_compensation = get_compensation(Y_diff);
        gimbal_yaw.SetSpeed(yaw_compensation);
        gimbal_pitch.SetSpeed(pitch_compensation);
    }else{
        Layser_On();
        Tracking();
    }
}

void Tracking(){
    //解包
    while(1){
        //底盘通信计算角速度理论值
        float yaw_theory;       //固定速度，一个定时器用来计算当前位置
        float pitch_theory;     // 0
        uint8_t stage;
        float x;
        float y;
        if(stage % 4 == 1){
            x = 0.5; y = 0.5 + v * t;
        }else if(stage % 4 == 2){
            x = 0.5 - v * t; y = 1.5;
        }else if(stage % 4 == 3){
            x = -0.5; y = 1.5 - v * t;
        }else{
            x = -0.5 + v * t; y = 0.5;
        }

        yaw_theory = yaw_solving(x, y, stage);

        //底盘通信得到角速度修正值
        float yaw_fix;
        Kp = ?;
        float



        //底盘通信，计算角速度理论值、修正值
        float yaw_compensation;
        float pitch_compensation;

        uint16_t corners[4][2] = {
                mc_packet.payload_buffer_[1],mc_packet.payload_buffer_[0],
                mc_packet.payload_buffer_[3],mc_packet.payload_buffer_[2],
                mc_packet.payload_buffer_[5],mc_packet.payload_buffer_[4],
                mc_packet.payload_buffer_[7],mc_packet.payload_buffer_[6]

        };
        pt.calcRectCenter(corners);
        uint16_t X_diff = abs(pt.getRawCenterX()-112);
        uint16_t Y_diff = abs(pt.getRawCenterY()-112);
        yaw_compensation = get_compensation(X_diff);
        pitch_compensation = get_compensation(Y_diff);
        gimbal_yaw.SetSpeed(yaw_compensation);
        gimbal_pitch.SetSpeed(pitch_compensation);

        uint8_t trans[2] = {(uint8_t)(yaw_compensation),(uint8_t)(pitch_compensation)};
        HAL_UART_Transmit(&huart6,trans,sizeof (trans),HAL_MAX_DELAY);
    }
}

float get_compensation(uint16_t diff){
    if(diff <=5)return MIN_SPEED;
    else if(diff <= 40) return 5;
    else if(diff <= 80) return 20;
    else return MIN_SPEED;
}