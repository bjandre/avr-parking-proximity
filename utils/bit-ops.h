
// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015-2016 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

// 
// Simple wrappers for common bit-wise operations
//

#ifndef BIT_OPS_H_
#define BIT_OPS_H_

#include <inttypes.h>

//    printf("mask = %s\n", byte_to_binary_string(mask));
//    printf("current = %s\n", byte_to_binary_string(current));    
//    printf("received = %s\n", byte_to_binary_string(received));    
//    printf("expected = %s\n", byte_to_binary_string(expected));    
#ifndef __AVR_ARCH__
    const char* byte_to_binary_string(char x);
#endif
char get_bit(volatile uint8_t *current, int position);
void set_bit_true(volatile uint8_t *byte, char bit_num);
void set_bit_false(volatile uint8_t *byte, char bit_num);
void flip_bit(volatile uint8_t *byte, char bit_num);

#endif     /* BIT_OPS_H_ */
