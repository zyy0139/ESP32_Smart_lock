#ifndef __DRI_NVS_H__
#define __DRI_NVS_H__
#include "nvs_flash.h"
#include <stdio.h>

void Dri_NVS_Init(void);

esp_err_t Dri_NVS_SetStr(char *key,char *value);
esp_err_t Dri_NVS_GetStr(char *key,char *buffer,size_t *sizes);

esp_err_t Dri_NVS_SetU8(char *key,uint8_t *value);
esp_err_t Dri_NVS_GetU8(char *key,uint8_t *buffer);

esp_err_t Dri_NVS_DelAll(void);
esp_err_t Dri_NVS_DelByKey(char *key);

esp_err_t Dri_NVS_KeyIsExist(char *key);

#endif /* __DRI_NVS_H__ */
