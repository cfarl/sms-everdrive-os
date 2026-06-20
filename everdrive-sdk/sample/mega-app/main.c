/*
 * File:   main.c
 * Author: krikzz
 *
 * Created on 23.06.2010
 */


#include "../../lib/edos.h"
extern u32 font_base[];

u16 y;

int main() {


    edInit();

    VDP_init();
    VDP_loadFont(font_base, 0);
    VDP_setPaletteColor(0, 15, 0xfff); //font color

    y = 0;

    VDP_drawText(APLAN, "Hello world!", 0, 12, 0);
    for (;;) {

        VDP_waitVSync();
        VDP_setVerticalScroll(APLAN, 0, -y%224);


    }

    return 0;
}

void vb() {
    y++;
    //vblank interrupt
}

void hb() {
    //hblank interrupt
}

void in() {
    //all other interrupts
}
