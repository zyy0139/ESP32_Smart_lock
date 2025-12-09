#include "App_IO.h"

//* 定义用户输入缓冲区
uint8_t first_buffers[50];
uint8_t second_buffers[50];

//* 用户输入的次数,用于判断是第一次还是第二次输入密码
uint8_t send_flag = 1;

//* 添加管理员用户
static void App_IO_AddAdmin(void);

//* 重置缓冲数组
static void App_IO_ResetBuffers(void);

//* 模块初始化
void App_IO_Init(void)
{
    Int_BDR6120_Init();
    Int_SC12B_Init();
    Int_WS2812_Init();
    Int_WTN6170_Init();
    Dri_NVS_Init();
}

Com_Status App_IO_GetUserInputContent(uint8_t buffers[])
{
    uint8_t timeout = 0; // 存储超时时间
    uint8_t index = 0;   // 输入数组索引
    while (1)
    {
        //* 获取用户输入数据
        Touch_Key key_num = Int_SC12B_GetKeyNum();
        if (key_num == KEY_NO)
        {
            //* 没有按下按键
            timeout++;
            if (timeout >= 60)
            {
                //* 超过3s即为超时
                return Com_TIMEOUT;
            }
        }
        else
        {
            //* 超时之前按下了按键
            timeout = 0;

            //* 添加按键声音
            sayWithoutInt();
            sayWaterDrop();
            //* 点亮对应的led
            Int_WS2818_LED_Off_All();
            Int_WS2812_LED_On(key_num, cyan);

            if (key_num == KEY_SHARP)
            {
                //* 用户按下了#,视为输入完成
                return Com_OK;
            }

            if (key_num == KEY_M)
            {
                //* 用户按下了M视为非法操作
                return Com_ERROR;
            }

            buffers[index++] = key_num + 48; // 转换为字符存储到缓冲数组中
        }
        vTaskDelay(50); // 50ms执行一次
    }
}

void App_IO_Handler(uint8_t receive_buffers[])
{
}

void App_IO_AddAdmin(void)
{
}

void App_IO_ResetBuffers(void)
{
}
