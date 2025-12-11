#ifndef __INT_FPM383_H__
#define __INT_FPM383_H__
#include "rom/ets_sys.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "Com_Config.h"
#include <string.h>

#define FPM383_TX_PIN GPIO_NUM_21
#define FPM383_RX_PIN GPIO_NUM_20
#define FPM383_TOUCH_OUT GPIO_NUM_10

void Int_FPM383_Init(void);

void Int_FPM383_Sleep(void);

void Int_FPM383_GetChipID(void);

//* 获取最近空闲索引表页码
uint16_t Int_FPM383_GetMinID(void);

//* 录入指纹
Com_Status Int_FPM383_RegisterFinger(uint16_t minID);

//* 取消录入
void Int_FPM383_Cancel(void);

//* 验证指纹
Com_Status Int_FPM383_VertyFinger(void);

//* 获取要删除的指纹ID
int8_t Int_FPM383_GetDelFingerID(void);

//* 删除单个指纹信息
Com_Status Int_FPM383_DelFinger(int8_t fingerID);

//* 删除所有指纹信息
Com_Status Int_FPM383_DelAllFinger(void);

#endif /* __INT_FPM383_H__ */
