#include <stdio.h>
#include "esp_task.h"
#include "rom/ets_sys.h"
#include "Int_BDR6120.h"
#include "Int_WTN6170.h"
#include "Int_SC12B.h"
#include "Int_WS2812.h"

void app_main(void)
{
    //* 按键测试
    //* 初始化
    Int_SC12B_Init();
    Int_WTN6170_Init();
    Int_WS2812_Init();

    while (1)
    {
        Touch_Key key_num = Int_SC12B_GetKeyNum();
        if (key_num != KEY_NO)
        {
            printf("%d\r\n", key_num);
            sayWaterDrop();
            Int_WS2818_LED_Off_All();
            Int_WS2812_LED_On(key_num, cyan);
            vTaskDelay(300);
            Int_WS2818_LED_Off_All();
        }
        vTaskDelay(50);
    }
}
