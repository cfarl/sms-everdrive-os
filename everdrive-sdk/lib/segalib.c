
#include "segalib.h"
#include "edos.h"

volatile u16 joy;

u16 vdp_plan_w;
u16 vdp_plan_h;


const u8 vdp_init_reg_valuses[] = {

    0x04, /* reg. 0 - Enable HBL */
    0x44, /* reg. 1 - Enable display, VBL, DMA + VCell size */
    0x30, /* reg. 2 - Plane A =$30*$400=$C000 */
    0x2C, /* reg. 3 - Window  =$2C*$400=$B000 */
    0x07, /* reg. 4 - Plane B =$7*$2000=$E000 */
    0x5E, /* reg. 5 - sprite table begins at $BC00=$5E*$200 */
    0x00, /* reg. 6 - not used */
    0x00, /* reg. 7 - Background Color number*/
    0x00, /* reg. 8 - not used */
    0x00, /* reg. 9 - not used */
    0x01, /* reg 10 - HInterrupt timing */
    0x00, /* reg 11 - $0000abcd a=extr.int b=vscr cd=hscr */
    0x81, /* reg 12 - hcell mode + shadow/highight + interlaced mode (40 cell, no shadow, no interlace)*/
    0x2E, /* reg 13 - HScroll Table = $B800 */
    0x00, /* reg 14 - not used */
    0x02, /* reg 15 - auto increment data */
    0x11, /* reg 16 - scrl screen v&h size (64x64) */
    0x00, /* reg 17 - window hpos */
    0xFF, /* reg 18 - window vpos */

};


u8 vdp_reg0c_st;
u8 vdp_reg1_st;

/*
extern u16 vdp_exec_code[];
u32 vdp_dma_addr;
asm("vdp_exec_code:");
asm("move.w vdp_dma_addr, 0xC00004");
asm("move.w vdp_dma_addr+2, 0xC00004");
asm("rts");
asm("");

u16 vdp_exec[16];
 */
u16 VDP_vinIsEnabled() {
    return vdp_reg1_st & (1 << 5);
}

void VDP_vintOff() {
    vdp_reg1_st &= ~(1 << 5);
    //*((volatile u16 *) GFX_CTRL_PORT) = 0x8100 | vdp_reg1_st;
    ed_os->VDP_vintOff();
}

void VDP_vintOn() {
    vdp_reg1_st |= (1 << 5);
    ed_os->VDP_vintOn();
    //*((volatile u16 *) GFX_CTRL_PORT) = 0x8100 | vdp_reg1_st;
}

void VDP_init() {

    u16 i;
    vdp_reg0c_st = vdp_init_reg_valuses[0x0c];


    VDP_setReg(1, 0x4);
    VDP_setAutoInc(2);
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(0);

    asm("movem.l %d0-%d1/%a0, -(%sp)");
    asm("move.w #16383, %d0");
    asm("move.w #0, %d1");
    asm("move.l #0xC00000, %a0");
    asm("vdp_clr:");
    asm("move.w %d1, (%a0)");
    asm("move.w %d1, (%a0)");
    asm("move.w %d1, (%a0)");
    asm("move.w %d1, (%a0)");
    asm("dbra %d0, vdp_clr");
    asm("movem.l (%sp)+, %d0-%d1/%a0");
    //for (i = 0; i < 32768; i++)*((volatile u16 *) GFX_DATA_PORT) = 0;
    //for (i = 0; i < 32768; i++)*((volatile u16 *) GFX_DATA_PORT) = 0;


    for (i = 0x00; i < 0x13; i++) *((volatile u16 *) GFX_CTRL_PORT) = 0x8000 | (i << 8) | vdp_init_reg_valuses[i];
    //    VDP_setReg(1, 0x44);
    //VDP_setReg(0x0c, vdp_reg0c_st);
    for (i = 0; i < 8; i++)VDP_waitVSync();
    VDP_setReg(1, 0x64);
    //VDP_setReg(1, 0x4);
    vdp_plan_w = ((vdp_init_reg_valuses[0x10] & 0xF) + 1) << 5;
    vdp_plan_h = ((vdp_init_reg_valuses[0x10] >> 4) + 1) << 5;

    VDP_setVerticalScroll(APLAN, 0, 0);
    VDP_setHorizontalScroll(APLAN, 0, 0);
    VDP_setVerticalScroll(BPLAN, 0, 0);
    VDP_setHorizontalScroll(BPLAN, 0, 0);

    vdp_reg1_st = 0x64;
    //for (i = 0; i < 16; i++)vdp_exec[i] = vdp_exec_code[i];


}

