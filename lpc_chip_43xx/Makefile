AS = $(TOOLCHAIN)as
AR = $(TOOLCHAIN)ar
CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++
OBJCOPY = $(TOOLCHAIN)objcopy
PROJECT = $(lastword $(subst /, ,$(realpath .)))
LIBTARGET = lib$(PROJECT).a
SRCDIR  = ./src
INCDIR  = ./inc
SRCROOT = ..

SRC_FILES  = chip_18xx_43xx.c sysinit_18xx_43xx.c clock_18xx_43xx.c fpu_init.c gpio_18xx_43xx.c gpiogroup_18xx_43xx.c
SRC_FILES += i2c_18xx_43xx.c i2s_18xx_43xx.c uart_18xx_43xx.c ring_buffer.c adc_18xx_43xx.c aes_18xx_43xx.c
SRC_FILES += atimer_18xx_43xx.c ccan_18xx_43xx.c dac_18xx_43xx.c eeprom_18xx_43xx.c emc_18xx_43xx.c enet_18xx_43xx.c
SRC_FILES += evrt_18xx_43xx.c gpdma_18xx_43xx.c hsadc_18xx_43xx.c i2cm_18xx_43xx.c iap_18xx_43xx.c lcd_18xx_43xx.c
SRC_FILES += otp_18xx_43xx.c pinint_18xx_43xx.c pmc_18xx_43xx.c rgu_18xx_43xx.c ritimer_18xx_43xx.c rtc_18xx_43xx.c
SRC_FILES += sct_18xx_43xx.c sct_pwm_18xx_43xx.c scu_18xx_43xx.c sdif_18xx_43xx.c sdmmc_18xx_43xx.c spi_18xx_43xx.c
SRC_FILES += ssp_18xx_43xx.c stopwatch_18xx_43xx.c timer_18xx_43xx.c wwdt_18xx_43xx.c

SRC = $(foreach file,$(SRC_FILES),$(SRCDIR)/$(file))
OBJ = $(SRC:.c=.o)

MFLAGS  = -mcpu=cortex-m4
MFLAGS += -mthumb
MFLAGS += -mfloat-abi=soft
MFLAGS += -mfpu=fpv4-sp-d16

OFLAGS  = -Os
CFLAGS  = $(OFLAGS) -nostartfiles
LDFLAGS = $(OFLAGS) -nostartfiles -Wl,-T,$(PROJECT).ld

DEFS    = -DCORE_M4
DEFS   += -D__USE_LPCOPEN

INC      = -I $(INCDIR)

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
