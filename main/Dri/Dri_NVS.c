#include "Dri_NVS.h"

nvs_handle_t flash_handle;

//* NVS初始化
void Dri_NVS_Init(void)
{
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        //* flash空间不足

        //* 擦除flash
        nvs_flash_erase();

        //* 重新初始化
        nvs_flash_init();
    }

    //* 在flash空间上申请空间(可写可读)
    esp_err_t result = nvs_open("stroage",NVS_READWRITE,&flash_handle);
    if(result == ESP_OK)
    {
        printf("flash space has been successfully created!\r\n");
    }
    else
    {
        printf("flash space created failed!\r\n");
    }
}

//* NVS写入字符串 -- 用于存储管理员密码
esp_err_t Dri_NVS_SetStr(char *key, char *value)
{
    return nvs_set_str(flash_handle, key, value);
}

//* NVS读取字符串 -- 用于读取管理员密码
esp_err_t Dri_NVS_GetStr(char *key, char *buffer, size_t sizes)
{
    return nvs_get_str(flash_handle,key,buffer,sizes);
}

//* NVS写入数字 -- 用于存储普通用户密码
esp_err_t Dri_NVS_SetU8(char *key, uint8_t *value)
{
    return nvs_set_u8(flash_handle,key,value);
}

//* NVS读取数字 -- 用于读取普通用户密码
esp_err_t Dri_NVS_GetU8(char *key, uint8_t *buffer)
{
    return nvs_get_u8(flash_handle,key,buffer);
}

//* 删除所有密码
esp_err_t Dri_NVS_DelAll(void)
{
    return nvs_erase_all(flash_handle);
}

//* 根据key删除指定密码
esp_err_t Dri_NVS_DelByKey(char *key)
{
    return nvs_erase_key(flash_handle,key);
}

//* 检测密码是否存在
esp_err_t Dri_NVS_KeyIsExist(char *key)
{
    return nvs_find_key(flash_handle,key,NULL);
}
