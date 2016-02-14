// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2015 Benjamin J. Andre
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.  2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

//
// sonar parking proximity sensor, using rgb led colors as distance
// indicator.  red --> too close, blue --> too far away, green -->
// good distance.
//

// NOTE(bja, 2016-02) We use timer counter 0 for pwm of the LED output
// because we need timer counter 1 for input capture of the sonar
// pulse width.


#include <inttypes.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "bit-ops.h"

// define common macro definitions
#include "sonar-pwm-3led.h"

#include "rgb-led.h"

#define delay1 250.0
#define delay2 6500.0

enum edges { rising, falling };
enum edges edge;

uint16_t sonar_range;

void sonar_pwm_init(void);
void led_pwm_init(void);
void cycle_led(void);


int main(void) {
    DDRB = 0;
    DDRD = 0;
    sonar_pwm_init();
    led_pwm_init();
    cycle_led();

    // initialize the range
    sonar_range = 300;

    // intial state of led
    PORTB = set_led_white(PORTB);
    PORTB = turn_led_on(PORTB);
    _delay_ms(delay1);



    // finished initialization, now we can enable global interupts
    sei();

    for(;;) {

        if (sonar_range < 30) {
            PORTB = set_led_red(PORTB);
        } else if (sonar_range > 60) {
            PORTB = set_led_blue(PORTB);
        } else {
            PORTB = set_led_green(PORTB);
        }
        // change PWM pulse width
        //OCR0A += 0x01;
        //_delay_ms(delay1);
    }

    return 0;
}


void sonar_pwm_init(void) {

    // MaxBotix LV-MaxSonar-EZ MB1010
    //
    //   * Pin 2 - PW - outputs pulse width representation of range,
    //     147 mu s/inch
    //
    //   * Range sent every 49 ms, 0.88 ms < pulse width < 37.5 ms

    // set sonar pw pin to low (input)
    SONAR_DATA_REGISTER = set_bit_false(SONAR_DATA_REGISTER, SONAR_PWM_PIN);

    // timer clock and pre scaling: assuming 1 MHz clock, need 50 ms resolution.
    // 1 x 10^6 [ticks/s] * 50 x 10^-3 [s] = 50 x 10^3 [ticks]
    // timer counter 1 is 16 bits, max uint16_t = 2^16 - 1 = 65535
    // ok to use system clock without prescaling.
    TCCR1B = set_bit_false(TCCR1B, CS12);
    TCCR1B = set_bit_false(TCCR1B, CS11);
    TCCR1B = set_bit_true(TCCR1B, CS10);

    // enable noise canceling for input capture, delays four system clock cycles
    TCCR1B = set_bit_true(TCCR1B, ICNC1);

    // ICES1 bit is zero, a falling (negative) edge is used as trigger
    // ICES1 bit is one, a raising (positive) edge is used as trigger
    // initially want to capture the leading rising edge to start timer
    TCCR1B = set_bit_true(TCCR1B, ICES1);
    edge = rising;

    // input capture interrupt enabled
    TIMSK = set_bit_true(TIMSK, ICIE1);


} // end sonar_pwm_init()


ISR(TIMER1_CAPT_vect, ISR_BLOCK) {
    // When using the Input Capture interrupt, the ICR1 Register
    // should be read as early in the interrupt handler routine as
    // possible

    // we are in an interrupt, so global intrrupts are disabled,
    // accessin the 16 bit registers should be atomic...?

    // Input Capture Register 1 is 16 bit – ICR1H and ICR1L
    // Timer/Counter1 is 16 bit – TCNT1H and TCNT1L

    if (edge == rising) {
        // we captured a rising edge, next will be falling.
        // reset the counter
        TCNT1 = 0;
        // next capture the other edge
        TCCR1B = set_bit_false(TCCR1B, ICES1);
        edge = falling;

    } else {
        // we captured a falling edge, next will be rising.
        // save the value and scale ticks to inches
        sonar_range = ICR1;
        sonar_range /= 147; // ticks * 1 x 10^6 [ticks/sec] * 147 x 10^-6 [sec/in] = [1/in]
        // next capture the other edge
        TCCR1B = set_bit_true(TCCR1B, ICES1);
        edge = rising;
    }
}


void led_pwm_init(void) {
    // set output pins for led and pwm signal
    // set pins for port b0=G, b1=B, b2=A, b3=R to output
    DDRB = set_bit_true(DDRB, LED_RED_PIN);
    DDRB = set_bit_true(DDRB, LED_ANODE_PIN);
    DDRB = set_bit_true(DDRB, LED_BLUE_PIN);
    DDRB = set_bit_true(DDRB, LED_GREEN_PIN);

    // setup timer/counter 0 for output pwm on pin OC0A
    TCCR0B = set_bit_true(TCCR0B, CS00);  // internal clock source, no scaling
    // NOTE(bja, 2015-10) clk/8 is max scaling without visible blinking at 1MHz.

    TCCR0B = set_bit_true(TCCR0B, CS01);  // internal clock source, clk/8 scaling
    TCCR0A = set_bit_true(TCCR0A, WGM00);  // PWM, phase correct, 8 bit
    TCCR0A = set_bit_true(TCCR0A, COM0A1);  // Clear OC0A on Compare Match

    // PWM pulse width without visible blinking at 1 MHz clock
    OCR0A = 0xE9;

    PORTB = turn_led_off(PORTB);  // led off
} // end led_init()


void cycle_led(void) {
    PORTB = turn_led_on(PORTB);
    _delay_ms(delay1);
    PORTB = set_led_red(PORTB);
    _delay_ms(delay1);
    PORTB = set_led_blue(PORTB);
    _delay_ms(delay1);
    PORTB = set_led_green(PORTB);
    _delay_ms(delay1);
    PORTB = turn_led_off(PORTB);
    _delay_ms(delay1);
}
