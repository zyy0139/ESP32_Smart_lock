#ifndef __INT_WS2812_H__
#define __INT_WS2812_H__
#include "driver/gpio.h"
#include "Int_SC12B.h"
#include "led_strip.h"
#define WS2812_PIN GPIO_NUM_6
#define LED_STRIP_LED_COUNT 12
#define LED_STRIP_RMT_RTS_HZ (10 * 1000 * 1000)
#define LED_STRIP_MEMORY_BLOCK_WORDS 0
#define LED_STRIP_USE_DMA 0

// 对外暴露
extern uint8_t black[3];
extern uint8_t white[3];
extern uint8_t red[3];
extern uint8_t green[3];
extern uint8_t blue[3];
extern uint8_t cyan[3];   /* 青色 */
extern uint8_t purple[3]; /* 紫色 */

void Int_WS2812_Init(void);

void Int_WS2812_LED_On(Touch_Key keyNumber, uint8_t colors[]);

void Int_WS2818_LED_Off_All(void);

#endif /* __INT_WS2812_H__ */
