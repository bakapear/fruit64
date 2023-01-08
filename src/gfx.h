#ifndef _GFX_H
#define _GFX_H

#include <libdragon.h>

void gfx_lock();
void gfx_unlock();
void gfx_color(uint32_t fc, uint32_t bc);
void gfx_line(short x1, short y1, short x2, short y2, uint32_t c);
void gfx_fill(uint32_t c);
void gfx_sprite(short x, short y, sprite_t* sprite);
void gfx_window(short x, short y, short w, short h, short t, uint32_t c1, uint32_t c2);
void gfx_text(short x, short y, const char* const msg);

#endif