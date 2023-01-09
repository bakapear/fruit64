#include <ed64/ff.h>
#include <libdragon.h>
#include <stdlib.h>

#include "filebrowser.h"

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

sprite_t *load_png(uint8_t *png_filename) {
    TCHAR *filename = (TCHAR *)malloc(strlen(png_filename));
    sprintf(filename, "%s", png_filename);

    FRESULT result;
    FIL file;
    UINT bytesread;
    result = f_open(&file, filename, FA_READ);

    if (result == FR_OK) {
        int fsize = f_size(&file);
        uint8_t png_rawdata[fsize];

        result =
            f_read(
                &file,       /* [IN] File object */
                png_rawdata, /* [OUT] Buffer to store read data */
                fsize,       /* [IN] Number of bytes to read */
                &bytesread   /* [OUT] Number of bytes read */
            );

        f_close(&file);

        free(filename);
        return loadImage32(png_rawdata, fsize);
    }

    return 0;
}
