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
    uint16_t expected = 0;
    sonar_string_init();
    sonar_string_as_int(&expected);
    assert_int_equal(expected, 123);
}

static void test_sonar_string_full_write(void **state) {
    uint16_t expected = 123;
    sonar_string_init();
    sonar_string_add_char('R');
    sonar_string_add_char('2');
    sonar_string_add_char('1');
    sonar_string_add_char('0');
    sonar_string_add_char('\r');
    sonar_string_as_int(&expected);
    assert_int_equal(expected, 210);
}

static void test_sonar_string_partial_write(void **state) {
    uint16_t expected = 123;
    sonar_string_init();
    sonar_string_add_char('R');
    sonar_string_add_char('0');
    sonar_string_add_char('3');
    sonar_string_as_int(&expected);
    assert_int_equal(expected, 123);
}


int main(int argc, char** argv) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sonar_string_init),
        cmocka_unit_test(test_sonar_string_full_write),
        cmocka_unit_test(test_sonar_string_partial_write),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
