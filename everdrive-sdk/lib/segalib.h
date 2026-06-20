/* 
 * File:   segalib.h
 * Author: krik
 *
 * Created on 29 Март 2012 г., 1:31
 */

#ifndef _SEGALIB_H
#define	_SEGALIB_H

#define s8      char
#define s16     short
#define s32     long

#define u8      unsigned char
#define u16     unsigned short
#define u32     unsigned long

#define u64     unsigned long long

#define GFX_DATA_PORT           0xC00000
#define GFX_CTRL_PORT           0xC00004
#define GFX_HVCOUNTER_PORT      0xC00008

#define VDP_FIFOEMPTY_FLAG      (1 << 9)
#define VDP_FIFOFULL_FLAG       (1 << 8)
#define VDP_VINTPENDING_FLAG    (1 << 7)
#define VDP_SPROVERFLOW_FLAG    (1 << 6)
#define VDP_SPRCOLLISION_FLAG   (1 << 5)
#define VDP_ODDFRAME_FLAG       (1 << 4)
#define VDP_VBLANK_FLAG         (1 << 3)
#define VDP_FBLANK_FLAG         (1 << 2)
#define VDP_DMABUSY_FLAG        (1 << 1)
#define VDP_PALMODE_FLAG        (1 << 0)
#define VSCROLL_MODE_SCREEN 0
#define VSCROLL_MODE_TILE 1
#define HSCROLL_MODE_SCREEN 0
#define HSCROLL_MODE_SCAN 1
#define HSCROLL_MODE_TILE 2
#define HSCROLL_MODE_LINE 3




#define FONT_LEN                128


#define TILE_MEM_END           0xB000
#define TILE_MAXNUM             (TILE_MEM_END / 32)
#define TILE_FONT_BASE          (TILE_MEM_END / 32  - FONT_LEN)


#define WPLAN                   (TILE_MEM_END + 0x0000)
#define HSCRL                   (TILE_MEM_END + 0x0800)
#define SLIST                   (TILE_MEM_END + 0x0C00)
#define APLAN                   (TILE_MEM_END + 0x1000)
#define BPLAN                   (TILE_MEM_END + 0x3000)


#define GFX_READ_VRAM_ADDR(adr)     ((0x0000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x00)
#define GFX_READ_CRAM_ADDR(adr)     ((0x0000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x20)
#define GFX_READ_VSRAM_ADDR(adr)    ((0x0000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x10)

#define GFX_WRITE_VRAM_ADDR(adr)    ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x00)
#define GFX_WRITE_CRAM_ADDR(adr)    ((0xC000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x00)
#define GFX_WRITE_VSRAM_ADDR(adr)   ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x10)

#define GFX_DMA_VRAM_ADDR(adr)      ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x80)
#define GFX_DMA_CRAM_ADDR(adr)      ((0xC000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x80)
#define GFX_DMA_VSRAM_ADDR(adr)     ((0x4000 + ((adr) & 0x3FFF)) << 16) + (((adr) >> 14) | 0x90)

#define GFX_VERT_SCROLL(adr)        GFX_WRITE_VSRAM_ADDR(adr)
#define GFX_HORZ_SCROLL(adr)        GFX_WRITE_VRAM_ADDR(adr)

#define GETVDPSTATUS(flag)          ((*(volatile u16*)(GFX_CTRL_PORT)) & (flag))
#define ISPALSYSTEM                 GETVDPSTATUS(VDP_PALMODE_FLAG)

#define GET_HVCOUNTER               (*(volatile u16*)(GFX_HVCOUNTER_PORT))
#define GET_HCOUNTER                (GET_HVCOUNTER & 0xFF)
#define GET_VCOUNTER                (GET_HVCOUNTER >> 8)

#define CONSOLE_REGION (*((volatile u16*) (0xa10000)) >> 6 & 3)

void VDP_init();
void VDP_setReg(u8 reg, u8 value);
void VDP_setWidth256();
void VDP_setWidth320();
void VDP_setPlanSize(u16 w, u16 h);
void VDP_setAutoInc(u8 value);
void VDP_setHInterrupt(u8 value);
void VDP_setHilightShadow(u8 value);
void VDP_waitFIFOEmpty();
void VDP_waitVSync();

extern u16 vdp_plan_w;
extern u16 vdp_plan_h;

void VDP_setHorizontalScroll(u16 plan, u16 line, u16 value);
void VDP_setVerticalScroll(u16 plan, u16 cell, u16 value);
void VDP_setScrollMode(u8 vs, u8 hs);
void VDP_vintOff();
void VDP_vintOn();
u16 VDP_vinIsEnabled();



//****************************************************************************** pal area
void VDP_setPaletteColor(u16 numpal, u16 numcol, u16 value);
void VDP_setPalette(u16 num, void *pal);
void VDP_getPalette(u16 num, u16 *pal);
u16 VDP_getPaletteColor(u16 numpal, u16 numcol);
//****************************************************************************** DMA area

