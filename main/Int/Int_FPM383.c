#include "Int_FPM383.h"

static const int RX_BUF_SIZE = 1024;

//* 存储接收应答的数据
uint8_t receive_buffers[50] = {1}; //* 不能初始化为0,否则会直接判定为接收成功

//* 识别用户是否输入指纹
uint8_t finger_touch_down = 0;

//* 计算校验和
static void Int_FPM383_CheckSum(uint8_t arr[], uint8_t length)
{
    uint16_t sum = 0;
    for (uint8_t i = 6; i < length - 2; i++)
    {
        sum += arr[i];
    }

    arr[length - 2] = (sum >> 8) & 0xFF;
    arr[length - 1] = (sum >> 0) & 0xFF;
}

//* 利用串口发送命令
static void Int_FPM383_SendCmd(uint8_t cmds[], uint8_t length)
{
    uart_write_bytes(UART_NUM_1, cmds, length);
}

//* 利用串口接收应答数据
static void Int_FPM383_ReceiveAck(uint32_t length, TickType_t waitTime)
{
    //* 接收之前需将缓冲区清空
    memset(receive_buffers, 1, 50); //! 注意重置为1,不是0
    //* 接收
    uart_read_bytes(UART_NUM_1, receive_buffers, length, waitTime);
}

//* FPM383 -- 初始化
void Int_FPM383_Init(void)
{
}

//* FPM383 -- 进入休眠状态
void Int_FPM383_Sleep(void)
{
}
