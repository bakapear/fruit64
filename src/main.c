#include <libdragon.h>
#include <stdio.h>

#include "ed64.h"

int main() {
    int fat = ed64init();

    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    display_context_t disp;
    while (!(disp = display_lock())) {}
    char msg[128];
    sprintf(msg, "Hello!\nFAT Initialized: %i", fat);
    graphics_draw_text(disp, 15, 15, msg);
    display_show(disp);
}
