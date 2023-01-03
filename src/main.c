#include <libdragon.h>
#include <stdio.h>

#include "ed64.h"

int main() {
    int fat = ed64init();

    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);

    gfx_lock();
    char msg[128];
    sprintf(msg, "Hello!\nFAT Initialized: %i", fat);
    gfx_text(10, 10, msg);
    gfx_unlock();
}
