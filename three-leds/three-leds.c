// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Blink three LEDs in sequence on an 8-bit avr attiny 2313
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

    // set pins for port b1, b2, b3 to output
    DDRB = 0b0001110;
    
    for(;;) {
        PORTB = 0b00000110; // 3 on, 2 off, 1 off
        _delay_ms(delay2);
        PORTB = 0b00001010; // 3 off, 2 on, 1 off
        _delay_ms(delay2);
        PORTB = 0b00001100; // 3 off, 2 off, 1 on
        _delay_ms(delay2);
    }
  
    return 0;
}
