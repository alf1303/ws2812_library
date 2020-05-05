#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/rmt.h>

#define RMT_CHAN RMT_CHANNEL_0
#define PIN GPIO_NUM_21
#define MYDIV 2

#define wsT0_H 300 // nanoseconds
#define wsT0_L 790
#define wsT1_H 790
#define wsT1_L 320
#define ws_RST 350 //microseconds

struct RGBColor {
    RGBColor(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    };
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class WS_STRIP {
    uint8_t* data;

    void initRMT();
    void initRMT_blocks();
    

    public:
    int pixelCount;
    WS_STRIP(int count) {
        pixelCount = count;
        data = (uint8_t*) calloc(count * 3, 1);
        initRMT();
        initRMT_blocks();
    }
    void setColor(RGBColor color);
    void setPixel(int pixel_num, RGBColor color);
    void show();
};
