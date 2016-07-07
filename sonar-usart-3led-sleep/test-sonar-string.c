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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "sonar-string.h"

static void test_sonar_string_init(void **state) {
    // Test that the string is initialized correctly and the integer
    // conversion is correct.
    uint16_t expected = 123;
    uint16_t current = 999;
    uint16_t received;
    sonar_string_init();
    received = sonar_string_as_int(current);
    assert_int_equal(received, expected);
}

static void test_sonar_string_full_write(void **state) {
    // Test that a full write cycle returns the corrent value.
    uint16_t expected = 210;
    uint16_t initial = 123;
    uint16_t current = 123;
    uint16_t final;
    sonar_string_init();
    current = sonar_string_as_int(initial);
    assert_int_equal(current, initial);
    
    sonar_string_add_char('R');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('2');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('1');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('0');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('\r');
    final = sonar_string_as_int(current);
    assert_int_equal(final, expected);
}

static void test_sonar_string_partial_write(void **state) {
    // Test that the integer range value doesn't update during the
    // intermediate steps..
    uint16_t expected = 123;
    uint16_t current = 123;
    uint16_t received;
    sonar_string_init();

    sonar_string_add_char('R');
    received = sonar_string_as_int(current);
    assert_int_equal(received, expected);

    sonar_string_add_char('0');
    received = sonar_string_as_int(current);
    assert_int_equal(received, expected);

    sonar_string_add_char('3');
    received = sonar_string_as_int(current);
    assert_int_equal(received, expected);
}

static void test_sonar_string_missed_write(void **state) {
    // Test that the integer value does not update if an intermediate
    // update is missed and the string range starts over.
    uint16_t initial = 123;
    uint16_t expected = 135;
    uint16_t final;
    uint16_t current;
    sonar_string_init();
    current = sonar_string_as_int(initial);
    assert_int_equal(current, initial);

    sonar_string_add_char('R');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('0');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('3');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('\r');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('R');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('1');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('3');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('5');
    current = sonar_string_as_int(current);
    assert_int_equal(current, initial);

    sonar_string_add_char('\r');
    final = sonar_string_as_int(current);
    assert_int_equal(final, expected);
}


int main(int argc, char** argv) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sonar_string_init),
        cmocka_unit_test(test_sonar_string_full_write),
        cmocka_unit_test(test_sonar_string_partial_write),
        cmocka_unit_test(test_sonar_string_missed_write),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
