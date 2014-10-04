AS = $(TOOLCHAIN)as
AR = $(TOOLCHAIN)ar
CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++
OBJCOPY = $(TOOLCHAIN)objcopy
PROJECT = $(lastword $(subst /, ,$(realpath .)))
LIBTARGET = lib$(PROJECT).a
SRCDIR  = ./src
INCDIR  = ./inc
SRCROOT = ../..

LPC_CHIP_PATH = $(SRCROOT)/lpc_chip_43xx
LPC_CHIP_INC  = $(LPC_CHIP_PATH)/inc
LPC_CHIP_SRC  = $(LPC_CHIP_PATH)/src

LPC_BOARD_PATH = $(SRCROOT)/lpc_board_ngx_xplorer_4330
LPC_BOARD_INC  = $(LPC_BOARD_PATH)/inc
LPC_BOARD_LIB  = -llpc_board_ngx_xplorer_4330

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:.c=.o)

MFLAGS  = -mcpu=cortex-m4
MFLAGS += -mthumb
MFLAGS += -mfloat-abi=soft
MFLAGS += -mfpu=fpv4-sp-d16

OFLAGS  = -Os
CFLAGS  = $(OFLAGS) -nostartfiles

DEFS    = -DCORE_M4
DEFS   += -D__USE_LPCOPEN

INC      = -I $(INCDIR)
INC	+= -I $(LPC_CHIP_INC)
INC	+= -I $(LPC_BOARD_INC)

all: $(LIBTARGET)
	$(AR) t $^

$(LIBTARGET): $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c
	@echo "(CC) $^"
	@$(CC) $(MFLAGS) $(INC) $(DEFS) $(CFLAGS) -c $^ -o $@ 

clean:
	@rm -f $(OBJ)
	@rm -f $(LIBTARGET)
