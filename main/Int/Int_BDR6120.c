#include "Int_BDR6120.h"

// 待命状态
static void Int_BDR6120_StandBy(void);
// 前进
static void Int_BDR6120_Forward(void);
// 后退
static void Int_BDR6120_Back(void);
// 刹车
static void Int_BDR6120_Break(void);

//* DBR6120初始化
void Int_BDR6120_Init(void)
{
    //* 初始化GPIO
    gpio_config_t io_config = {};
    //* 确定使用的引脚
    io_config.pin_bit_mask = (1 << BDR_A) | (1 << BDR_B); //* 这里使用的是掩码
    //* 设定引脚为输入模式
    io_config.mode = GPIO_MODE_OUTPUT;
    //* 关闭中断
    io_config.intr_type = GPIO_INTR_DISABLE;
    //* 是否需要下拉和上拉
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;

    //* 配置gpio
    gpio_config(&io_config);

    //* 初始状态设为待命状态
    Int_BDR6120_StandBy();
}

//* 电机开门
void Int_BDR6120_OpenDoor(void)
{
    //* 前进
    Int_BDR6120_Forward();
    vTaskDelay(1000);

    //* 刹车
    Int_BDR6120_Break();
    vTaskDelay(1000);

    //* 后退
    Int_BDR6120_Back();
    vTaskDelay(1000);

    //* 刹车
    Int_BDR6120_Break();
    vTaskDelay(1000);
}

//* 待命状态
void Int_BDR6120_StandBy(void)
{
    gpio_set_level(BDR_A, 0);
    gpio_set_level(BDR_B, 0);
}

//* 前进状态
void Int_BDR6120_Forward(void)
{
    gpio_set_level(BDR_A, 1);
    gpio_set_level(BDR_B, 0);
}

//* 后退状态
void Int_BDR6120_Back(void)
{
    gpio_set_level(BDR_A, 0);
    gpio_set_level(BDR_B, 1);
}

//* 刹车状态
void Int_BDR6120_Break(void)
{
    gpio_set_level(BDR_A, 1);
    gpio_set_level(BDR_B, 1);
}