void VDP_setReg(u8 reg, u8 value) {

    if (reg == 1) {
        // VDP_waitDMACompletion();
        vdp_reg1_st = value & ~0x10;
    }

    //*((volatile u16 *) GFX_CTRL_PORT) = 0x8000 | (reg << 8) | value;
    ed_os->VDP_setReg(reg, value); 

    if (reg == 0x10) {
        vdp_plan_w = ((value & 0xF) + 1) << 5;
        vdp_plan_h = ((value >> 4) + 1) << 5;
    }
    // if (reg == 1)

}

void VDP_setWidth256() {

    //reg 0x0c
    vdp_reg0c_st &= ~0x81;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x8C00 | vdp_reg0c_st;
}

void VDP_setWidth320() {

    //reg 0x0c
    vdp_reg0c_st |= ~0x81;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x8C00 | vdp_reg0c_st;
}

void VDP_setPlanSize(u16 w, u16 h) {

    u16 val = (((h >> 5) - 1) << 4) | (((w >> 5) - 1) << 0);
    VDP_setReg(0x10, val);

}

void VDP_setAutoInc(u8 value) {

    //reg 0x0f
    *((volatile u16 *) GFX_CTRL_PORT) = 0x8F00 | value;
}

void VDP_setHInterrupt(u8 state) {

    if (state) {
        state = 0x14;
    } else {
        state = 0x04;
    }
    //reg0x00
    *((volatile u16 *) GFX_CTRL_PORT) = 0x8000 | state;
}

void VDP_setHilightShadow(u8 value) {

    if (value) {
        vdp_reg0c_st |= 0x08;
    } else {
        vdp_reg0c_st &= ~0x08;
    }
    //reg 0x0c
    *((volatile u16 *) GFX_CTRL_PORT) = 0x8C00 | vdp_reg0c_st;
}

void VDP_waitVSync() {

    u16 vdp_state;
    vdp_state = VDP_VBLANK_FLAG;
    while (vdp_state & VDP_VBLANK_FLAG) vdp_state = *((volatile u16 *) GFX_CTRL_PORT);
    while (!(vdp_state & VDP_VBLANK_FLAG)) vdp_state = *((volatile u16 *) GFX_CTRL_PORT);
}

void VDP_setHorizontalScroll(u16 plan, u16 line, u16 value) {

    u16 addr;
    addr = HSCRL + ((line & 0xFF) * 4);
    if (plan == BPLAN) addr += 2;

    *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);
    *((volatile u16 *) GFX_DATA_PORT) = value;
}

void VDP_setVerticalScroll(u16 plan, u16 cell, u16 value) {

    u16 addr;
    addr = (cell & 0x1F) << 2;
    if (plan == BPLAN) addr += 2;

    *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VSRAM_ADDR(addr);
    *((volatile u16 *) GFX_DATA_PORT) = value;
}

void VDP_setScrollMode(u8 vs, u8 hs) {
    VDP_setReg(11, ((vs & 1) << 2) | (hs & 3));
}

void VDP_readVram(u16 src, u16 *dst, u16 len) {

    len >>= 1;

    VDP_setAutoInc(2);
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_READ_VRAM_ADDR(src);
    while (len--)*dst++ = *((u16 *) GFX_DATA_PORT);
}

void VDP_readCram(u16 src, u16 *dst, u16 len) {

    len >>= 1;
    VDP_setAutoInc(2);
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_READ_CRAM_ADDR(src);
    while (len--)*dst++ = *((u16 *) GFX_DATA_PORT);
}

void VDP_readVsram(u16 src, u16 *dst, u16 len) {

    len >>= 1;
    VDP_setAutoInc(2);
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_READ_VSRAM_ADDR(src);
    while (len--)*dst++ = *((u16 *) GFX_DATA_PORT);
}

void VDP_waitFIFOEmpty() {
    while (!GETVDPSTATUS(VDP_FIFOEMPTY_FLAG));
}


//****************************************************************************** pal area

void VDP_setPaletteColor(u16 numpal, u16 numcol, u16 value) {

    u16 addr;
    addr = (numpal * 32) + (numcol * 2);
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_CRAM_ADDR(addr);
    *((volatile u16 *) GFX_DATA_PORT) = value;
}

