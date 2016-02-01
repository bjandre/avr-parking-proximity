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

#ifndef LED_ANODE_PIN
#  define LED_ANODE_PIN 0
#  define LED_RED_PIN 1
#  define LED_BLUE_PIN 2
#  define LED_GREEN_PIN 3
#endif

char turn_led_on(char current);
char turn_led_off(char current);

char get_led_switch_status(char current);

char set_led_red(char current);
char set_led_blue(char current);
char set_led_green(char current);
char set_led_white(char current);


#endif     /* RGB_LED_H_ */
