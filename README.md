# External dependencies #

avr-gcc, avrdude

osx homebrew:

* avrdude
* osx-cross/avr/avr-gcc
* osx-cross/avr/avr-libc
* osx-cross/avr/avr-binutils


# avr-libc basics #

[avr-libc manual](http://nongnu.org/avr-libc/user-manual/)

## I/O - \<avr/io.h> ##

Pins are grouped together into ports.  Each port has three
registers. Each bit of a register controls a single pin.

  1. DDRx : (d)ata (d)irection (r)egister controling the input/output
     status of port x. input = low = 0, output = high = 1

  2. PORTx : port data register of port x, If pin is output, controls
     the high/low level of the pin. If pin is input, controls the
     internal pull up resistor. activate by setting high.
  
  3. PINx : contains the status of all pins on port x. if pin is
  input, reads pin. if pin is output, the last output value.

Note: the LSB is right most bit: 0b00000100 = 4

### References ###

* [i/o register tutorial](https://iamsuhasm.wordpress.com/tutsproj/avr-gcc-tutorial/)
* [ATmega328 LED example](http://www.micahcarrick.com/tutorials/avr-microcontroller-tutorial/getting-started.html)
* [ATmega8 LED example](http://www.tuxgraphics.org/electronics/200904/avr-c-programming.shtml)


## delay - \<util/delay.h> ##

Built in delay function:

    #define F_CPU 1000000UL // clock frequency (Hz)
    _delay_ms(double milliseconds)


Note: F_CPU can also be set from the makefile.

Note: the delay is a double! It should be a compile time constant
(macro) to be preprocessed out and avoid floating point on the chip!
Just using a const double doesn't seem to cut it.

Maximum delay is 262.14 ms / F_CPU MHz. When user request exceeds max,
silently switch to a low resolution 0.1ms timer, so the max is 6.5535
sec.


## watchdog timer - \<avr/wdt.h> ##




