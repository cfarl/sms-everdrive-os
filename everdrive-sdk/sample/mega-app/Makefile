GDK= ../../
CC= $(GDK)bin/gcc
OBJC= $(GDK)bin/objcopy
ASMZ80= $(GDK)bin/asmz80
BINTOC= $(GDK)bin/bintoc
NM= $(GDK)bin/nm
NM2WCH= $(GDK)bin/nm2wch
OUTPUT= app
RESOL=bin

SRC_S = sega.s
SRC_C = $(wildcard *.c)
SRC_LIB_C = $(wildcard $(GDK)lib/*.c)


OBJ = $(SRC_S:.s=.o)
OBJ += $(SRC_C:.c=.o)
OBJ += $(SRC_LIB_C:.c=.o)

LINKOBJ= $(OBJ)


FLAGS= $(OPTION) -m68000 -Wall -Os -fomit-frame-pointer $(INCS)
FLAGSZ80= -c -i -x1 -x2 -x3 -z -lnul

all: $(OUTPUT).$(RESOL)


$(OUTPUT).$(RESOL): $(OUTPUT).out
	$(NM) -n -S -t x $(OUTPUT).out >$(OUTPUT).nm
	$(NM2WCH) $(OUTPUT).nm $(OUTPUT).wch
	$(OBJC) --pad-to 0x110000 -O binary $(OUTPUT).out $(OUTPUT).$(RESOL)

$(OUTPUT).out: $(OBJ)
	$(CC) -T rom.ld -nostdlib $(LINKOBJ) $(GDK)bin/libgcc.a -o $(OUTPUT).out


%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) *.o
	$(RM) *.out
	$(RM) *.wch
	$(RM) *.nm

