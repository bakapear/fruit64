#ifndef _GFX_H
#define _GFX_H

#include <libdragon.h>

void gfx_lock();
void gfx_unlock();
uint32_t gfx_color(short r, short b, short g, short a);
void gfx_line(short x1, short y1, short x2, short y2, uint32_t c);
sprite_t* gfx_load(const char* const path);
void gfx_sprite(short x, short y, sprite_t* sprite);
void gfx_window(short x, short y, short w, short h, short t, uint32_t c1, uint32_t c2);
void gfx_text(short x, short y, const char* const msg);

#endif