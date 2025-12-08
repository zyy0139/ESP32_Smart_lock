#include "Int_WS2812.h"

uint8_t black[3] = {0, 0, 0};
uint8_t white[3] = {255, 255, 255};
uint8_t red[3] = {0, 255, 0};
uint8_t green[3] = {255, 0, 0};
uint8_t blue[3] = {0, 0, 255};
uint8_t cyan[3] = {255, 0, 255};   /* 青色 */
uint8_t purple[3] = {0, 255, 255}; /* 紫色 */

led_strip_handle_t led_handle;

//* LED模块 -- WS2818 -- 初始化
void Int_WS2812_Init(void)
{
    led_strip_config_t led_config = {
        .strip_gpio_num = WS2812_PIN,                                // 控制引脚
        .max_leds = LED_STRIP_LED_COUNT,                             // led个数
        .led_model = LED_MODEL_WS2812,                               // 型号
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGB, // RGB模式
        .flags = {
            .invert_out = false // 输出信号不需要翻转
        }};

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,                    // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RTS_HZ,             // RMT counter clock frequency
        .mem_block_symbols = LED_STRIP_MEMORY_BLOCK_WORDS, // the memory block size used by the RMT channel
        .flags = {
            .with_dma = LED_STRIP_USE_DMA, // Using DMA can improve performance when driving more LEDs
        }};

    led_strip_new_rmt_device(&led_config, &rmt_config, &led_handle);
}

//* 点亮目标LED
void Int_WS2812_LED_On(Touch_Key keyNumber, uint8_t colors[])
{
    led_strip_set_pixel(led_handle, keyNumber, colors[0], colors[1], colors[2]);
    led_strip_refresh(led_handle);
}

//* 熄灭所以LED
void Int_WS2818_LED_Off_All(void)
{
    led_strip_clear(led_handle);
}
