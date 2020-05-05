#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ws2812_strip.h"

extern "C" {
    
void app_main() {
RGBColor black = RGBColor(0, 0, 0);
//RGBColor white = RGBColor(255, 255, 255);
WS_STRIP strip = WS_STRIP(10);
strip.setColor(black);
while (true) {
    for (int i = 0; i < strip.pixelCount; i++)
{
    for(int j = 0; j < 254; j= j+2) {
        strip.setPixel(i, RGBColor(0, 0, j));
    strip.show();
    vTaskDelay(1);
    }
    strip.setPixel(i, black);
}
}

}

}