void VDP_DoDMA(u8 area, void *from, u16 to, u16 len); //ram to vram
void VDP_DoDMAFill(u16 to, u16 len, u16 value); //fill vram (not cram and not vsram!!)
void VDP_DoDMACopy(u8 area, u16 from, u16 to, u16 len); //vram to vram
void VDP_readVram(u16 src, u16 *dst, u16 len);
void VDP_readCram(u16 src, u16 *dst, u16 len);
void VDP_readVsram(u16 src, u16 *dst, u16 len);
void VDP_waitDMACompletion();
u16 VDP_dmaIsComplete();
#define VDP_DMA_VRAM    0
#define VDP_DMA_CRAM    1
#define VDP_DMA_VSRAM   2
//****************************************************************************** sprite area

typedef struct {
    s16 y;
    u8 size;
    u8 link;
    u16 attr;
    s16 x;
} Sprite;

void VDP_spriteBuffToVram(u16 *sprite_buff, u16 sprite_num);
void VDP_addSprite(u16 *buff_ptr, u16 x, u16 y, u8 size, u16 tile_attr, u8 link);

#define SPRITE_SIZE(w, h)   ((((w) - 1) << 2) | ((h) - 1))
#define SPRITE_POS(pos) (0x80 + pos)
//****************************************************************************** tile area
void VDP_loadTileTo( u32 *data, u16 index, u16 num, u8 use_dma); //load tile data to vram
void VDP_loadFont( u32 *font, u8 use_dma); //load font data to vram
void VDP_setTile(u16 plan, u16 tile, u16 x, u16 y); //set tile index to video buffer area in vram, from now this tile is visible
void VDP_fillTileRect(u16 plan, u16 tile, u16 x, u16 y, u16 w, u16 h); //fill video bufer. as example this can be used for clear screen.
void VDP_fillTileRectInc(u16 plan, u16 basetile, u16 x, u16 y, u16 w, u16 h); //incemented video buffer fill. can be used for place image
void VDP_setTileRect(u16 plan, u16 *data, u16 basetile, u16 x, u16 y, u16 w, u16 h); //copy array to video buffer
void VDP_drawText(u16 plan, u8 *str, u16 basetile, u16 x, u16 y);
void VDP_drawTextML(u16 plan, u8 *str, u16 basetile, u16 x, u16 y, u8 max_len);
#define TILE_ATTR(pal, pri, flipV, flipH)   (((flipH) << 11) + ((flipV) << 12) + ((pal) << 13) + ((pri) << 15))

//****************************************************************************** Z80
//******************************************************************************
//******************************************************************************

#define Z80_HALT_PORT       0xA11100
#define Z80_RESET_PORT      0xA11200

#define Z80_RAM             0xA00000
#define Z80_YM2612          0xA04000
#define Z80_BANK_REGISTER   0xA06000
#define Z80_PSG             0xA07F11

#define Z80_BUSREQ_ON *((volatile u16 *) Z80_HALT_PORT) = 0x0100
#define Z80_BUSREQ_OFF *((volatile u16 *) Z80_HALT_PORT) = 0x0000
#define Z80_IS_BUSREQ_OFF (*((volatile u16 *) Z80_HALT_PORT) & 0x0100)

#define Z80_RESET_ON *((volatile u16 *) Z80_RESET_PORT) = 0
#define Z80_RESET_OFF *((volatile u16 *) Z80_RESET_PORT) = 0x0100

void Z80_busReq(u8 req, u8 wait);
void Z80_reset(u8 reset);
void Z80_init();
void Z80_setBank(const u16 bank);
void Z80_loadDriver(const u8 *driver, u16 len);

//****************************************************************************** YM2612
//******************************************************************************
//******************************************************************************

#define YM2612_BASEPORT     0xA04000
#define YM2612_A0   	0xa04000
#define YM2612_D0   	0xa04001
#define YM2612_A1   	0xa04002
#define YM2612_D1   	0xa04003

#define PSG_ENVELOPE_MIN    15
#define PSG_ENVELOPE_MAX    0

#define YM2612_IS_BUSY (*((volatile u8*) YM2612_BASEPORT) & 0x80)

void YM2612_reset();
void ym2612_write(u8 reg, u8 val, u8 bank);

//****************************************************************************** PSG
//******************************************************************************
//******************************************************************************

#define PSG_PORT            0xC00011
#define PSG_ENVELOPE_MIN    15
#define PSG_ENVELOPE_MAX    0

void PSG_init();

void PSG_write(u8 data);

void PSG_setEnvelope(u8 channel, u8 value);
void PSG_setTone(u8 channel, u16 value);
void PSG_setFrequency(u8 channel, u16 value);


//****************************************************************************** JOY
//******************************************************************************
//******************************************************************************

extern volatile u16 joy;

#define JOY_1           0x0000
#define JOY_2           0x0001

#define BUTTON_UP       0x0001
#define BUTTON_DOWN     0x0002
#define BUTTON_LEFT     0x0004
#define BUTTON_RIGHT    0x0008
#define BUTTON_A        0x0040
#define BUTTON_B        0x0010
#define BUTTON_C        0x0020
#define BUTTON_START    0x0080
#define BUTTON_X        0x0100
#define BUTTON_Y        0x0200
#define BUTTON_Z        0x0400
#define BUTTON_MODE     0x0800

void JOY_init();
u16 JOY_readJoypad(u16 joy);
void JOY_waitAct();

#endif	/* _SEGALIB_H */