void VDP_setPalette(u16 num, void *pal) {

    VDP_DoDMA(VDP_DMA_CRAM, pal, num << 5, 32);

}

void VDP_getPalette(u16 num, u16 *pal) {

    u16 *dest;
    u16 i;
    u16 addr;

    VDP_setAutoInc(2);
    dest = pal;
    addr = num * 32;
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_READ_CRAM_ADDR(addr);

    i = 16;
    while (i--) *dest++ = *((volatile u16 *) GFX_DATA_PORT);
}

u16 VDP_getPaletteColor(u16 numpal, u16 numcol) {

    u16 addr;

    addr = (numpal * 32) + (numcol * 2);
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_READ_CRAM_ADDR(addr);

    return *((volatile u16 *) GFX_DATA_PORT);
}

//****************************************************************************** DMA area

void VDP_DoDMA(u8 area, void *from, u16 to, u16 len) {

    u32 addr = (u32) from;

    VDP_setReg(1, vdp_reg1_st | 0x10);

    VDP_setAutoInc(2);
    /* Setup DMA length (in word here) */
    len >>= 1;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9300 + (len & 0xff);
    len >>= 8;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9400 + (len & 0xff);

    /* Setup DMA address */
    addr >>= 1;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9500 + (addr & 0xff);
    addr >>= 8;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9600 + (addr & 0xff);
    addr >>= 8;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9700 + (addr & 0xff);


    /* Enable DMA */


    switch (area) {
        case VDP_DMA_VRAM:
            *((volatile u32 *) GFX_CTRL_PORT) = GFX_DMA_VRAM_ADDR(to);
            //vdp_dma_addr = GFX_DMA_VRAM_ADDR(to);
            //asm("jsr vdp_exec");
            break;

        case VDP_DMA_CRAM:
            *((volatile u32 *) GFX_CTRL_PORT) = GFX_DMA_CRAM_ADDR(to);
            //vdp_dma_addr = GFX_DMA_CRAM_ADDR(to);
            //asm("jsr vdp_exec");
            break;

        case VDP_DMA_VSRAM:
            *((volatile u32 *) GFX_CTRL_PORT) = GFX_DMA_VSRAM_ADDR(to);
            //vdp_dma_addr = GFX_DMA_VSRAM_ADDR(to);
            //asm("jsr vdp_exec");
            break;
    }

    VDP_waitDMACompletion();
    VDP_setReg(1, vdp_reg1_st);

}

void VDP_DoDMAFill(u16 to, u16 len, u16 value) {

    VDP_setReg(1, vdp_reg1_st | 0x10);

    to -= 1;
    /* Setup DMA length */
    //VDP_setReg(1, 0x74);
    VDP_setAutoInc(1);

    *((volatile u16 *) GFX_CTRL_PORT) = 0x9300 + (len & 0xff);
    len >>= 8;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9400 + (len & 0xff);

    /* Setup DMA operation (VRAM FILL) */
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9780;



    /* Enable DMA */
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_DMA_VRAM_ADDR(to);


    /* set up value to fill */
    *((volatile u16 *) GFX_DATA_PORT) = value << 8;

    VDP_waitDMACompletion();
    VDP_setReg(1, vdp_reg1_st);
}

void VDP_DoDMACopy(u8 area, u16 from, u16 to, u16 len) {

    VDP_setReg(1, vdp_reg1_st | 0x10);
    /* Setup DMA length */
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9300 + (len & 0xff);
    len >>= 8;
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9400 + (len & 0xff);

    /* Setup DMA address */
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9500 + (from & 0xff);
    *((volatile u16 *) GFX_CTRL_PORT) = 0x9600 + ((from >> 8) & 0xff);

    /* Setup DMA operation (VRAM COPY) */
    *((volatile u16 *) GFX_CTRL_PORT) = 0x97C0;

    /* Enable DMA */
    switch (area) {
        case VDP_DMA_VRAM:
            VDP_setAutoInc(1);
            *((volatile u32 *) GFX_CTRL_PORT) = GFX_DMA_VRAM_ADDR(to);
            //*vdp_dma_exec_addr = GFX_DMA_VRAM_ADDR(to);
            //asm("jsr vdp_dma_exec_module");
            break;

        case VDP_DMA_CRAM:
            VDP_setAutoInc(2);
            *((volatile u32 *) GFX_CTRL_PORT) = GFX_DMA_CRAM_ADDR(to);
            //*vdp_dma_exec_addr = GFX_DMA_CRAM_ADDR(to);
            //asm("jsr vdp_dma_exec_module");
            break;

        case VDP_DMA_VSRAM:
            VDP_setAutoInc(2);
            *((volatile u32 *) GFX_CTRL_PORT) = GFX_DMA_VSRAM_ADDR(to);
            //*vdp_dma_exec_addr = GFX_DMA_VSRAM_ADDR(to);
            //asm("jsr vdp_dma_exec_module");
            break;
    }

    VDP_waitDMACompletion();
    VDP_setReg(1, vdp_reg1_st);
}

