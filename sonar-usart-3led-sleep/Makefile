# -*- mode: Makefile -*-
current_makefile = $(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))
makefile_name := $(current_makefile)

include ../Makefile.inc

SUBDIRS = ../utils

HEX = sonar-usart-3led-sleep.hex
ELF = sonar-usart-3led-sleep.elf
SRCS = \
	sonar-string.c \
	sonar-usart-3led-sleep.c

OBJS = \
	$(SRCS:%.c=%.o)

LDFLAGS = -L../utils -lbjaavr

CLOCK=2000000

CFLAGS += -DF_CPU=$(CLOCK)
CFLAGS += -I../utils

# MAKECMDGOALS is the make option: make 'clobber' or 'all'
TARGET = $(MAKECMDGOALS)

#
# macro for executing TARGET in all SUBDIRS
#
ifdef SUBDIRS
$(SUBDIRS) : FORCE
	@if [ -d $@ ]; then \
		echo $(WHERE_ART_THOU); \
		$(MAKE) --file $(makefile_name) --directory=$@ $(TARGET); \
	fi	
endif	
	@echo Build complete: $@ : $(shell date)

all : $(SUBDIRS) ../utils/libbjaavr.a $(HEX)

flash : all
	$(AVRDUDE) $(AVRDUDEFLAGS) -U flash:w:$(HEX):i

read-elf : $(ELF)
	avr-readelf --all $<

#
# Generic rule to generate objects
#
%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $<


all : $(HEX)

$(ELF) :$(OBJS) ../utils/libbjaavr.a
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(HEX) : $(ELF)
	rm -f $(HEX)
	avr-objcopy -j .text -j .data -O ihex $(ELF) $@
	avr-size --format=avr --mcu=$(CHIP) $(ELF)


clean : $(SUBDIRS)
	rm -rf *~ *.o lfuse.hex hfuse.hex efuse.hex $(HEX) $(ELF)

