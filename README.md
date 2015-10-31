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

### Timers and counters ###

[avr-130 AVR Timers](/doc/avr-130-timers-doc2505.pdf)

* Tx – Timer / Counter x

  * T0 - an 8-bit timer/counter. pin T0 is an input from an external
    clock source.

  * OCxA and OCxB - (O)utput (C)ompare pins for timer Tx which can
    output a frequency, i.e. PWM

* OCRxA - (O)utput (C)ompare (R)egister xA

  * loaded with a value. The timer will compare to this value every
    timer cycle. When the timer reaches this value, the (O)utput
    (C)ompare (F)lag in TIFR register is set.

* TIMSK - (T)imer/counter (I)nterupt (M)a(SK)

  * ISR - (I)nterupt (S)ervice (R)outine
  * TOIEx - (T)imer (O)verflow (I)nterupt ?E? x - bit it TIMSK - turns interupt on or off.

* TIFR - (T)imer (I)nterupt (F)lag (R)egister

  * CTC - (C)lear (T)imer on (C)ompare - mode
  * ICR - (I)nput (C)compare (R)egister
  * ICF - (I)nput (C)compare (F)lag in TIFR register
  * TOVx - ?T?imer ?OV?erflow bit?

* TCNTx - (T)imer (C)ou(NT)er Register x

  8-bit register where the counter for a timer resides.

  `TCNT0 = 0; // initialize counter`

* TCCRx(A|B) = (T)imer/(C)ounter (C)ontrol (R)egister xA and B

  timer setup register - Activates and controls the timer and counter.

  TCCRxB - Bits 2-0 select the clock source. zeroing these bits selects no
  clock and stops timer. see
  [Table 1 of AVR Timers](/doc/avr-130-timers-doc2505.pdf). 1 =
  0b001 - system clock. values 2-5 scale system clock by 8, 64, 256,
  1024 respectively. values 6-7 are external Tx pin clock, falling and
  rising edge respectively.

  * CSxx - (C)lock (S)elect bits xx - used with or-ing/shifting to set clock, e.g. `TCCR0A = (1 << CS); // turn on timer w/o prescaling`
  * COMxA(1|0) - (CO)mpare (M)atch output - bit 7-6 in TCCRxA
  * WGMxx - (W)aveform (G)eneration (M)ode xx
WGM00 = 


### References ###

* [i/o register tutorial](https://iamsuhasm.wordpress.com/tutsproj/avr-gcc-tutorial/)
* [ATmega328 LED example](http://www.micahcarrick.com/tutorials/avr-microcontroller-tutorial/getting-started.html)
* [ATmega8 LED example](http://www.tuxgraphics.org/electronics/200904/avr-c-programming.shtml)

* [avr timers](http://maxembedded.com/2011/07/avr-timers-ctc-mode/)

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



# Misc. reminders #

## c bitwise operators ##
* `a & b` and; `a &= b` assignement
* `a | b` or; `a |= b`
* `a ^ b` xor - exclusive or - return 1 in position if bits are
  different, 0 if they are the same; `a ^= b` assignment 
* `~a` not; `a ~= b` assignment
* `a >> b` right shift - shift bits in a right by b positions. if
  unsigned type,generated bits are filled with zero (logical
  shift). if signed, bits are filled with sign (arithmetic shift); `a
  >>= b` assignment
* `a << b` left shift - shift bits left by b positions. generated bits
  are filled with zero (logical and arithmetic are same); `a <<= b`
  assignment
* circular rotates, no native C operator, nieve implementations can
  open security holes. good implementation: `unsigned int y = (x << n)
  | (x >> (-n & 31));`
