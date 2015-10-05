// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Blink a single LED on an 8-bit avr attiny 2313
// 
    
// #define F_CPU 1000000UL // clock frequency (Hz) defined in makefile!

#include <avr/io.h>
#include <util/delay.h>

#define delay1 250.0
#define delay2 250.0

int main(void) {

    /* DDRB = _BV(3); */
    DDRB = 0b0001000;
    
    for(;;) {
        PORTB = 0b00000000; // on
        _delay_ms(delay1);
        PORTB = 0b00001000; // off
        _delay_ms(delay2);
    }
  
    return 0;
}
