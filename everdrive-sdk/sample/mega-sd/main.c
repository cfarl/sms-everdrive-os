/*
 * File:   main.c
 * Author: krikzz
 *
 * Created on 23.06.2010
 */


#include "../../lib/edos.h"

u16 strContain(u8 *target, u8 *str);
u16 strLen(u8 *str);

extern u32 font_base[];

/*
 * sd card sample
 * current application finds .txt file in the root of sd card, then open, read and print text file
 * */

int main() {



    u8 buff[512];
    u16 i = 0;
    u8 resp;
    FatFullRecord rec;

    edInit();

    VDP_init();
    VDP_loadFont(font_base, 0);
    VDP_setPaletteColor(0, 15, 0xfff); //font color




    ed_os->fatOpenDir(0);//open root dir


    for (;;) {

        resp = ed_os->fatReadDir(&rec);

        if (rec.entry_cluster == 0 || resp != 0) {

            if (resp != 0) {
                VDP_drawText(APLAN, "ERROR", 0, 1, 1);
            } else {
                VDP_drawText(APLAN, "*.txt file not found", 0, 1, 1);
            }

            for (;;);
        }


        if (strContain(".txt", rec.name))break;
    }




    resp = ed_os->fatOpenFile(&rec, 0);
    if (resp != 0) {
        VDP_drawText(APLAN, "ERROR", 0, 1, 1);
        for (;;);
    }

    resp = ed_os->fatReadFile(buff, 1); //read one sectors (512 bytes)
    if (resp != 0) {
        VDP_drawText(APLAN, "ERROR", 0, 1, 1);
        for (;;);
    }

    //print file name
    VDP_drawText(APLAN, rec.name, 0, 1, 1);

    //print file data
    for (i = 0; i < 512 / 40; i++) {

        VDP_drawTextML(APLAN, &buff[i * 40], 0, 1, i + 4, 40);
    }

    for (;;);

    return 0;
}

u16 strLen(u8 *str) {

    u16 i;
    for (i = 0; *str != 0; i++)*str++;
    return i;
}

u16 strContain(u8 *target, u8 *str) {

    u16 targ_len = strLen(target);
    u16 eq_len;


    for (eq_len = 0; eq_len < targ_len;) {

        if (*str == 0)return 0;
        if (*str++ == target[eq_len]) {
            eq_len++;
        } else {
            eq_len = 0;
        }
    }

    if (eq_len != targ_len)return 0;
    return 1;

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
