// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "rgb-led.h"

//    printf("mask = %s\n", byte_to_binary_string(mask));
//    printf("current = %s\n", byte_to_binary_string(current));    
//    printf("received = %s\n", byte_to_binary_string(received));    
//    printf("expected = %s\n", byte_to_binary_string(expected));    

static void test_turn_led_on_initially_off(void **state) {
    char current = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    char expected = current | (1 << LED_ANODE_PIN);
    char received = turn_led_on(current);
    assert_int_equal(received, expected);
}

static void test_turn_led_on_initially_on(void **state) {
    char current = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    current |= current | (1 << LED_ANODE_PIN);
    char expected = current;
    char received = turn_led_on(current);
    assert_int_equal(received, expected);
}

static void test_turn_led_off_initially_on(void **state) {
    char current = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    char expected = current;
    current = current | (1 << LED_ANODE_PIN);
    char received = turn_led_off(current);
    assert_int_equal(received, expected);
}

static void test_turn_led_off_initially_off(void **state) {
    char current = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    char expected = current;
    char received = turn_led_off(current);
    assert_int_equal(received, expected);
}

int main(int argc, char** argv) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_turn_led_on_initially_on),
        cmocka_unit_test(test_turn_led_on_initially_off),
        cmocka_unit_test(test_turn_led_off_initially_on),
        cmocka_unit_test(test_turn_led_off_initially_off),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
