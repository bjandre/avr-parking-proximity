// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Blink three LEDs in sequence on an 8-bit avr attiny 2313, driving
// LEDs with PWM.
//
// Actually a single RGB led that turns on each color sequentially.
//
// RGB LED is common anode, RABG, (common connected to 3.3V,
// invididual pins connected to ground). led on when individual ping
// goes low, allowing current through. off when high.  Individual pins
// of LED connected to pin PB1, PB2, PB3

    
// #define F_CPU 1000000UL // clock frequency (Hz) defined in makefile!

#include <avr/io.h>
#include <util/delay.h>

#define delay1 250.0
#define delay2 6500.0

int main(void) {

    // set output pins for led and pwm signal
    // set pins for port b0=G, b1=B, b2=A, b3=R to output
    DDRB = 0b0001111;
    //DDRB |= (1 << PB2); // PWM output on PB2, OC0A

    // setup timer/counter 0 for pwm
    //TCCR0B = (1 << CS00);  // internal clock source, no scaling
    // NOTE(bja, 2015-10) clk/8 is max scaling without visible blinking.
    TCCR0B = (1 << CS01);  // internal clock source, clk/8 scaling
    TCCR0A = (1 << WGM00);  // PWM, phase correct, 8 bit
    TCCR0A |= (1 << COM0A1);  // Clear OC0A on Compare Match
    
    // initial PWM pulse width
    OCR0A  = 0xFF;

    // Turn the led on, off, then set to on position for pwm
    PORTB = 0b00000100; // on
    _delay_ms(delay2);
    PORTB = 0b00001111; // turn off led, leave on pwm pin
    _delay_ms(delay1);
    PORTB = 0b00000100; // on

    for(;;) {

        // change PWM pulse width every 2 seconds, note 8-bit pwm!
        _delay_ms(delay2);
        PORTB = 0b00000111; // r
        OCR0A  = 0x0F;
        _delay_ms(delay2);
        PORTB = 0b00001101; // b
        OCR0A  = 0x40;
        _delay_ms(delay2);
        PORTB = 0b00001110; // g
        OCR0A  = 0xA0;
        _delay_ms(delay2);
        PORTB = 0b00000100; // w
        OCR0A  = 0xFF;
        _delay_ms(delay2);
        PORTB = 0b00001011; // off
        _delay_ms(delay2);
        PORTB = 0b00000000; // off
    }
  
    return 0;
}

  
