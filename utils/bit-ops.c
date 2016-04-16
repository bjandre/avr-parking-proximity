// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Simple wrappers isolating common bit-wise operations so they can be unit tested and reused.
// tested.
//
#include <assert.h>
#include <inttypes.h>
#include <limits.h>

#include "bit-ops.h"

//    printf("byte = %s\n", byte_to_binary_string(byte));
/* #ifndef __AVR_ARCH__ */
/* #include <stdio.h> */
/* #include <string.h> */
/* const char* byte_to_binary_string(char x) { */
/*     // '0b' + 1 byte + null string terminator */
/*     static char b[CHAR_BIT + 3] = { '0', 'b', '\0'} ; */
/*     b[2] = '\0'; // static var, so need to reset the string terminator \0! */
/*     fflush(stdout); */
/*     for (int z = (1 << (CHAR_BIT - 1)); z > 0; z >>= 1) { */
/*         strcat(b, ((x & z) == z) ? "1" : "0"); */
/*     } */
    
/*     return b; */
/* } */
/* #endif */

char get_bit(volatile uint8_t *current, int position) {
    assert(position < CHAR_BIT);
    char mask = 1 << position;
    return *current & mask;
}

void set_bit_true(volatile uint8_t *byte, char bit_num) {
    /* char mask = (1 << bit_num); */
    /* return current | mask; */
    assert(bit_num < CHAR_BIT);
    *byte |= (1 << bit_num);
}

void set_bit_false(volatile uint8_t *byte, char bit_num) {
    /* char mask = ~current; */
    /* mask |= (1 << bit_num); */
    /* return ~mask; */
    assert(bit_num < CHAR_BIT);
    *byte &= ~(1 << bit_num);
}

void flip_bit(volatile uint8_t *byte, char bit_num) {
    assert(bit_num < CHAR_BIT);
    *byte ^= ~(1 << bit_num);
    *byte = ~(*byte);
}
