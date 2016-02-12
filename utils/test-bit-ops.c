// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// 

#include <limits.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "bit-ops.h"

static void test_get_bit_on(void **state) {
    char current = 0b00011101;
    int position = 3;
    char mask = get_bit(current, position);
    assert_int_equal(mask, (1 << position));
}

static void test_get_bit_off(void **state) {
    char current = 0b11110111;
    int position = 3;
    char mask = get_bit(current, position);
    assert_int_equal(mask, 0b00000000);
}

static void test_set_bit_true_current_false(void **state) {
    char current  = 0b10101101;
    char expected = current;
    int position = 3;
    char received = set_bit_true(current, position);
    assert_int_equal(expected, received);
}

static void test_set_bit_true_current_true(void **state) {
    char current  = 0b10101101;
    char expected = 0b10111101;    
    int position = 4;
    char received = set_bit_true(current, position);
    assert_int_equal(expected, received);
}

static void test_set_bit_false_current_false(void **state) {
    char current  = 0b10101101;
    char expected = current;
    int position = 4;
    char received = set_bit_false(current, position);
    assert_int_equal(expected, received);
}

static void test_set_bit_false_current_true(void **state) {
    char current  = 0b10101101;
    char expected = 0b10100101;
    int position = 3;
    char received = set_bit_false(current, position);
    assert_int_equal(expected, received);
}


static void test_flip_bit_current_true(void **state) {
    char current  = 0b10101101;
    char expected = 0b10100101;
    int position = 3;
    char received = flip_bit(current, position);
    assert_int_equal(expected, received);
}

static void test_flip_bit_current_false(void **state) {
    char current  = 0b10100101;
    char expected = 0b10101101;
    int position = 3;
    char received = flip_bit(current, position);
    assert_int_equal(expected, received);
}


int main(int argc, char** argv) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_bit_on),
        cmocka_unit_test(test_get_bit_off),
        cmocka_unit_test(test_set_bit_true_current_false),
        cmocka_unit_test(test_set_bit_true_current_true),
        cmocka_unit_test(test_set_bit_false_current_false),
        cmocka_unit_test(test_set_bit_false_current_true),
        cmocka_unit_test(test_flip_bit_current_true),
        cmocka_unit_test(test_flip_bit_current_false),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
