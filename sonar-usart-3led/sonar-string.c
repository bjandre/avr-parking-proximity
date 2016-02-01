// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// store the sonar data received from usart in a string buffer and
// convert to an integer range value.
//
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sonar-string.h"

// Sonar usart output format: ASCII capital 'R' followed by three
// ASCII digits representing range in inches, followed by carriage
// return. C string 'R123\r'.

#define MAXSONAR_STR_LEN 5
static const uint8_t sonar_string_length = MAXSONAR_STR_LEN;
static char sonar_string[MAXSONAR_STR_LEN];
static uint8_t sonar_string_index;

void sonar_string_init(void) {
    strncpy(sonar_string, "R123\r", sonar_string_length);
    sonar_string_index = sonar_string_length;
}

void sonar_string_add_char(uint8_t next_char) {
    if (next_char == 'R') {
        sonar_string_index = 0;
    }
    sonar_string[sonar_string_index] = next_char;
    sonar_string_index++;
}

void sonar_string_as_int(uint16_t *sonar_range) {
    if (sonar_string_index == sonar_string_length) {
        *sonar_range = atoi(sonar_string + 1);
    }
}
