// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Routines for turning a RGB led on/off and switching pure colors

// RGB LED is common anode, RABG, (common connected to 3.3V,
// invididual pins connected to ground). led on when individual pin
// goes low, allowing current through. off when high.  Individual pins
// of LED connected to pin 

#ifndef RGB_LED_H_
#define RGB_LED_H_

#include <inttypes.h>

#ifdef __AVR_ARCH__
#  include <avr/io.h>
   // pins on board
#  define LED_RED_PIN PB3
#  define LED_ANODE_PIN PB2
#  define LED_BLUE_PIN PB1
#  define LED_GREEN_PIN PB0
#else
   // mock values for unit testing
#  define LED_ANODE_PIN 0
#  define LED_RED_PIN 1
#  define LED_BLUE_PIN 2
#  define LED_GREEN_PIN 3
#endif

void turn_led_on(volatile uint8_t *buffer);
void turn_led_off(volatile uint8_t *buffer);
void turn_off_led_anode(volatile uint8_t *buffer);

uint8_t get_led_switch_status(volatile uint8_t *buffer);

void set_led_red(volatile uint8_t *buffer);
void set_led_blue(volatile uint8_t *buffer);
void set_led_green(volatile uint8_t *buffer);
void set_led_white(volatile uint8_t *buffer);


#endif     /* RGB_LED_H_ */
