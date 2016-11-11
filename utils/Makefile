include ../Makefile.inc


SRCS = \
	bit-ops.c \
	rgb-led.c 

OBJS = \
	$(SRCS:%.c=%.o)

LIB = libbjaavr.a

CLOCK=2000000

CFLAGS += -DF_CPU=$(CLOCK)

# MAKECMDGOALS is the make option: make 'clobber' or 'all'
TARGET = $(MAKECMDGOALS)

all : $(LIB)

flash : all


#
# Generic rule to generate objects
#
%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $<


$(LIB) :$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)


clean :
	rm -rf *~ *.o lfuse.hex hfuse.hex efuse.hex $(LIB) $(HEX) $(ELF)

