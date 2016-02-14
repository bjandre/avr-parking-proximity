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

#ifndef SONAR_PWM_3LED_H_
#define SONAR_PWM_3LED_H_

#include <avr/io.h>

//
// led pins set in rgb-led.h
//


// (I)nput (C)apture (P)in
#define SONAR_DATA_REGISTER PORTD
#define SONAR_PWM_PIN PD6

#endif     /* SONAR_PWM_3LED_H_ */
