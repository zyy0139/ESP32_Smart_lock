#include <stdio.h>
#include "esp_task.h"
#include "rom/ets_sys.h"
#include "Dri_NVS.h"

void app_main(void)
{
    //* flash测试

    //* 初始化
    Dri_NVS_Init();

    //* 存入管理员密码
    Dri_NVS_SetStr("ADMIN", "123456");

    char buffers[10] = {0};
    size_t len = sizeof(buffers);

    //* 获取管理员密码
    esp_err_t result = Dri_NVS_GetStr("ADMIN", buffers, &len);

    if (result == ESP_OK)
    {
        printf("admin ok : %s\r\n", buffers);
    }
}
