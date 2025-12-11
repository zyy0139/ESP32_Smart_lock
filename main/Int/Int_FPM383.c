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

//* 指纹中断服务
static void Int_FPM383_handler(void *pin)
{
    esp_rom_printf("touch down\r\n");
    //* 切换标志位
    finger_touch_down = 1;
}

//* FPM383 -- 初始化
void Int_FPM383_Init(void)
{
    //* 串口相关配置
    const uart_config_t uart_config = {
        .baud_rate = 57600,                    // 波特率
        .data_bits = UART_DATA_8_BITS,         // 数据位是8位
        .parity = UART_PARITY_DISABLE,         // 禁用奇偶校验
        .stop_bits = UART_STOP_BITS_1,         // 1位停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 禁用硬件流功能
        .source_clk = UART_SCLK_DEFAULT        // 串口时钟源
    };

    uart_param_config(UART_NUM_1, &uart_config);

    //* 开启串口使能
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);

    //* 配置uart收发引脚
    uart_set_pin(UART_NUM_1, FPM383_TX_PIN, FPM383_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //* 添加外部中断
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_POSEDGE, // 上升沿触发
        .mode = GPIO_MODE_INPUT,        // 输入模式
        .pin_bit_mask = (1 << FPM383_TOUCH_OUT),
        .pull_down_en = GPIO_PULLDOWN_ENABLE, // 允许下拉
        .pull_up_en = GPIO_PULLUP_DISABLE     // 禁用上拉
    };

    gpio_config(&io_config);

    //* 配置中断优先级
    gpio_install_isr_service(0);

    //* 添加中断服务函数
    gpio_isr_handler_add(FPM383_TOUCH_OUT, Int_FPM383_handler, (void *)FPM383_TOUCH_OUT);

    //* 关闭外部中断
    gpio_intr_disable(FPM383_TOUCH_OUT);

    //* 延迟一段时间后再发送命令
    vTaskDelay(150); //* 150ms

    //* 进入休眠状态
    Int_FPM383_Sleep();
}

//* FPM383 -- 进入休眠状态
void Int_FPM383_Sleep(void)
{
    //* 定义命令数组
    uint8_t cmds[12] = {
        0xEF, 0x01,             // 包头
        0xff, 0xff, 0xff, 0xff, // 设备地址
        0x01,                   // 包标识
        0x00, 0x03,             // 包长度
        0x33,                   // 指令码
        0x00, 0x37              // 校验和
    };

    //* 发送多次休眠命令,直到收到正确响应为止
    do
    {
        //* 发送sleep命令
        Int_FPM383_SendCmd(cmds, 12);
        //* 接收应答数据
        Int_FPM383_ReceiveAck(12, 1000);
    } while (receive_buffers[9] != 0x00);

    //* 开启中断
    gpio_intr_enable(FPM383_TOUCH_OUT);

    esp_rom_printf("fpm383 sleep!\r\n");
}

//* 获取芯片序列号 -- 测试
void Int_FPM383_GetChipID(void)
{
    //* 定义命令数组
    uint8_t cmds[13] = {
        0xEF, 0x01,             // 包头
        0xff, 0xff, 0xff, 0xff, // 设备地址
        0x01,                   // 包标识
        0x00, 0x04,             // 包长度
        0x34,                   // 指令码
        0x00,                   // 参数
        0x00, 0x39              // 校验和
    };

    //* 发送命令
    Int_FPM383_SendCmd(cmds, 13);
    //* 接收应答数据
    Int_FPM383_ReceiveAck(44, 2000);

    if (receive_buffers[9] == 0x00)
    {
        //* 获取成功
        esp_rom_printf("%.32s\r\n", &receive_buffers[10]);
    }
    else
    {
        //* 获取失败
        esp_rom_printf("GetChipID ERROR\r\n");
    }
}

//* 获取最近空闲索引表页码
uint16_t Int_FPM383_GetMinID(void)
{
    //* 编写命令
    uint8_t cmds[13] = {
        0xEF, 0x01,             // 包头
        0xff, 0xff, 0xff, 0xff, // 设备地址
        0x01,                   // 包标识
        0x00, 0x04,             // 包长度
        0x1f,                   // 指令码
        0x00,                   // 页码
        '\0', '\0'              // 校验和
    };

    //* 计算校验和
    Int_FPM383_CheckSum(cmds, 13);

    //* 发送命令
    Int_FPM383_SendCmd(cmds, 13);

    //* 接收应答
    Int_FPM383_ReceiveAck(44, 2000);

    //* 判断确认码
    if (receive_buffers[9] == 0x00)
    {
        //* 获取应答中的索引信息
        for (uint8_t i = 0; i < 32; i++)
        {
            uint8_t byte = receive_buffers[i + 10]; // 索引信息长32字节,从数组第11位开始
            for (uint8_t j = 0; j < 8; j++)
            {
                if (((byte >> j) & 0x01) == 0)
                {
                    return 8 * i + j;
                }
            }
        }
    }
    return 0;
}

