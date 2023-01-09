#include <ed64/everdrive.h>
#include <ed64/ff.h>
#include <ed64/mem.h>
#include <ed64/sd.h>
#include <ed64/sys.h>
#include <libdragon.h>
#include <stdlib.h>

FATFS *fs;

// ED64P
// Title: ED64+ bootloader
// Firmware: 0x110 (272)

void configure() {
    evd_init();
    sdSetInterface(DISK_IFACE_SPI);
    memSpiSetDma(0);
}

int initFS() {
    evd_ulockRegs();
    sleep(10);
    fs = malloc(sizeof(FATFS));
    FRESULT result = f_mount(fs, "", 1);
    return result == FR_OK;
}

int ed64_init() {
    configure();
    return initFS();
}

void ed64_close() {
    f_unmount("");
    free(fs);
}