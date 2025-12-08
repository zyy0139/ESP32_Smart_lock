#include "Int_SC12B.h"

//* 定义按钮被按下的标志位
uint8_t sc12b_flag = 0;

//* i2c写数据
static void Int_SC12B_WriteReg(uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buffers[2] = {reg_addr, data};
    i2c_master_write_to_device(I2C_NUM_0, DEVICE_ADDR, write_buffers, sizeof(write_buffers), 1000);
}

//* i2c读数据
static uint8_t Int_SC12B_ReadReg(uint8_t reg_addr)
{
    uint8_t rx_byte = 0;
    i2c_master_write_read_device(I2C_NUM_0, DEVICE_ADDR, &reg_addr, 1, &rx_byte, 1, 1000);
    return rx_byte;
}

static void SC12B_Handler(void *gpio_pin)
{
    sc12b_flag = 1;
}

//* SC12B初始化 -- i2c初始化
void Int_SC12B_Init(void)
{
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,               // 主设备
        .scl_io_num = SC_SCL,                  // 时钟线
        .sda_io_num = SC_SDA,                  // 数据线
        .scl_pullup_en = GPIO_PULLUP_ENABLE,   // 接上拉
        .sda_pullup_en = GPIO_PULLUP_ENABLE,   // 接上拉
        .master.clk_speed = I2C_MASTER_FREQ_HZ // 传输速度
    };

    //* 配置i2c
    i2c_param_config(I2C_NUM_0, &i2c_config);

    //* 开启i2c使能
    i2c_driver_install(I2C_NUM_0, i2c_config.mode, 0, 0, 0);

    //* 调整灵敏度
    Int_SC12B_WriteReg(0x00, 0x25);
    Int_SC12B_WriteReg(0x01, 0x25);

    //* 配置引脚中断

    gpio_config_t io_config = {
        .pin_bit_mask = (1 << SC_INT),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_POSEDGE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};

    gpio_config(&io_config);

    //* 配置中断优先级
    gpio_install_isr_service(0);

    //* 添加中断处理程序
    gpio_isr_handler_add(SC_INT, SC12B_Handler, (void *)SC_INT);
}

Touch_Key Int_SC12B_GetKeyNum(void)
{
    Touch_Key key_num = KEY_NO;

    if (sc12b_flag)
    {
        //* 读取寄存器output1和output2
        uint16_t result = (Int_SC12B_ReadReg(0x08) << 8) | (Int_SC12B_ReadReg(0x09));

        switch (result)
        {
        case 0x8000:
            key_num = KEY_0;
            break;
        case 0x4000:
            key_num = KEY_1;
            break;
        case 0x2000:
            key_num = KEY_2;
            break;
        case 0x1000:
            key_num = KEY_3;
            break;

        case 0x0800:
            key_num = KEY_7;
            break;
        case 0x0400:
            key_num = KEY_5;
            break;
        case 0x0200:
            key_num = KEY_6;
            break;
        case 0x0100:
            key_num = KEY_4;
            break;

        case 0x0080:
            key_num = KEY_M;
            break;
        case 0x0040:
            key_num = KEY_8;
            break;
        case 0x0020:
            key_num = KEY_9;
            break;
        case 0x0010:
            key_num = KEY_SHARP;
            break;

        default:
            break;
        }

        //* 重置标志位
        sc12b_flag = 0;
    }

    return key_num;
}
