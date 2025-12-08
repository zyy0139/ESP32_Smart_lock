#include "Int_WTN6170.h"

//* 语音模块初始化
void Int_WTN6170_Init(void)
{
    gpio_config_t io_config = {
        .pin_bit_mask = (1 << WTN_DATA),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};

    gpio_config(&io_config);

    //* 初始状态电平拉高
    WTN_DATA_H;

    //* 等待100ms
    vTaskDelay(100);
}

//* 语音模块发送数据
void Int_WTN6170_SendByte(uint8_t byte)
{
    //* 拉低DATA
    WTN_DATA_L;
    vTaskDelay(5);

    for (uint8_t i = 0; i < 8; i++)
    {
        //* 1 -- 高低电平比 3:1   0 -- 高低电平比 1:3
        if ((byte >> i) & 0x01)
        {
            //* 低位为1
            WTN_DATA_H;
            usleep(600);
            WTN_DATA_L;
            usleep(200);
        }
        else
        {
            //* 低位为0
            WTN_DATA_H;
            usleep(200);
            WTN_DATA_L;
            usleep(600);
        }
    }

    //* 拉高电平1ms
    WTN_DATA_H;
    vTaskDelay(1);
}
