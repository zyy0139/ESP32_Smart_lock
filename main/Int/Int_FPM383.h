#ifndef __INT_FPM383_H__
#define __INT_FPM383_H__
#include "rom/ets_sys.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include <string.h>

#define FPM383_TX_PIN GPIO_NUM_21
#define FPM383_RX_PIN GPIO_NUM_20
#define FPM383_TOUCH_OUT GPIO_NUM_10

void Int_FPM383_Init(void);

void Int_FPM383_Sleep(void);

void Int_FPM383_GetChipID(void);

#endif /* __INT_FPM383_H__ */
