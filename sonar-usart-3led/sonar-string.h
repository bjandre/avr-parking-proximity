// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015-2016 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// convert sonar usart string to integer
//

// Sonar usart output format: ASCII capital 'R' followed by three
// ASCII digits representing range in inches, followed by carriage
// return. C string 'R123\r'.

#ifndef SONAR_STRING_TO_INT_H_
#define SONAR_STRING_TO_INT_H_

#include <inttypes.h>

void sonar_string_init(void);
void sonar_string_add_char(uint8_t next_char);
void sonar_string_as_int(uint16_t *sonar_string);

#endif     /* SONAR_STRING_TO_INT_H_ */
