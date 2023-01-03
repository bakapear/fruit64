#include <libdragon.h>
#include <malloc.h>

display_context_t disp;

void gfx_lock() {
    while (!(disp = display_lock())) {}
}

void gfx_unlock() {
    if (disp) display_show(disp);
}

uint32_t gfx_color(short r, short b, short g, short a) {
    return graphics_make_color(r, g, b, a);
}

void gfx_line(short x1, short y1, short x2, short y2, uint32_t c) {
    graphics_draw_line_trans(disp, x1, y1, x2, y2, c);
}

sprite_t *gfx_load(const char *const path) {
    int fp = dfs_open(path);
    sprite_t *sprite = malloc(dfs_size(fp));
    dfs_read(sprite, 1, dfs_size(fp), fp);
    dfs_close(fp);
    return sprite;
}

void gfx_sprite(short x, short y, sprite_t *sprite) {
    graphics_draw_sprite(disp, x, y, sprite);
}

void gfx_window(short x, short y, short w, short h, short t, uint32_t c1, uint32_t c2) {
    y += t;
    graphics_draw_box_trans(disp, x + 1, y + 1, w - 1, h - 1, c2);  // bg
    graphics_draw_box(disp, x, y - t, w, t + 1, c1);                // top
    graphics_draw_line(disp, x, y, x, h + y, c1);                   // left
    graphics_draw_line(disp, w + x, y - t, w + x, h + y, c1);       // right
    graphics_draw_line(disp, x, h + y, w + x, h + y, c1);           // bottom
}

void gfx_text(short x, short y, const char *const msg) {
    graphics_draw_text(disp, x, y, msg);
}