#include <stdio.h>
#include "esp_task.h"
#include "rom/ets_sys.h"
#include "Dri_NVS.h"

void app_main(void)
{
    //* flash测试

    //* 初始化
    Dri_NVS_Init();

    Dri_NVS_SetStr("ADMIN","123456");
    
}
