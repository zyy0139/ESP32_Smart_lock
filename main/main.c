#include <stdio.h>
#include "esp_task.h"
#include "App_IO.h"
#include "Com_Config.h"
#include "Int_FPM383.h"
#include <string.h>

//* 按键任务相关
TaskHandle_t SC12B_Task_Handle;
void sc12b_handle(void *param);

//* 指纹任务相关
TaskHandle_t Finger_Task_Handle;
void finger_handle(void *param);

void app_main(void)
{
    //* 模块初始化
    App_IO_Init();

    //* 创建任务
    xTaskCreate(sc12b_handle, "sc12b_handle", 4096, NULL, 4, &SC12B_Task_Handle);
    xTaskCreate(finger_handle, "finger_handle", 2048, NULL, 4, &Finger_Task_Handle);
}

void sc12b_handle(void *param)
{
    uint8_t receive_info_buffers[50] = {0};

    while (1)
    {
        Com_Status status = App_IO_GetUserInputContent(receive_info_buffers);
        switch (status)
        {
        case Com_ERROR:
            //* 输入M语音提示
            sayWithoutInt();
            sayIllegalOperation();
            break;
        case Com_OK:
            //* 获取用户输入内容,并处理
            App_IO_Handler(receive_info_buffers);
            break;
        case Com_TIMEOUT:
            //* 超时处理
            //* 关闭所有led
            Int_WS2818_LED_Off_All();
            break;
        default:
            break;
        }

        //* 清除缓冲数组
        memset(receive_info_buffers, 0, 50);
        vTaskDelay(50);
    }
}

void finger_handle(void *param)
{
    while (1)
    {
        App_IO_FingerHandler();
        vTaskDelay(50);
    }
}
