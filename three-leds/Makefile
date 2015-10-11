include ../Makefile.inc

HEX = three-leds.hex
ELF = three-leds.elf
SRCS = \
	three-leds.c
OBJS = \
	$(SRCS:%.c=%.o)


all : $(HEX)

flash : all
	$(AVRDUDE) $(AVRDUDEFLAGS) -U flash:w:$(HEX):i

read-elf : $(ELF)
	avr-readelf --all $<

#
# Generic rule to generate objects
#
%.o : %.c 
	$(CC) $(CFLAGS) -o $@ $<


all : $(HEX)

$(ELF) :$(OBJS)
	$(CC) -o $@ $(OBJS)

$(HEX) : $(ELF)
	rm -f $(HEX)
	avr-objcopy -j .text -j .data -O ihex $(ELF) $@
	avr-size --format=avr --mcu=$(CHIP) $(ELF)


clean :
	rm -rf *~ *.o lfuse.hex hfuse.hex efuse.hex $(HEX) $(ELF)

