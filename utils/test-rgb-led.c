// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

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
    uint8_t buffer = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    uint8_t expected = buffer | (1 << LED_ANODE_PIN);
    turn_led_on(&buffer);
    assert_int_equal(expected, buffer);
}

static void test_turn_led_on_initially_on(void **state) {
    uint8_t buffer = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    buffer |= buffer | (1 << LED_ANODE_PIN);
    uint8_t expected = buffer;
    turn_led_on(&buffer);
    assert_int_equal(expected, buffer);
}

static void test_turn_led_off_initially_on(void **state) {
    uint8_t buffer =
        (1 << LED_RED_PIN) |
        (1 << LED_BLUE_PIN) |
        (1 << LED_ANODE_PIN);
    uint8_t expected =
        (1 << LED_RED_PIN) |
        (1 << LED_BLUE_PIN) |
        (1 << LED_GREEN_PIN);
    turn_led_off(&buffer);
    assert_int_equal(expected, buffer);
}

static void test_turn_led_off_initially_off(void **state) {
    uint8_t buffer =
        (1 << LED_RED_PIN) |
        (1 << LED_BLUE_PIN);
    uint8_t expected =
        (1 << LED_RED_PIN) |
        (1 << LED_BLUE_PIN) |
        (1 << LED_GREEN_PIN);
    turn_led_off(&buffer);
    assert_int_equal(expected, buffer);
}

static void test_turn_off_led_anode_initially_on(void **state) {
    uint8_t buffer = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    uint8_t expected = buffer;
    buffer = buffer | (1 << LED_ANODE_PIN);
    turn_off_led_anode(&buffer);
    assert_int_equal(expected, buffer);
}

static void test_turn_off_led_anode_initially_off(void **state) {
    uint8_t buffer = (1 << LED_RED_PIN) | (1 << LED_BLUE_PIN);
    uint8_t expected = buffer;
    turn_off_led_anode(&buffer);
    assert_int_equal(expected, buffer);
}

int main(int argc, char** argv) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_turn_led_on_initially_on),
        cmocka_unit_test(test_turn_led_on_initially_off),
        cmocka_unit_test(test_turn_led_off_initially_on),
        cmocka_unit_test(test_turn_led_off_initially_off),
        cmocka_unit_test(test_turn_off_led_anode_initially_on),
        cmocka_unit_test(test_turn_off_led_anode_initially_off),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
