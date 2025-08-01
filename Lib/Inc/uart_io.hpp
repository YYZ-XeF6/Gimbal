//
// Created by teleaki on 25-7-25.
//

#ifndef UART_IO_H
#define UART_IO_H

#ifdef __cplusplus

#include <cstdint>
#include "usart.h"
//#include "stdint.h"

#define HEAD    0xA5
#define ESCAPE  0xC5

#define CMD_CENTER 0xB1
#define CMD_CORNERS 0xB2

#define MAX_BUFFER_SIZE 128

typedef enum {
    UARTSuccess = 0,
    HeaderError = -1,
    EscapeError = -2,
    LengthError = 1,
    CheckSumError = 2
} UART_Status;

class MCPacket {
public:
    MCPacket() {
        escape_flag_ = 0; receive_flag_ = 0; payload_len_ = 0; send_len_ = 0;
    }

    uint8_t send_buffer_[MAX_BUFFER_SIZE]{};
    uint8_t receive_buffer_[MAX_BUFFER_SIZE]{};
    uint8_t payload_buffer_[MAX_BUFFER_SIZE]{};

    void receive_packet_start();
    void send_packet(UART_HandleTypeDef *huart, const uint8_t *payload, uint8_t cmd, uint16_t len);
    void encode_packet(const uint8_t *payload, uint8_t cmd, uint16_t len);
    UART_Status decode_packet(uint16_t receive_len);

    uint8_t getReceiveFlag() { return receive_flag_; }
    void setReceiveFlag(uint8_t flag) { receive_flag_ = flag; }

    uint16_t getPayloadLength() { return payload_len_; }
    void setPayloadLength(uint16_t len) { payload_len_ = len; }

    uint8_t getReceivedCommand() { return cmd_; }

private:
    uint8_t escape_flag_;
    uint8_t receive_flag_;

    uint16_t send_len_;
    uint16_t payload_len_;

    uint8_t cmd_{};

    static uint8_t checksum(const uint8_t *data, uint16_t len);
};

inline MCPacket mc_packet;


#endif

#endif //UART_IO_H
