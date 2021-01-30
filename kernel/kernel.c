#include "../drivers/screen.h"
#include "util.h"

void main() {
    kclear_screen();

    /* Fill up the screen */
    int i = 0;
    for (i = 0; i < 24; i++) {
        char str[255];
        int_to_ascii(i, str);
        kprint_at(str, 0, i);
    }

    kprint("\n");
    kprint("This text forces the kernel to scroll. Row 0 will disappear.\n");
    kprint("And with this text, the kernel will scroll again, and row 1 will disappear too!\n");
}