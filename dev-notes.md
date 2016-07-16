# avr-libc basics #

[avr-libc manual](http://nongnu.org/avr-libc/user-manual/)

## I/O - `<avr/io.h>` ##

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

## Timers and counters ##

[avr-130 AVR Timers](/doc/avr-130-timers-doc2505.pdf)

* Tx - Timer / Counter x

  * T0 - an 8-bit timer/counter. pin T0 is an input from an external
    clock source.

  * T1 - an 16-bit timer/counter. pin T1 is an input from an external
    clock source. pin ICP is input capture pin tied to T1.

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

## delay - `<util/delay.h>` ##

Built in delay function:

    #define F_CPU 1000000UL // clock frequency (Hz)
    _delay_ms(double milliseconds)

NOTE: F_CPU must be consistent with fuses set for the chip (clock
source and scaling.)

Note: F_CPU can also (should) be set from the makefile with
`-DF_CPU=xxx` in the CFLAGS.

Note: the delay is a double! It should be a compile time constant
(macro) to be preprocessed out and avoid floating point on the chip!
Just using a const double doesn't seem to cut it.

Maximum delay is 262.14 ms / F_CPU MHz. When user request exceeds max,
silently switch to a low resolution 0.1ms timer, so the max is 6.5535
sec.


# watchdog timer - `<avr/wdt.h>` #

The watchdog timer counts cycles on its own oscillator.

* In system reset mode operation, it will trigger a system reset when
it reaches a given value. The user must restart the counter to avoid a
system reset. Useful for ensuring that the system will never hang for
an extended time.

* In interrupt mode the timer will trigger an interrupt when the timer
expires. Can be used to wake from sleep mode or as a general timer.

* In interrupt-reset mode, an interrupt will be triggered first, then
  then switches to system reset mode. Can be used for safe shutdown to
  save critical parameters before reset.

* Changing the watchdog functionality or prescaler requires a timed
  sequence of commands to be executed in four clock cycles!

* WDTCSR - (W)atch(d)og (t)imer (c)ontrol (r)egister

  * WDIF - bit 7 - watchdog interrupt flag - set by hardware when a
    timeout occurs and interrupt mode is enabled. if global interrupts
    are enabled, the corresponding interrupt will be executed. Cleared
    by hardware when the interrupt is executed. Can be cleared
    manually by writing a logical one.

  * WDIE - bit 6 - watchdog interrupt enable - set to one and turn on
    global interrupts to enable the watchdog interrupt.

  * WDP3 - bit 5 - see WDP2 - WDP0

  * WDCE - bit 4 - watchdog change enable - must be set to one inorder
    to change the interrupt mode and prescaler. It is automatically
    cleared to zero by the hardware after four clock cycles (for
    security).

  * WDE - bit 3 - watchdog system reset enable - trigger a system
    reset when the watchdog timer triggers. This bit is overridden by
    the WDRF bit in MCUSR. Must clear the bit in MCUSR to change this
    bit. Ensures multiple resets during failure conditions and safe
    startup.

  * WDP{2,1,0} - bit 2, 1, 0 - watchdog prescaler. set WDP0-WDP3 to
    control the prescaler.

* Fuse High Byte - WDTON - bit 4 - watchdog timer is always on

# system control and reset #

* MCUSR - (mcu) (s)tatus (r)egister

  * WDRF - bit 3 - watchdog reset flag - set as logical one

# interrupts  - `<avr/interrupt.h>` #

Three different interrupt sources. Each has it's own bit in a
register. In addition there is a global interrupt enable bit. An
interrupt's bit and the global enable bit must be set for an interrupt
to be enabled (I bit in SREG). The global bit is deactivate when an
interrupt occurs. It can be reactivated for nested interrupts, or will
automatically be reactivated when a return from interrupt instruction
is executed.

* SREG - AVR (S)tatus (Reg)ister
  * I - bit 7, Global Interrupt Enable - must be set for any interrupt
    to occur. Cleared by hardware after an interrupt has occured. RETI
    instruction resets it. Can also be set and cleared by the
    application with the SEI and CLI instructions.


## USART - `<avr/io.h>` ##

Hardware USART is available on the ATTiny2313 pins:

* RxD = PD0 = pin 2

* TxD = PD1 = pin 3

### Setup ###

* UCSRA - (U)SART (C)ontrol (S)tatus (R)egister (A)

  * RXC - bit 7 - USART receive complete - set to one when there is
    data in the receive buffer.

  * TXC - bit 6 - USART transmit complete - set to one when transmit
    shift register is empty and the transmit buffer is empty.


  * UDRE - bit 5 - USART Data Register Empty - transmit buffer is ready to
    receive new data.

  * FE - bit 4 - Frame Error - when the first stop bit of the next
    character in the receive buffer is zero.

  * DOR - bit 3 - Data OverRun - data overrun condition is detected,
    the receive buffer is full (two characters), a new character is
    waiting in the receive shift register and a new start bit has been
    detected.

  * UPE - bit 2 - parity error

  * U2X - bit 1 - Double the USART Transmission Speed. asynchronous only. Set
    to "one will reduce the divisor of the baud rate divider from 16
    to 8 effectively doubling the transfer rate for asynchronous
    communication."

  * MPCM - bit 0 - multi-processor communication mode - incoming
    frames must contain address information.

* UCSRB - (U)SART (C)ontrol (S)tatus (R)egister (B)

  * RXCIE - bit 7 - RX Complete Interrupt Enable. Enables interrupt on
    RXC flag. requires:

    * RXCIE bit = 1
    * SREG Global interrupt flag = 1
    * UCSRA RXC bit = 1

  * TXCIE - bit 6 - TX Complete Interrupt Enable

  * UDRIE - bit 5 - USART Data Register Empty Interrupt Flag

  * RXEN - bit 4 - reciever enable

  * TXEN - bit 3 - transmitter enable


    // enable both tranmit and receive
    UCSRB |= (1 << TXEN) | (1 << RXEN)


  * UCSZ2 - bit 2 - character size (combined with UCSZ0 and UCSZ1 in
    UCSRC.



* UCSRC - (U)SART (C)ontrol (S)tatus (R)egister (C)

  * UMSEL - bit 6 USART mode select - set to zero for asynchronous,
    one for synchronous

  * UPM0, UPM1 - bit 4 and bit 5 - parity mode, set both its to zero
    to disable parity.

  * USBS - bit 3 - stop bits - 0 = 1-bit, 1 = 2-bits.

  * UCSZ0, UCSZ1 - UCSRC bit 1 and bit 2, and UCSZ2 in UCSRB bit 2 - 3
    bits that control character size. 8 bit characters:


    // USART 8 bit characters:
    UCSRC |= (1 << UCSZ0);
    UCSRC |= (1 << UCSZ1);
    UCSRB |= (0 << UCSZ2);


* UBRR (U)SART (B)aud (R)ate (R)egister - Setting baud rate [bits/sec] : 




# MaxBotix LV-MaxSonar-EZ MB1010 #

## Specs ##

* Sonar range information from 6" - 245" with 1" resolution.
* Object detection from 0" - 254" (0-6 typicall reported a 6").
* Ranges from 6" - 20" may have inaccuracies of up to 2" due to phase
  cancellation.
* Factory calibrated to 1% at 5V. Operation at 3.3V typically results
  in 1-2% further reading than actual. "Objects at 3.3V are closer
  than they appear."

## Calibration ##

* Sensor calibrates on first read cycle after powerup.
* Best calibration when nearest object is >14" away.
* To recalibrate, powercycle sensor, then command read.
* calibration is for ringdown pattern in a particular set of
  conditions. Need to recalibrate when temperature or humidity changes
  significantly. If temperature increases, sonsor will have false
  close settings.  If temperature decreases, sensor will have reduced
  close sensitivity.

## Pins ##

1. 'BW' : input : open or hold low for serial output on TX, high for
   chaining.
2. 'PW' : output : pulse width representation off range. 145 mu s/in.
3. 'AN' : output : analog voltage representation of range. (Vcc/512)
   per inch. At 5V, approximately 9.8mV/in. At 3.3V approximately
   6.4mV/in.
4. 'RX' : input : internally held high for continuous ranging. Hold
   low to stop ranging. Hold high for > 20 mu s to command range
   reading.
5. 'TX' : output : Asynchronous serial with RS232 format when BW is
   open or low.
   * V = 0-Vcc.
   * Baud rate = 9600
   * 8 bits
   * no parity
   * one stop bit
   * Format: ASCII capital 'R' followed by three ASCII digits
     representing range in inches, followed by carriage return. C
     string 'R123\r'.
6. 'Vcc' : 2.5V - 5.5V, recommend 3mA for 5V, 2mA for 3V.
7. 'GND' : ground

## Timing ##

* 250 mS after power up ready to receive first RX command.
* First reading will be: 49 mS calibration followed by 49 mS reading
* all subsequent readings are 49 mS
* The RX pin is checked at the end of every reading, so readings can
  occur on 49 mS cycles.
* See timing diagram in spec sheet.
  

# Misc. #

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

## ASCII table ##

    |-------|---------|-----|----------|
    | char  | decimal | hex | binary   |
    |-------|---------|-----|----------|
    | CR    | 13      | 0D  | 00001101 |
    | R     | 82      | 52  | 01010010 |
    | zero  | 48      | 30  | 00110000 |
    | one   | 49      | 31  | 00110001 |
    | two   | 50      | 32  | 00110010 |
    | three | 51      | 33  | 00110011 |
    | four  | 52      | 34  | 00110100 |
    | five  | 53      | 35  | 00110101 |
    | six   | 54      | 36  | 00110110 |
    | seven | 55      | 37  | 00110111 |
    | eight | 56      | 38  | 00111000 |
    | nine  | 57      | 39  | 00111001 |
    |-------|---------|-----|----------|
