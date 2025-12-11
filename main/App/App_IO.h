#ifndef __APP_IO_H__
#define __APP_IO_H__
#include "Int_BDR6120.h"
#include "Int_SC12B.h"
#include "Int_WS2812.h"
#include "Int_WTN6170.h"
#include "Int_FPM383.h"
#include "Dri_NVS.h"
#include "Com_Config.h"
#include <string.h>

#define ADMIN "ADMIN"

#define PW_ERR_NUM 3

void App_IO_Init(void);

Com_Status App_IO_GetUserInputContent(uint8_t buffers[]);

//* 密码处理函数
void App_IO_Handler(uint8_t receive_buffers[]);

//* 指纹处理函数
void App_IO_FingerHandler(void);

#endif /* __APP_IO_H__ */
