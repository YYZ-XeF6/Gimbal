//
// Created by user on 2025/8/1.
//
#include "Usart.hpp"
#include "main.h"

// 定义接收数据结构体（可选，推荐使用）
typedef struct {
    uint8_t task;       // 任务号
    uint8_t is_start;   // 启动标志
    uint8_t now_lamps;  // 当前灯状态
    uint8_t encoder;    // 编码器值
    uint8_t duty_cycle0; // PWM通道0占空比
    uint8_t duty_cycle1; // PWM通道1占空比
} ReceptionData;

// 解包函数
HAL_StatusTypeDef UnpackData(uint8_t *rx_buffer, uint16_t size, ReceptionData *out) {
    // 验证数据包基本完整性
    if (size < 8) {
        return HAL_ERROR;  // 数据长度不足
    }

    // 查找帧头帧尾
    for (uint16_t i = 0; i <= size - 8; i++) {
        if (rx_buffer[i] == 0xAA && rx_buffer[i + 7] == 0x55) {
            // 提取数据 - 直接赋值形式
            // uint8_t task = rx_buffer[i + 1];
            // uint8_t is_start = rx_buffer[i + 2];
            // ... 其他字段类似

            // 使用结构体形式（推荐）
            out->task = rx_buffer[i + 1];
            out->is_start = rx_buffer[i + 2];
            out->now_lamps = rx_buffer[i + 3];
            out->encoder = rx_buffer[i + 4];
            out->duty_cycle0 = rx_buffer[i + 5];
            out->duty_cycle1 = rx_buffer[i + 6];

            return HAL_OK;
        }
    }
    return HAL_ERROR;  // 未找到有效数据包
}



// 使用示例
void USART_ReceiveCallback(UART_HandleTypeDef *huart) {
    uint8_t rx_buf[64];  // 接收缓冲区
    ReceptionData data;

    if (UnpackData(rx_buf, sizeof(rx_buf), &data) == HAL_OK) {
        // 使用解包后的数据


        // 其他处理...
        if (data.is_start) {
            // 任务启动处理
        }
    }
}


uint8_t rx_buf[16];
void usart_dma_init()
{
    HAL_UART_Receive_DMA(&huart1, rx_buf, 8);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    UnpackData(rx_buf, sizeof(rx_buf), NULL);
    usart_dma_init();
}









