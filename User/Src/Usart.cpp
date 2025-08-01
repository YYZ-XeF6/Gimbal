//
// Created by user on 2025/8/1.
//
#include "Usart.hpp"
#include "main.h"
#include "usart.h"

// 定义接收数据结构体（可选，推荐使用）
typedef struct {
    uint8_t task;       // 任务号
    uint8_t is_start;   // 启动标志
    uint8_t now_lamps;  // 当前灯状态
    uint8_t error;
    uint8_t encoder;    // 编码器值
} ReceptionData;

// // 解包函数
// HAL_StatusTypeDef UnpackData(uint8_t *rx_buffer, uint16_t size, ReceptionData *out) {
//     // 验证数据包基本完整性
//     if (size < 8) {
//         return HAL_ERROR;  // 数据长度不足
//     }
//
//     // 查找帧头帧尾
//     for (uint16_t i = 0; i <= size - 8; i++) {
//         if (rx_buffer[i] == 0xAA && rx_buffer[i + 7] == 0x55) {
//             // 提取数据 - 直接赋值形式
//             // uint8_t task = rx_buffer[i + 1];
//             // uint8_t is_start = rx_buffer[i + 2];
//             // ... 其他字段类似
//
//             // 使用结构体形式（推荐）
//             out->task = rx_buffer[i + 1];
//             out->is_start = rx_buffer[i + 2];
//             out->now_lamps = rx_buffer[i + 3];
//             out->encoder = rx_buffer[i + 4];
//             out->duty_cycle0 = rx_buffer[i + 5];
//             out->duty_cycle1 = rx_buffer[i + 6];
//
//             return HAL_OK;
//         }
//     }
//     return HAL_ERROR;  // 未找到有效数据包
// }



// 使用示例
// void USART_ReceiveCallback(UART_HandleTypeDef *huart) {
//     uint8_t rx_buf[64];  // 接收缓冲区
//     ReceptionData data;
//
//     if (UnpackData(rx_buf, sizeof(rx_buf), &data) == HAL_OK) {
//         // 使用解包后的数据
//
//
//         // 其他处理...
//         if (data.is_start) {
//             // 任务启动处理
//         }
//     }
// }

ReceptionData rx_data;
uint8_t rx_dma_buf[8];
extern DMA_HandleTypeDef hdma_usart2_rx;

void usart_dma_init()
{
    HAL_UART_Receive_DMA(&huart2, (uint8_t*)rx_dma_buf, 8);
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart == &huart2)
    {
        int8_t index = -1;
        for (int8_t i = 0; i < 8; i++)
        {
            if (rx_dma_buf[i] == 0xAA)
            {
                index = i;
                break;
            }
        }
        if (index >= 0 && rx_dma_buf[(index + 7) % 8] == 0x55)
        {
            rx_data.task = rx_dma_buf[(index + 1) % 8];
            rx_data.is_start = rx_dma_buf[(index + 2) % 8];
            rx_data.now_lamps = rx_dma_buf[(index + 3) % 8];
            rx_data.error = (int8_t)rx_dma_buf[(index + 4) % 8];
            rx_data.encoder = (int16_t)(rx_dma_buf[(index + 5) % 8] << 8 | rx_dma_buf[(index + 6) % 8]);
            // usart_printf("%x,%d,%d,%d,%d,%d,%x\r\n", rx_dma_buf[index],
            //              task, is_start, now_lamps, trace, encoder, rx_dma_buf[(index + 7) % 8]);
        }
        // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)rx_dma_buf, 8);
        usart_dma_init();
    }
}









