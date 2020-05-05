#include "../include/ws2812_strip.h"

rmt_item32_t high_pulse;
rmt_item32_t low_pulse;
uint32_t rst_ticks = 0;
rmt_config_t rmtConfig;

void translator_func(const void* src, rmt_item32_t* dst, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num) {
    if(src == NULL || dst == NULL)  {
        *translated_size = 0;
        *item_num = 0;
        return;
    }

    size_t size_el = 0;
    size_t num = 0;
    rmt_item32_t *pdst = dst;
    uint8_t *psrc = (uint8_t*) src;

    while ((size_el < src_size) && (num < wanted_num)) {
        for (int i = 0; i < 8; i++) {
            if (*psrc & (1 << (7-i)))
            {
                *pdst = high_pulse;
            }
            else {
                *pdst = low_pulse;
            }
            num++;
            pdst++;
        }
        size_el++;
        psrc++;
    }
    pdst--;
    pdst->duration1 = pdst->duration1 + rst_ticks;
    *translated_size = size_el;
    *item_num = num;
}

void WS_STRIP::setPixel(int pixel_num, RGBColor color) {
    data[pixel_num*3] = color.g;
    data[pixel_num*3 + 1] = color.r;
    data[pixel_num*3 + 2] = color.b;
}

void WS_STRIP::setColor(RGBColor color) {
    for(int i = 0; i < pixelCount; i++) {
        setPixel(i, color);
    }
}

void WS_STRIP::initRMT() {
    rmtConfig.rmt_mode = RMT_MODE_TX;
    rmtConfig.channel = RMT_CHAN;
    rmtConfig.gpio_num = PIN;
    rmtConfig.clk_div = MYDIV;
    rmtConfig.mem_block_num = (pixelCount*3*8)/64 + 1;
    rmt_config(&rmtConfig);
    rmt_driver_install(RMT_CHAN, 0, 0);
    rmt_translator_init(RMT_CHAN, translator_func);
}

void WS_STRIP::initRMT_blocks() {
    int rate = 80/MYDIV;
    uint32_t t0h_ticks = (int)((rate*wsT0_H)/1000);
    uint32_t t0l_ticks = (int)((rate*wsT0_L)/1000);
    uint32_t t1h_ticks = (int)((rate*wsT1_H)/1000);
    uint32_t t1l_ticks = (int)((rate*wsT1_L)/1000);
    //printf("%d\n", t0h_ticks);
    rst_ticks = (int)((rate*ws_RST));
    high_pulse = {{{t1h_ticks, 1, t1l_ticks, 0}}};
    low_pulse = {{{t0h_ticks, 1, t0l_ticks, 0}}};
}

void WS_STRIP::show() {
    //printf("show\n");
    rmt_write_sample(RMT_CHAN, data, pixelCount*3, 0);
}