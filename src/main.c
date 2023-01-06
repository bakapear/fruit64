#include <libdragon.h>
#include <stdio.h>

#include "ed64.h"
#include "gfx.h"

void setTVMode(tv_type_t mode) {
    *(uint32_t *)0x80000300 = mode;
}

void setSDSpeed(sd_speed_t speed) {
    switch (speed) {
        case SD_25MHZ: return bi_speed25();
        case SD_50MHZ: return bi_speed50();
    }
}

int main() {
    int fat = initED64();

    setTVMode(TV_PAL);
    setSDSpeed(SD_50MHZ);
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);

    gfx_lock();
    char msg[128];
    sprintf(msg, "Hello!\nFAT Initialized: %i", fat);
    gfx_text(10, 10, msg);
    gfx_unlock();
}
