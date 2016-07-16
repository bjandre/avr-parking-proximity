// -*- mode: c; c-default-style: "k&r"; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-

//
// Copyright (c) 2016 Benjamin J. Andre
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

    
// #define F_CPU 1000000UL // clock frequency (Hz) defined in makefile!

#include <stdlib.h>
#include <inttypes.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#define BAUD 9600
#include <util/setbaud.h>

#include "bit-ops.h"

// define common macro definitions
#include "sonar-usart-3led-sleep.h"
#include "rgb-led.h"
#include "sonar-string.h"

#define delay1 250.0
#define delay2 6500.0

#define DEBUG_SERIAL 1

uint8_t sonar_range; // [inches], integer range 0-254
uint8_t sonar_range_previous; // inches
static const uint8_t max_delta_range = 3; // inches

void usart_init(void);
void led_pwm_init(void);
void cycle_led(void);
void watchdog_off(void);
void watchdog_init(uint8_t interval);
void sonar_init();

int main(void) {
    watchdog_off();
    usart_init();
    led_pwm_init();
    cycle_led();
    sonar_init();
    sonar_string_init();
    
    // initialize the string just to be safe
    sonar_range = sonar_string_as_int(sonar_range);
    sonar_range_previous = sonar_range + max_delta_range + 1;

    // intial state of led
    OCR0A = 0xff;
    set_led_white(&PORTB);
    turn_led_on(&PORTB);
    _delay_ms(delay1);

    // watchdog_init(WDTO_1S);
    watchdog_init(WDTO_500MS);
    
    // finished initialization, now we can enable global interupts
    sei();
    
    for(;;) {

        if (sonar_range < 30) {
            set_led_red(&PORTB);
        } else if (sonar_range > 60) {
            set_led_blue(&PORTB);
        } else {
            set_led_green(&PORTB);
        }
        uint8_t delta_range = abs(sonar_range - sonar_range_previous);
        sonar_range_previous = sonar_range;
        if (delta_range <= max_delta_range) {
            // range not changing. Disable ranging and led to conserve
            // power then go to sleep.
            turn_led_off(&PORTB);
            set_bit_false(&PORTB, SONAR_RANGING_PIN);

            sleep_mode();
            
            turn_led_on(&PORTB);
            set_bit_true(&PORTB, SONAR_RANGING_PIN);
        }
    }
  
    return 0;
}


void usart_init(void) {

    // MaxBotix LV-MaxSonar-EZ MB1010
    //   * Asynchronous serial with RS232 format
    //   * Baud rate = 9600
    //   * 8 bit characters
    //   * no parity
    //   * one stop bit

    // RX receiver enabled
    set_bit_true(&UCSRB, RXEN);
    // RX complete interrupt enable
    set_bit_true(&UCSRB, RXCIE);

#if 0
    // asynchronous mode
    set_bit_true(&UCSRC, UMSEL);
#endif
    // no parity
    set_bit_true(&UCSRC, UPM0);
    set_bit_false(&UCSRC, UPM1);
    // one stop bit
    set_bit_false(&UCSRC, USBS);
    // USART 8 bit characters:
    set_bit_true(&UCSRC, UCSZ0);
    set_bit_true(&UCSRC, UCSZ1);
    set_bit_false(&UCSRB, UCSZ2);
	    
    // 9600 baud, set by util/setbaud macros
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;

    if (DEBUG_SERIAL) {
        // enable the transmitter for debugging
        set_bit_true(&UCSRB, TXEN);
    }

    
} // end usart_init()


// header avr/iotn2313.h contains processor specific info
ISR(USART_RX_vect, ISR_BLOCK) {
    uint8_t received_byte = UDR;
    if (DEBUG_SERIAL) {
        // now echo out the received character
        UDR = received_byte;
    }
    sonar_string_add_char(received_byte);
    sonar_range = sonar_string_as_int(sonar_range);
}

void sonar_init(void) {
    // turn on sonar for continuous ranging

    // setup pin for output
    set_bit_true(&DDRB, SONAR_RANGING_PIN);
    set_bit_true(&PORTB, SONAR_RANGING_PIN);
}

void led_pwm_init(void) {
    // set output pins for led and pwm signal
    // set pins for port b0=G, b1=B, b2=A, b3=R to output
    DDRB = 0;
    set_bit_true(&DDRB, LED_RED_PIN);
    set_bit_true(&DDRB, LED_ANODE_PIN);
    set_bit_true(&DDRB, LED_BLUE_PIN);
    set_bit_true(&DDRB, LED_GREEN_PIN);

    // setup timer/counter 0 for pwm
    set_bit_true(&TCCR0B, CS00);  // internal clock source, no scaling
    // NOTE(bja, 2015-10) clk/8 is max scaling without visible blinking at 1MHz.
    
    set_bit_true(&TCCR0B, CS01);  // internal clock source, clk/8 scaling
    set_bit_true(&TCCR0A, WGM00);  // PWM, phase correct, 8 bit
    set_bit_true(&TCCR0A, COM0A1);  // Clear OC0A on Compare Match
   
    // initial PWM pulse width
    OCR0A = 0xFF;

    turn_led_off(&PORTB);  // led off
} // end led_init()


void cycle_led(void) {
    turn_led_on(&PORTB);
    _delay_ms(delay1);
    set_led_red(&PORTB);
    _delay_ms(delay1);
    set_led_blue(&PORTB);
    _delay_ms(delay1);
    set_led_green(&PORTB);
    _delay_ms(delay1);
    turn_led_off(&PORTB);
    _delay_ms(delay1);
}

void watchdog_off(void) {
    // C example from page 40 of attiny spec
    cli(); 
    MCUSR &= ~(1 << WDRF);
    /* Write logical one to WDCE and WDE */
    /* Keep old prescaler setting to prevent unintentional time-out */
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    /* Turn off WDT */
    WDTCSR = 0x00;
}

void watchdog_init(uint8_t interval) {
    // based on example from:
    // https://github.com/sparkfun/H2OhNo/blob/3b1200043102c58669a8e8270b7e9d3a3b9bb17c/firmware/H2OhNo/H2OhNo.ino
    
    cli(); 

    // max valid value at the 2313
    if (interval > 9 ) {
        interval = 9;
    }

    // bits for the prescaler aren't continuous in memory.
    uint8_t prescaler = interval & 7; 
    if (interval > 7) {
         //Set the discontinuous special 5th bit if necessary
        prescaler |= (1 << 5);
    }

    // start timed sequence for changing the watchdog
    MCUSR &= ~(1 << WDRF); //Clear the watch dog reset
    WDTCSR |= (1 << WDCE) | (1 << WDE); // enable changes to WD
    WDTCSR = prescaler; // new timeout value
    WDTCSR |= _BV(WDIE); // enable interrupts
}

ISR(WDT_OVERFLOW_vect) {
    // Every time the watchdog timer goes off, call this interrupt to
    // wake up from sleep.
}