u16 VDP_dmaIsComplete() {

    return GETVDPSTATUS(VDP_DMABUSY_FLAG) == 0;
}

void VDP_waitDMACompletion() {

    while (GETVDPSTATUS(VDP_DMABUSY_FLAG));
}

void VDP_fillTileRectInc(u16 plan, u16 basetile, u16 x, u16 y, u16 w, u16 h) {


    u32 addr;
    u32 planwidth;
    u16 tile;
    u16 i, j;

    VDP_setAutoInc(2);


    planwidth = vdp_plan_w;
    addr = plan + ((x + (planwidth * y)) << 1);
    tile = basetile;

    i = h;
    while (i--) {

        *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);
        j = w;
        while (j--) *((volatile u16 *) GFX_DATA_PORT) = tile++;
        addr += planwidth << 1;
    }
}

void VDP_fillTileRect(u16 plan, u16 tile, u16 x, u16 y, u16 w, u16 h) {


    u32 addr;
    u32 planwidth;
    u16 i, j;

    VDP_setAutoInc(2);

    planwidth = vdp_plan_w;
    addr = plan + ((x + (planwidth * y)) << 1);

    i = h;
    while (i--) {

        *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);
        j = w;
        while (j--) *((volatile u16 *) GFX_DATA_PORT) = tile;
        addr += planwidth << 1;
    }
}

void VDP_drawText(u16 plan, u8 *str, u16 basetile, u16 x, u16 y) {

    VDP_drawTextML(plan, str, basetile, x, y, 40);
}

void VDP_drawTextML(u16 plan, u8 *str, u16 basetile, u16 x, u16 y, u8 max_len) {

    u8 len = 0;
    u32 addr;
    const u8 *src = str;

    while (str[len] && len++ < 40);
    if (len > (vdp_plan_w - x)) len = vdp_plan_w - x;
    if (len > max_len)len = max_len;

    VDP_setAutoInc(2);
    addr = plan + ((x + (vdp_plan_w * y)) << 1);
    *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);

    while (len--) *((volatile u16 *) GFX_DATA_PORT) = basetile | ((*src++) - 32 + TILE_FONT_BASE);
}

void VDP_loadFont(u32 *font, u8 use_dma) {
    //VDP_loadTileTo(font, TILE_FONT_BASE, 128, use_dma);
    VDP_DoDMA(VDP_DMA_VRAM, font, TILE_FONT_BASE * 32, 128 * 32);
}

//****************************************************************************** sprite area

void VDP_spriteBuffToVram(u16 *sprite_buff, u16 sprite_num) {

    VDP_DoDMA(VDP_DMA_VRAM, sprite_buff, SLIST, sprite_num << 3);
}

