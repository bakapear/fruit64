#include <libdragon.h>

display_context_t disp;

void gfx_lock() {
    while (!(disp = display_lock())) {}
}

void gfx_unlock() {
    if (disp) display_show(disp);
}

void gfx_color(uint32_t fc, uint32_t bc) {
    return graphics_set_color(fc, bc);
}

void gfx_fill(uint32_t c) {
    graphics_fill_screen(disp, c);
}

void gfx_line(short x1, short y1, short x2, short y2, uint32_t c) {
    graphics_draw_line_trans(disp, x1, y1, x2, y2, c);
}

void gfx_sprite(short x, short y, sprite_t *sprite) {
    graphics_draw_sprite(disp, x, y, sprite);
}

void gfx_window(short x, short y, short w, short h, short t, uint32_t c1, uint32_t c2) {
    y += t;
    if (c2 != 0) graphics_draw_box_trans(disp, x + 1, y + 1, w - 1, h - 1, c2);  // bg
    if (c1 != 0) {
        graphics_draw_box_trans(disp, x, y - t, w, t + 1, c1);           // top
        graphics_draw_line_trans(disp, x, y, x, h + y, c1);              // left
        graphics_draw_line_trans(disp, w + x, y - t, w + x, h + y, c1);  // right
        graphics_draw_line_trans(disp, x, h + y, w + x, h + y, c1);      // bottom
    }
}

void gfx_text(short x, short y, const char *const msg) {
    graphics_draw_text(disp, x, y, msg);
}
