// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Isolating LED register manipulation functions so they can be unit
// tested.
//
// RGB LED is common anode, RABG, (common annode connected to Vcc,
// invididual pins connected to ground). led on when individual pin
// goes low, allowing current through. off when high.  Individual pins
// of LED are defined by the macros LED_RED_PIN, LED_BLUE_PIN,
// LED_GREEN_PIN

#include "bit-ops.h"
#include "rgb-led.h"

// led anode pin set to high, 1, for led on. low, 0, to led off
char turn_led_on(char current) {
    return set_bit_true(current, LED_ANODE_PIN);
}

char turn_led_off(char current) {
    return set_bit_false(current, LED_ANODE_PIN);
}

// led cathode pins are low for on, high for off
char set_led_red(char current) {
    current = set_bit_false(current, LED_RED_PIN);
    current = set_bit_true(current, LED_GREEN_PIN);
    current = set_bit_true(current, LED_BLUE_PIN);
    return current;
}


char set_led_green(char current) {
    current = set_bit_true(current, LED_RED_PIN);
    current = set_bit_false(current, LED_GREEN_PIN);
    current = set_bit_true(current, LED_BLUE_PIN);
    return current;
}
char set_led_blue(char current) {
    current = set_bit_true(current, LED_RED_PIN);
    current = set_bit_true(current, LED_GREEN_PIN);
    current = set_bit_false(current, LED_BLUE_PIN);
    return current;
}

char set_led_white(char current) {
    current = set_bit_false(current, LED_RED_PIN);
    current = set_bit_false(current, LED_GREEN_PIN);
    current = set_bit_false(current, LED_BLUE_PIN);
    return current;
}
