
#include "filebrowser.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"

char fb_dir[256];
direntry_t *fb_list;
int fb_size, fb_cursor, *fb_cursor_stack, fb_cursor_stack_size;
int fb_page = 18;

void fb_setdir(char *path) {
    strcpy(fb_dir, path);
    fb_size = sd_read(&fb_list, path);
    fb_cursor = 0;
}

void fb_move(int x, int y) {
    int shift = (fb_cursor / fb_page) * fb_page;

    if (x != 0) {
        if ((x < 0 && shift != 0) || (x > 0 && shift <= fb_size - fb_page)) {
            fb_cursor += (x * fb_page);
        }
    }

    if (y != 0) {
        fb_cursor += y;

        if (fb_cursor - shift < 0) fb_cursor += fb_page;
        else if (fb_cursor - shift >= fb_page) fb_cursor -= fb_page;
        else if (fb_cursor > fb_size - 1) fb_cursor -= fb_size % fb_page;
    }

    if (fb_cursor < 0) fb_cursor = 0;
    else if (fb_cursor >= fb_size) fb_cursor = fb_size - 1;
}

void fb_select() {
    direntry_t current = fb_list[fb_cursor];
    if (current.type == DT_DIR) {
        if (fb_dir[strlen(fb_dir) - 1] != '/') strcat(fb_dir, "/");
        strcat(fb_dir, current.name);

        fb_cursor_stack = realloc(fb_cursor_stack, sizeof(int) * (fb_cursor_stack_size + 1));
        fb_cursor_stack[fb_cursor_stack_size++] = fb_cursor;
        fb_setdir(fb_dir);
    }
}

void fb_back() {
    if (strcmp(fb_dir, "/") == 0) return;

    char *last = strrchr(fb_dir, '/');
    if (last - fb_dir == 0) last += 1;
    *last = '\0';

    fb_setdir(fb_dir);
    fb_cursor = fb_cursor_stack[--fb_cursor_stack_size];
    fb_cursor_stack = realloc(fb_cursor_stack, sizeof(int) * (fb_cursor_stack_size + 1));
}

void fb_draw() {
    char buf[36];
    int x = 25, y = 32, yb = 204;

    // background
    gfx_window(x - 4, y - 16, 278, 167, 12, 0xFFFFFFFF, 0x000000AA);
    gfx_window(x - 4, yb - 4, 278, 23, 0, 0xFFFFFFFF, 0x000000AA);

    // header
    gfx_color(0x000000FF, 0);

    sprintf(buf, "SD:/%s", fb_dir);
    strncpy(buf, buf, sizeof(buf) - 1);
    gfx_text(x, 19, buf);

    sprintf(buf, "(%i/%i)", (fb_cursor / fb_page) + 1, (fb_size / fb_page) + 1);
    gfx_text(x + 200 + (-8 * (strlen(buf) - 9)), 19, buf);

    if (fb_size <= 0) {
        gfx_color(0x555555FF, 0);
        gfx_text(x, y, "<EMPTY DIRECTORY>");
    } else {
        // list
        int from = (fb_cursor / fb_page) * fb_page;
        int to = from + fb_page;
        if (to > fb_size) to = fb_size;
        for (int i = from; i < to; i++) {
            if (i == fb_cursor) {
                gfx_window(x - 11 - 4, y - 2, 278 + 11, 10, 0, 0, 0xFFFFFF99);
                gfx_color(fb_list[i].type == DT_DIR ? 0xCC44CCFF : 0xFFFFFFFF, 0);
                gfx_text(x - 12, y, ">");
            } else {
                gfx_color(fb_list[i].type == DT_DIR ? 0x888800FF : 0x888888FF, 0);
            }

            strncpy(buf, fb_list[i].name, sizeof(buf) - 2);
            gfx_text(x, y, buf);
            y += 9;
        }

        // footer
        direntry_t current = fb_list[fb_cursor];

        gfx_color(current.type == DT_DIR ? 0xFFFF00FF : 0xFFFFFFFF, 0);
        strncpy(buf, current.name, sizeof(buf) - 2);
        gfx_text(x, yb, buf);
        if (strlen(current.name) > sizeof(buf) - 2) {
            strncpy(buf, (char *)current.name + sizeof(buf) - 2, sizeof(buf) - 2);
            gfx_text(x, yb + 9, buf);
        }
    }
}