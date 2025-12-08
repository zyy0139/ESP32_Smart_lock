#ifndef __INT_SC12B_H__
#define __INT_SC12B_H__
#include "driver/gpio.h"
#include "driver/i2c.h"

//* 宏定义引脚
#define SC_SCL GPIO_NUM_1
#define SC_SDA GPIO_NUM_2
#define SC_INT GPIO_NUM_0

//* 宏定义从机地址
#define DEVICE_ADDR 0x42

//* 宏定义i2c传输速度
#define I2C_MASTER_FREQ_HZ 400000

typedef enum
{
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SHARP,
    KEY_M,
    KEY_NO
} Touch_Key;

void Int_SC12B_Init(void);

Touch_Key Int_SC12B_GetKeyNum(void);

#endif /* __INT_SC12B_H__ */
