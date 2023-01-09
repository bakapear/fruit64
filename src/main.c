#include <libdragon.h>
#include <stdio.h>

#include "ed64.h"
#include "gfx.h"
#include "utils.h"

void setTVMode(tv_type_t mode) {
    *(uint32_t *)0x80000300 = mode;
}

void setSDSpeed(sd_speed_t speed) {
    switch (speed) {
        case SD_25MHZ: bi_speed25(); break;
        case SD_50MHZ: bi_speed50(); break;
    }
}

sprite_t *spr_bg;

void setBackground(const char *path) {
    spr_bg = load_png(path);
}

int main() {
    ed64_init();

    setTVMode(TV_PAL);
    setSDSpeed(SD_50MHZ);

    controller_init();
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);

    fb_setdir("/");

    setBackground("background.png");

    while (1) {
        gfx_lock();
        spr_bg ? gfx_sprite(0, 0, spr_bg) : gfx_fill(0xFFAA55FF);
        fb_draw();
        gfx_unlock();

        while (1) {
            controller_scan();
            struct SI_condat pressed = get_keys_pressed().c[0];  // continous
            struct SI_condat holding = get_keys_down().c[0];     // once

            if (pressed.up) fb_move(0, 1);
            else if (pressed.down) fb_move(0, -1);
            else if (pressed.left) fb_move(-1, 0);
            else if (pressed.right) fb_move(1, 0);
            else if (holding.A) fb_select();
            else if (holding.B) fb_back();
            else continue;
            break;
        }
    }
}