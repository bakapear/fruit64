#ifndef _ED64_H
#define _ED64_H

typedef enum {
    SD_25MHZ = 0,
    SD_50MHZ = 1
} sd_speed_t;

int initED64();
void closeED64();

#endif
