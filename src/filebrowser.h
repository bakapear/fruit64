#ifndef _FILEBROWSER_H
#define _FILEBROWSER_H

#include <libdragon.h>
#include <stdint.h>

typedef struct {
    uint32_t type;
    char name[MAX_FILENAME_LEN + 1];
} direntry_t;

void fb_setdir(char* path);
void fb_move(int x, int y);
void fb_select();
void fb_back();
void fb_draw();

#endif
