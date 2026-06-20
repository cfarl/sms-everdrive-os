/*
 * File:   main.c
 * Author: krikzz
 *
 * Created on 23.06.2010
 */


#include "../../lib/edos.h"
extern u32 font_base[];


/*
 * usb-serial sample
 * current application reads data from serial-usb port, print received data on the screen, and send received data back to serial-usb port
 * on PC side anys serial terminal can be used
 * */

int main() {


    edInit();

    VDP_init();
    VDP_loadFont(font_base, 0);
    VDP_setPaletteColor(0, 15, 0xfff); //font color



    u8 str[2];
    str[1] = 0;
    u16 x = 1;
    u16 y = 2;

    VDP_drawText(APLAN, "serial terminal", 0, 1, 1);

    for (;;) {

        str[0] = ed_os->usbReadByte();

        VDP_drawText(APLAN, str, 0, x, y);
        x++;
        if (x == 40) {
            y++;
            x = 1;
        }

        if (y == 27) {
            y = 1;
        }


        ed_os->usbWriteByte(str[0]);
    }

    for (;;);

    return 0;
}

void vb() {
    //vblank interrupt
}

void hb() {
    //hblank interrupt
}

void in() {
    //all other interrupts
}
