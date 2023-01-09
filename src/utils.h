#ifndef _UTILS_H
#define _UTILS_H

#include <libdragon.h>

#include "filebrowser.h"

int sd_read(direntry_t **list, char *path);
sprite_t *load_png(uint8_t *png_filename);

#endif
