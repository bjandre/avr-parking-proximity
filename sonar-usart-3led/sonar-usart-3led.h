// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// isolate macro definitions so that code is easier to reuse.
//

#ifndef SONAR_USART_3LED_H_
#define SONAR_USART_3LED_H_

#include <avr/io.h>

//
// pins
//
#define LED_RED_PIN PB3
#define LED_ANODE_PIN PB2
#define LED_BLUE_PIN PB1
#define LED_GREEN_PIN PB0

#define USART_RX_PIN PD0

#endif     /* SONAR_USART_3LED_H_ */