void VDP_addSprite(u16 *buff_ptr, u16 x, u16 y, u8 size, u16 tile_attr, u8 link) {

    buff_ptr[0] = 0x80 + y;
    // size & link
    buff_ptr[1] = (size << 8) | link;
    // tile attribut
    buff_ptr[2] = tile_attr;
    // x position
    buff_ptr[3] = 0x80 + x;
}
//****************************************************************************** tile area
/*
void VDP_loadTileTo(const u32 *data, u16 index, u16 num, u8 use_dma) {
    u16 addr;

    use_dma = 0;
    addr = index << 5;

    if (use_dma) VDP_DoDMA(VDP_DMA_VRAM, (u32) data, addr, num << 5);
    else {

        const u32 *src;
        u16 i;

        VDP_setAutoInc(2);

 *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);

        src = data;

        i = num;

        while (i--) {

 *((volatile u32 *) GFX_DATA_PORT) = *src++;
 *((volatile u32 *) GFX_DATA_PORT) = *src++;
 *((volatile u32 *) GFX_DATA_PORT) = *src++;
 *((volatile u32 *) GFX_DATA_PORT) = *src++;
 *((volatile u32 *) GFX_DATA_PORT) = *src++;
 *((volatile u32 *) GFX_DATA_PORT) = *src++;
 *((volatile u32 *) GFX_DATA_PORT) = *src++;
 *((volatile u32 *) GFX_DATA_PORT) = *src++;
        }
    }
}

void VDP_loadFont(const u32 *font, u8 use_dma) {
    VDP_loadTileTo(font, TILE_FONT_BASE, 128, use_dma);
}

void VDP_setTile(u16 plan, u16 tile, u16 x, u16 y) {

    u32 addr = plan + ((x + (vdp_plan_w * y)) << 1);

 *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);
 *((volatile u16 *) GFX_DATA_PORT) = tile;
}

void VDP_fillTileRect(u16 plan, u16 tile, u16 x, u16 y, u16 w, u16 h) {


    u32 addr;
    u32 planwidth;
    u16 i, j;

    VDP_setAutoInc(2);

    planwidth = vdp_plan_w;
    addr = plan + ((x + (planwidth * y)) << 1);

    i = h;
    while (i--) {

 *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);
        j = w;
        while (j--) *((volatile u16 *) GFX_DATA_PORT) = tile;
        addr += planwidth << 1;
    }
}

void VDP_fillTileRectInc(u16 plan, u16 basetile, u16 x, u16 y, u16 w, u16 h) {


    u32 addr;
    u32 planwidth;
    u16 tile;
    u16 i, j;

    VDP_setAutoInc(2);


    planwidth = vdp_plan_w;
    addr = plan + ((x + (planwidth * y)) << 1);
    tile = basetile;

    i = h;
    while (i--) {

 *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);
        j = w;
        while (j--) *((volatile u16 *) GFX_DATA_PORT) = tile++;
        addr += planwidth << 1;
    }
}

void VDP_setTileRect(u16 plan, const u16 *data, u16 basetile, u16 x, u16 y, u16 w, u16 h) {


    const u16 *src;
    u32 addr;
    u32 planwidth;
    u16 i, j;

    VDP_setAutoInc(2);

    planwidth = vdp_plan_w;
    addr = plan + ((x + (planwidth * y)) << 1);
    src = data;

    i = h;
    while (i--) {

 *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);
        j = w;
        while (j--) *((volatile u16 *) GFX_DATA_PORT) = basetile | *src++;
        addr += planwidth << 1;
    }
}

void VDP_drawText(u16 plan, const char *str, u16 basetile, u16 x, u16 y) {

    u8 len = 0;
    u32 addr;
    const u8 *src = str;

    while (str[len] && len++ < 40);
    if (len > (vdp_plan_w - x)) len = vdp_plan_w - x;

    VDP_setAutoInc(2);
    addr = plan + ((x + (vdp_plan_w * y)) << 1);
 *((volatile u32 *) GFX_CTRL_PORT) = GFX_WRITE_VRAM_ADDR(addr);

    while (len--) *((volatile u16 *) GFX_DATA_PORT) = basetile | ((*src++) - 32 + TILE_FONT_BASE);
}

 */
//****************************************************************************** Z80
//******************************************************************************
//******************************************************************************

void Z80_init() {

    u16 i;
    Z80_BUSREQ_ON;
    Z80_RESET_OFF;
    while (Z80_IS_BUSREQ_OFF);
    i = 8192;
    while (i--)((u8 *) Z80_RAM)[i] = 0;
    Z80_BUSREQ_OFF;

}

void Z80_loadDriver(const u8 *driver, u16 len) {

    volatile u8 *z80_ram_ptr = (u8 *) Z80_RAM;
    const volatile u8 *driver_prt = driver;
    Z80_BUSREQ_ON;
    Z80_RESET_OFF;
    while (Z80_IS_BUSREQ_OFF);
    while (len--)*(z80_ram_ptr++) = *(driver_prt++);
    Z80_RESET_ON;
    Z80_BUSREQ_OFF;
    Z80_RESET_OFF;
}

void Z80_setBank(const u16 bank) {

    u16 i, value;
    i = 9;
    value = bank;
    while (i--) {
        *((volatile u8 *) Z80_BANK_REGISTER) = value;
        value >>= 1;
    }
}
//****************************************************************************** YM2612
//******************************************************************************
//******************************************************************************

