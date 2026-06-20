GDK= ../everdrive-sdk/
CC= $(GDK)bin/gcc
OBJC= $(GDK)bin/objcopy
ASMZ80= $(GDK)bin/asmz80
BINTOC= $(GDK)bin/bintoc
NM= $(GDK)bin/nm
NM2WCH= $(GDK)bin/nm2wch
OUTPUT= MDOS
RESOL=BIN

all:

#******************************************************************************* SMS
HOME_Z80=c:/SDCC-3.3/bin
CC_Z80=$(HOME_Z80)/sdcc	-mz80 --xram-loc 0xc000
LINK_Z80=$(HOME_Z80)/sdcc -mz80  --data-loc 0xc000 --no-std-crt0


SRC_S_SMS = bios_msed.s sys_sms_a.s
SRC_C_SMS = $(wildcard *.c)

OBJ_SMS = $(SRC_S_SMS:.s=_sms.rel)
OBJ_SMS += $(SRC_C_SMS:.c=_sms.rel)

make_sms:  MSOS.SMS

MSOS.SMS: $(OBJ_SMS)
	  $(LINK_Z80) $(OBJ_SMS)   -o MSOS.IHX
	  $(HOME_Z80)/makebin -s 31744 <MSOS.IHX > MSOS.SMX
#	  cat MSOS.SMX font_sms.bin > MSOS.SMS
	
%_sms.rel : %.c
	$(CC_Z80) -c $< -o $@ -D CART_MSED


%_sms.rel : %.s
	$(HOME_Z80)/sdasz80 -l -o $@ $<

clean_sms:
	$(RM) *.rel
	$(RM) *.sym
	$(RM) *.asm
	$(RM) *.lst
	$(RM) *.o
	$(RM) MSOS.*
