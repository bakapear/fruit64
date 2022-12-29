#include <libdragon.h>

void setTVMode(tv_type_t mode) {
    *(uint32_t *)0x80000300 = mode;
}

int main(void) {
    setTVMode(TV_PAL);

    console_init();
    printf("Hello world!\n");
}
