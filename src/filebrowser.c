#include "filebrowser.h"

#include <ed64/ff.h>
#include <stdlib.h>
#include <string.h>

char fb_dir[256];
direntry_t *fb_list;
int fb_size, fb_cursor, fb_shift, fb_cursor_last, fb_shift_last;
int fb_page = 18;

int sort_compare(const void *a, const void *b) {
    direntry_t *A = (direntry_t *)a;
    direntry_t *B = (direntry_t *)b;

    if (A->type == DT_DIR && B->type != DT_DIR) return -1;
    if (A->type != DT_DIR && B->type == DT_DIR) return 1;
    return strcmp(A->name, B->name);
}

int sd_read(direntry_t **list, char *path) {
    free(*list);
    *list = malloc(sizeof(direntry_t));

    DIR dp;
    FILINFO info;
    FRESULT res;

    if (f_opendir(&dp, path) != FR_OK) return -1;  // dir not found

    int count = 0;

    while (1) {
        res = f_readdir(&dp, &info);
        if (res != FR_OK || info.fname[0] == 0) break;  // end of dir

        *list = realloc(*list, sizeof(direntry_t) * (count + 1));

        if (info.fattrib & AM_DIR) {
            (*list)[count].type = DT_DIR;
        } else {
            (*list)[count].type = DT_REG;
        }

        strcpy((*list)[count].name, info.fname);

        count++;
    }

    f_closedir(&dp);

    qsort(*list, count, sizeof(direntry_t), sort_compare);

    return count;
}

void fb_setdir(char *path) {
    strcpy(fb_dir, path);
    fb_size = sd_read(&fb_list, path);
    fb_cursor = fb_shift = 0;
}

void fb_move(int x, int y) {
    if (x == 0) {
        fb_cursor += (y > 0 ? -1 : 1);
        if (fb_cursor - fb_shift >= fb_page) fb_cursor -= fb_page;
        else if (fb_cursor > fb_size - 1) fb_cursor -= fb_size % fb_page;
        else if (fb_cursor - fb_shift < 0) fb_cursor += fb_page;
    } else {
        if (x < 0 && fb_shift != 0) fb_cursor -= fb_page;
        else if (x > 0 && fb_shift <= fb_size - fb_page) fb_cursor += fb_page;
    }

    if (fb_cursor < 0) fb_cursor = 0;
    if (fb_cursor > fb_size - 1) fb_cursor = fb_size - 1;
    if (fb_cursor - fb_shift >= fb_page) fb_shift += fb_page;
    if (fb_cursor - fb_shift < 0) fb_shift -= fb_page;
}

void fb_select() {
    direntry_t current = fb_list[fb_cursor];
    if (current.type == DT_DIR) {
        if (fb_dir[strlen(fb_dir) - 1] != '/') strcat(fb_dir, "/");
        strcat(fb_dir, current.name);

        fb_cursor_last = fb_cursor;
        fb_shift_last = fb_shift;
        fb_setdir(fb_dir);
    }
}

void fb_back() {
    if (strcmp(fb_dir, "/") == 0) return;

    char *last = strrchr(fb_dir, '/');
    if (last - fb_dir == 0) last += 1;
    *last = '\0';

    fb_setdir(fb_dir);
    fb_cursor = fb_cursor_last;
    fb_shift = fb_shift_last;
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
        int max = fb_page + fb_shift;
        if (max > fb_size) max = fb_size;
        for (int i = fb_shift; i < max; i++) {
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