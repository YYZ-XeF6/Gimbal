//
// Created by teleaki on 25-7-25.
//

#include "uart_io.hpp"

#include <cstring>

#include "main.h"
#include "usart.h"

/**
 * @brief 计算校验和
 */
uint8_t MCPacket::checksum(const uint8_t *data, uint16_t len) {
    uint8_t check_sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        check_sum += data[i];
    }
    return check_sum;
}

/**
 * @brief 开始用DMA方法接收包
 */
void MCPacket::receive_packet_start() {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, receive_buffer_, sizeof(receive_buffer_));
}

/**
 * @brief 将原始数据编码并发包
 */
void MCPacket::send_packet(UART_HandleTypeDef *huart, const uint8_t *payload, uint8_t cmd, uint16_t len) {
    encode_packet(payload, cmd, len);
    HAL_UART_Transmit(huart, send_buffer_, send_len_, HAL_MAX_DELAY);
}

/**
 * @brief 对数据编码，格式：HEAD(1B) + CMD(1B) + PayloadSize(2B) + Payload(len B) + Checksum(1B)
 */
void MCPacket::encode_packet(const uint8_t *payload, uint8_t cmd, uint16_t len) {
    uint16_t temp_buffer_len = len + 5;
    uint8_t temp_buffer[temp_buffer_len];

    temp_buffer[0] = HEAD;

    temp_buffer[1] = cmd;

    temp_buffer[2] = (uint8_t)len;
    temp_buffer[3] = (uint8_t)(len >> 8);

    memcpy(temp_buffer + 4, payload, len);

    temp_buffer[len + 4] = checksum(temp_buffer, temp_buffer_len - 1);

    send_buffer_[0] = HEAD;
    uint16_t index = 1;
    for (uint16_t i = 1; i < temp_buffer_len; i++) {
        if (temp_buffer[i] == HEAD || temp_buffer[i] == ESCAPE) {
            send_buffer_[index++] = ESCAPE;
            send_buffer_[index++] = temp_buffer[i] - 1;
        } else {
            send_buffer_[index++] = temp_buffer[i];
        }
    }
    send_len_ = index;
}

/**
 * @brief 对数据解码并返回状态
 */
UART_Status MCPacket::decode_packet(uint16_t receive_len) {
    if (receive_len < 5) {
        return LengthError;
    }
    if (receive_buffer_[0] != HEAD) {
        return HeaderError;
    }

    uint8_t packet_buffer[receive_len];
    uint16_t index = 0;
    for (uint16_t i = 0; i < receive_len; i++) {
        if (escape_flag_) {
            packet_buffer[index++] = receive_buffer_[i] + 1;
            escape_flag_ = 0;
        } else {
            if (receive_buffer_[i] == ESCAPE) {
                escape_flag_ = 1;
            } else {
                packet_buffer[index++] = receive_buffer_[i];
            }
        }
    }

    cmd_ = packet_buffer[1];

    uint16_t payload_len = (uint16_t)packet_buffer[2] +
                           (uint16_t)(packet_buffer[3] << 8);
    if (payload_len + 5 != index) {
        return LengthError;
    }

    uint8_t check_sum = packet_buffer[index - 1];
    if (checksum(packet_buffer, index - 1) != check_sum) {
        return CheckSumError;
    }

    payload_len_ = payload_len;
    memcpy(payload_buffer_, packet_buffer + 4, payload_len);

    return UARTSuccess;
}

/**
 * @brief 接收回调
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart == &huart1) {
        UART_Status flag = mc_packet.decode_packet(Size);
        if (flag == UARTSuccess) {
            mc_packet.setReceiveFlag(1);
        }

        mc_packet.receive_packet_start();
    }
}
