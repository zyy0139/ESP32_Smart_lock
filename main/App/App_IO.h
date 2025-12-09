#ifndef __APP_IO_H__
#define __APP_IO_H__
#include "Int_BDR6120.h"
#include "Int_SC12B.h"
#include "Int_WS2812.h"
#include "Int_WTN6170.h"
#include "Dri_NVS.h"
#include "Com_Config.h"
#include <string.h>

#define ADMIN "ADMIN"

void App_IO_Init(void);

Com_Status App_IO_GetUserInputContent(uint8_t buffers[]);

void App_IO_Handler(uint8_t receive_buffers[]);

#endif /* __APP_IO_H__ */
