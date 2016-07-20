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

#include <inttypes.h>

#include "bit-ops.h"
#include "rgb-led.h"

// led anode pin set to high, 1, for led on. low, 0, to led off
void turn_led_on(volatile uint8_t *buffer) {
    set_bit_true(buffer, LED_ANODE_PIN);
}

void turn_led_off(volatile uint8_t *buffer) {
    set_bit_false(buffer, LED_ANODE_PIN);
    set_bit_true(buffer, LED_RED_PIN);
    set_bit_true(buffer, LED_GREEN_PIN);
    set_bit_true(buffer, LED_BLUE_PIN);
}

void turn_off_led_anode(volatile uint8_t *buffer) {
    // NOTE(bja, 201607) This preserves the led color state, but will
    // only work if the anode pin is connected to a transistor. Logic
    // level 0 on the anode pin can be > Vground, up to 2 or 3 volts.
    set_bit_false(buffer, LED_ANODE_PIN);
}

// led cathode pins are low for on, high for off
void set_led_red(volatile uint8_t *buffer) {
    set_bit_false(buffer, LED_RED_PIN);
    set_bit_true(buffer, LED_GREEN_PIN);
    set_bit_true(buffer, LED_BLUE_PIN);
}

void set_led_green(volatile uint8_t *buffer) {
    set_bit_true(buffer, LED_RED_PIN);
    set_bit_false(buffer, LED_GREEN_PIN);
    set_bit_true(buffer, LED_BLUE_PIN);
}
void set_led_blue(volatile uint8_t *buffer) {
    set_bit_true(buffer, LED_RED_PIN);
    set_bit_true(buffer, LED_GREEN_PIN);
    set_bit_false(buffer, LED_BLUE_PIN);
}

void set_led_white(volatile uint8_t *buffer) {
    set_bit_false(buffer, LED_RED_PIN);
    set_bit_false(buffer, LED_GREEN_PIN);
    set_bit_false(buffer, LED_BLUE_PIN);
}
