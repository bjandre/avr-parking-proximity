// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Blink a single LED using pulse width modulation on an 8-bit avr
// attiny 2313

//
// Same circuit as three-leds-pwm.c
// 
    
#include <avr/io.h>
#include <util/delay.h>

static const unsigned int delay1 = 250.0;
static const unsigned int delay2 = 2000.0;

int main(void) {

    // set output pins for led and pwm signal
    DDRB = 0b0001000; // pin PB3, ddrb pin 3, red led
    DDRB |= (1 << PB2); // PWM output on PB2, OC0A

    // setup timer/counter 0 for pwm
    //TCCR0B = (1 << CS00);  // internal clock source, no scaling
    // NOTE(bja, 2015-10) clk/8 is max scaling without visible blinking.
    TCCR0B = (1 << CS01);  // internal clock source, clk/8 scaling
    TCCR0A = (1 << WGM00);  // PWM, phase correct, 8 bit
    TCCR0A |= (1 << COM0A1);  // Clear OC0A on Compare Match
    
    // initial PWM pulse width
    OCR0A  = 0xFF;

    // Turn the led on, off, then set to on position for pwm
    PORTB = 0b00000000; // on
    _delay_ms(delay2);
    PORTB = 0b00001000; // turn off led, leave on pwm pin
    _delay_ms(delay1);
    PORTB = 0b00000000; // on

    for(;;) {

        // change PWM pulse width every 2 seconds, note 8-bit pwm!
        _delay_ms(delay2);
        OCR0A  = 0x0F;
        _delay_ms(delay2);
        OCR0A  = 0x40;
        _delay_ms(delay2);
        OCR0A  = 0xA0;
        _delay_ms(delay2);
        OCR0A  = 0xFF;
    }
  
    return 0;
}