//* 录入指纹
Com_Status Int_FPM383_RegisterFinger(uint16_t minID)
{
    //* 编写命令
    /*
        参数：最低位为bit0。
        1) bit0：采图背光灯控制位，0-LED长亮，1-LED获取图像成功后灭； -- 1
        2) bit1：采图预处理控制位，0-关闭预处理，1-打开预处理； -- 1
        3) bit2：注册过程中，是否要求模组在关键步骤，返回当前状态，0-要求返回，1-不
        要求返回；-- 0
        4) bit3：是否允许覆盖ID号，0-不允许，1-允许； -- 0
        5) bit4：允许指纹重复注册控制位，0-允许，1-不允许；-- 1
        6) bit5：注册时，多次指纹采集过程中，是否要求手指离开才能进入下一次指纹图
        像采集，0-要求离开；1-不要求离开；-- 1
        7) bit6~bit15：预留。 -- 0
    */
    uint8_t cmds[17] = {
        0xEF, 0x01,             // 包头
        0xff, 0xff, 0xff, 0xff, // 设备地址
        0x01,                   // 包标识
        0x00, 0x08,             // 包长度
        0x31,                   // 指令码
        '\0', '\0',             // ID号
        0x01,                   // 录入次数
        0x00, 0x33,             // 参数
        '\0', '\0'              // 校验和
    };

    //* 注入ID
    cmds[10] = (minID >> 8) & 0xff;
    cmds[11] = (minID >> 0) & 0xff;

    //* 注入校验和
    Int_FPM383_CheckSum(cmds, 17);

    //! 由于硬件需求,在注册指纹之前,需要取消注册四次
    Int_FPM383_Cancel();
    Int_FPM383_Cancel();
    Int_FPM383_Cancel();
    Int_FPM383_Cancel();

    //* 发送注册命令
    Int_FPM383_SendCmd(cmds, 17);

    //* 接收应答数据
    do
    {
        Int_FPM383_ReceiveAck(14, 1000);
        if (receive_buffers[9] != 0x00)
        {
            return Com_ERROR;
        }
    } while (receive_buffers[10] != 0x06);

    //* 指纹注册成功
    return Com_OK;
}

//* 取消录入
void Int_FPM383_Cancel(void)
{
    //* 编写命令
    uint8_t cmds[12] = {
        0xEF, 0x01,             // 包头
        0xff, 0xff, 0xff, 0xff, // 设备地址
        0x01,                   // 包标识
        0x00, 0x03,             // 包长度
        0x30,                   // 指令码
        '\0', '\0'              // 校验和
    };

    //* 计算校验和
    Int_FPM383_CheckSum(cmds, 12);

    //* 循环发送命令,直到取消录入成功
    do
    {
        Int_FPM383_SendCmd(cmds, 12);
        Int_FPM383_ReceiveAck(12, 1000);
        esp_rom_printf("FPM383 Cancel....\r\n");
    } while (receive_buffers[9] != 0x00);

    //* 取消成功
    esp_rom_printf("FPM383 Cancel OK\r\n");
}

//* 验证指纹
Com_Status Int_FPM383_VertyFinger(void)
{
    //* 编写命令
    /*
        参数：最低位为 bit0。
        1) bit0：采图背光灯控制位，0-LED 长亮，1-LED 获取图像成功后灭； -- 1
        2) bit1：采图预处理控制位，0-关闭预处理，1-打开预处理； -- 1
        3) bit2：注册过程中，是否要求模组在关键步骤，返回当前状态，0-要求返回，1-不要求
        返回； -- 1
        4) bit3~bit15：预留。 -- 0
    */
    uint8_t cmds[17] = {
        0xEF, 0x01,             // 包头
        0xff, 0xff, 0xff, 0xff, // 设备地址
        0x01,                   // 包标识
        0x00, 0x08,             // 包长度
        0x32,                   // 指令码
        0x03,                   // 分数等级(灵敏度)
        0xff, 0xff,             // ID号 -- 这样写代表会与指纹库一一对比
        0x00, 0x07,             // 参数
        '\0', '\0'              // 校验和
    };

    //* 计算校验和
    Int_FPM383_CheckSum(cmds,17);

    //* 发送命令
    Int_FPM383_SendCmd(cmds,17);

    //* 接收响应
    Int_FPM383_ReceiveAck(17,2000);

    if(receive_buffers[9] == 0x00)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}

//* 删除单个指纹信息
Com_Status Int_FPM383_DelFinger(int8_t fingerID)
{
    return Com_OK;
}

//* 删除所有指纹信息
Com_Status Int_FPM383_DelAllFinger(void)
{
    return Com_OK;
}
