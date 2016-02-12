# -*- mode: Makefile; -*-
# fuse calculator:
# http://www.engbedded.com/fusecalc
# attiny2313-20p
# Features: Ext Crystal Osc; Frequency 8- MHz, Startup time: 14 CK + 65ms; cksel=1111 SUT=11
# divide clock by 8 internally
# crystal oscillator = FS16.00P - 16MHz parallel
# new fuses: -U lfuse:w:0x7f:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m 
# original fuses: avrdude: safemode: Fuses OK (E:FF, H:DF, L:64)

CHIP = attiny2313
WRITE_FUSES = -U lfuse:w:0x7f:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m 
READ_FUSES = -U lfuse:r:lfuse.hex:i -U hfuse:r:hfuse.hex:i -U efuse:r:efuse.hex:i 

CC = avr-gcc
CFLAGS = -std=c11 -Wall -Os -DNDEBUG
CFLAGS += -mmcu=$(CHIP)
# NOTE(bja, 2015-01) Set -DF_CPU=N to clock speed in individual
# makefiles so it can vary as needed.
AR = avr-ar
ARFLAGS = -rvs 

PROGRAMMER = usbtiny
AVRDUDE = avrdude
AVRDUDEFLAGS = -c $(PROGRAMMER) -p $(CHIP)

read-fuses : FORCE
	$(AVRDUDE) $(AVRDUDEFLAGS) $(READ_FUSES)

write-fuses : FORCE
	$(AVRDUDE) $(AVRDUDEFLAGS) $(WRITE_FUSES)



FORCE :