void YM2612_reset() {


    s16 aa, bb, cc;
    ym2612_write(0x2f, 0, 0xaa); /* freq.scaler */
    ym2612_write(0x2d, 0, 0xaa);
    ym2612_write(0x22, 0, 0xaa); /* LFO off */
    ym2612_write(0x2b, 0, 0xaa); /* DAC off */
    ym2612_write(0x27, 0, 0xaa); /* CH3 normal mode */
    ym2612_write(0x27, 0, 1); /* CH3 normal mode */
    for (cc = 0; cc < 2; cc++) {
        for (aa = 0; aa < 3; aa++) {
            for (bb = 0x30; bb < 0x40; bb += 4) ym2612_write(aa + bb, 0x00, cc); /* dt1/mul */
            for (bb = 0x40; bb < 0x50; bb += 4) ym2612_write(aa + bb, 0x7f, cc); /* tl */
            for (bb = 0x50; bb < 0x60; bb += 4) ym2612_write(aa + bb, 0x00, cc); /* rs/ar */
            for (bb = 0x60; bb < 0x70; bb += 4) ym2612_write(aa + bb, 0x00, cc); /* am/d1r */
            for (bb = 0x70; bb < 0x80; bb += 4) ym2612_write(aa + bb, 0x00, cc); /* d2r */
            for (bb = 0x80; bb < 0x90; bb += 4) ym2612_write(aa + bb, 0x0f, cc); /* d1l/rr */
            for (bb = 0x90; bb < 0xa0; bb += 4) ym2612_write(aa + bb, 0x00, cc); /* ssg-eg */
            ym2612_write(0xb0 + bb, 0x00, cc); /* fb/algo */
            ym2612_write(0xb4 + bb, 0x00, cc); /* ams/fms */
            ym2612_write(0x28, aa + (cc << 2), 0); /* keyoff */
        }
    }
}

void ym2612_write(u8 reg, u8 val, u8 bank) {

    if (!bank) {
        while (YM2612_IS_BUSY);
        *((volatile u8*)YM2612_A0) = reg;
        while (YM2612_IS_BUSY);
        *((volatile u8*)YM2612_D0) = val;
    } else {
        while (YM2612_IS_BUSY);
        *((volatile u8*)YM2612_A1) = reg;
        while (YM2612_IS_BUSY);
        *((volatile u8*)YM2612_D1) = val;
    }
}

//****************************************************************************** PSG
//******************************************************************************
//******************************************************************************

void PSG_init() {

    u16 i;

    for (i = 0; i < 4; i++) {
        PSG_setEnvelope(i, PSG_ENVELOPE_MIN);
        PSG_setFrequency(i, 0);
        PSG_setTone(i, 0);
    }

}

void PSG_write(u8 data) {

    *((volatile u8*) PSG_PORT) = data;
}

void PSG_setEnvelope(u8 channel, u8 value) {

    *((volatile u8*) PSG_PORT) = 0x90 | ((channel & 3) << 5) | (value & 0xF);
}

void PSG_setTone(u8 channel, u16 value) {

    *((volatile u8*) PSG_PORT) = 0x80 | ((channel & 3) << 5) | (value & 0xF);
    *((volatile u8*) PSG_PORT) = (value >> 4) & 0x3F;
}

void PSG_setFrequency(u8 channel, u16 value) {

    u16 data;
    if (value) {

        data = 3546893 / (value * 32);
    } else {
        data = 0;
    }

    *((volatile u8*) PSG_PORT) = 0x80 | ((channel & 3) << 5) | (data & 0xF);
    *((volatile u8*) PSG_PORT) = (data >> 4) & 0x3F;
}


//****************************************************************************** JOY
//******************************************************************************
//******************************************************************************

#define JOY_DATA *((volatile u8 *) 0xa10003)
#define JOY_CONTROL *((volatile u8 *) 0xa10009)

void JOY_init() {


    volatile u8 *pb;

    pb = (u8 *) 0xa10009;
    *pb = 0x40;
    pb += 2;
    *pb = 0x40;

}

u16 JOY_readJoypad(u16 joy) {

    u8 i, j;

    JOY_DATA = 0x40;
    asm("nop");
    asm("nop");
    asm("nop");
    i = JOY_DATA & 0x3f;

    JOY_DATA = 0;

    asm("nop");
    asm("nop");
    asm("nop");
    j = (JOY_DATA & 0x30) << 2;
    return (~(i | j)) & 0xFF;
}

void JOY_waitAct() {
    while (joy == 0);
    while (joy != 0);
}
