include ../Makefile.inc

HEX = single-led-pwm.hex
ELF = single-led-pwm.elf
SRCS = \
	single-led-pwm.c
OBJS = \
	$(SRCS:%.c=%.o)


CLOCK=1000000

CFLAGS += -DF_CPU=$(CLOCK)

all : $(HEX)

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

$(ELF) :$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(HEX) : $(ELF)
	rm -f $(HEX)
	avr-objcopy -j .text -j .data -O ihex $(ELF) $@
	avr-size --format=avr --mcu=$(CHIP) $(ELF)


clean :
	rm -rf *~ *.o lfuse.hex hfuse.hex efuse.hex $(HEX) $(ELF)

