AS = $(TOOLCHAIN)as
CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++
OBJCOPY = $(TOOLCHAIN)objcopy
SRCROOT = ../../..

LPC_CHIP_PATH = $(SRCROOT)/lpc_chip_43xx
LPC_CHIP_INC  = $(LPC_CHIP_PATH)/inc
LPC_CHIP_LIB  = -llpc_chip_43xx

LPC_BOARD_NGX_XPLORER_4330_PATH = $(SRCROOT)/lpc_board_ngx_xplorer_4330
LPC_BOARD_NGX_XPLORER_4330_INC = $(LPC_BOARD_NGX_XPLORER_4330_PATH)/inc
LPC_BOARD_NGX_XPLORER_4330_LIB = -llpc_board_ngx_xplorer_4330

LPC_SYSCALLS_PATH = $(SRCROOT)/syscalls
LPC_SYSCALLS_INC = $(LPC_SYSCALLS)/inc
LPC_SYSCALLS_LIB = -lsyscalls

LPC_FATFS_PATH = ../../fatfs
LPC_FATFS_INC = $(LPC_FATFS_PATH)/inc
LPC_FATFS_LIB = -lfatfs

LPC_LWIP_PATH = ../../lwip
LPC_LWIP_INC  = $(LPC_LWIP_PATH)/inc
LPC_LWIP_LIB  = -llwip

LIBPATH = -L $(LPC_BOARD_NGX_XPLORER_4330_PATH) -L $(LPC_CHIP_PATH) -L $(LPC_SYSCALLS_PATH) -L $(LPC_FATFS_PATH) -L $(LPC_LWIP_PATH)

LIBS  = -Wl,--start-group
LIBS += -lgcc 
LIBS += -lc
LIBS += $(LPC_SYSCALLS_LIB)
LIBS += $(LPC_BOARD_NGX_XPLORER_4330_LIB)
LIBS += $(LPC_CHIP_LIB)
LIBS += $(LPC_FATFS_LIB)
LIBS += $(LPC_LWIP_LIB)
LIBS += -Wl,--end-group

SRC = $(wildcard *.c)
PROJECT = $(lastword $(subst /, ,$(realpath $(dir $(realpath $(firstword $(SRC))))/../..)))
OBJ = $(SRC:.c=.o)

MFLAGS  = -mcpu=cortex-m4
MFLAGS += -mthumb
MFLAGS += -mfloat-abi=soft
MFLAGS += -mfpu=fpv4-sp-d16

OFLAGS  = -Os
CFLAGS  = $(OFLAGS) 
LDFLAGS = $(OFLAGS) $(LIBPATH) -static -nostartfiles -nostdlib -Wl,-T,$(PROJECT).ld

DEFS    = -DCORE_M4
DEFS   += -D__USE_LPCOPEN

INC      = -I $(LPC_CHIP_INC)
INC	+= -I $(LPC_BOARD_NGX_XPLORER_4330_INC)
INC	+= -I $(LPC_LWIP_INC)
INC	+= -I $(LPC_LWIP_INC)/ipv4
INC	+= -I $(LPC_FATFS_INC)
INC	+= -I ../inc

all: $(PROJECT).dfu

$(PROJECT).dfu: $(PROJECT).bin
	@echo "($(PROJECT) HDR) $^ --> $@ "
	@cp $^ $@
	@dfu-prefix -a $@ -L

$(PROJECT).bin: $(PROJECT).elf
	@echo "($(PROJECT) OBJ_CP) $^ $@"
	@$(OBJCOPY) --strip-unneeded -O binary $^ $@

$(PROJECT).elf: $(OBJ)
	@echo "($(PROJECT) LD) $^"
	$(CC) $(MFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	@echo "($(PROJECT) CC) $^"
	@$(CC) $(MFLAGS) $(INC) $(DEFS) $(CFLAGS) -c $^ -o $@ 

clean:
	@rm -f $(OBJ)
	@rm -f $(PROJECT).elf
	@rm -f $(PROJECT).bin
	@rm -f $(PROJECT).dfu
